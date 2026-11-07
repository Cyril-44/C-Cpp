#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
arbiter-plus —— ArbiterPlus 考场管理控制台 (单文件, 纯 Python 标准库)

协议逆向自 NOI ArbiterPlus 二进制 (aclient/bclient/selfeval):
  UDP : 6 字节包头 seq/len/code/status + 命令文本; 服务器 7729, 客户端 7730
  code: 01请求 02续包 04等响应 08静默 10响应数据 20最终结果 40要回执
  TCP : 7731 文件通道, 帧 = u32长度 + 文本(含NUL); put/ok/deny/echo/get
默认启动 CLI (Tab 自动补全 + 用法提示); --web 启动 WebUI; --exec 单发命令。
"""
import argparse
import cmd as cmdmod
import json
import os
import shlex
import socket
import struct
import sys
import threading
import time

try:
    import readline  # noqa: F401  (Linux 自带; 提供 Tab 补全)
except ImportError:
    readline = None

VERSION = "ArbiterPlus Console (rebuilt) 1.0"
BATCH_YES = False

SERVER_PORT = 7729
CLIENT_PORT = 7730
FILE_PORT   = 7731

CODE_REQUEST, CODE_MORE, CODE_NEEDRESP, CODE_QUIET = 0x01, 0x02, 0x04, 0x08
CODE_RESP_DATA, CODE_RESP_RES, CODE_RECEIPT = 0x10, 0x20, 0x40

STATE_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                          "arbiter-plus.state.json")
RECV_ROOT  = os.path.join(os.path.dirname(os.path.abspath(__file__)), "received")

# ---------------------------------------------------------------- 命令目录
# name: (用法, 说明, 危险级 0/1/2/3)  —— 用法串与 aclient 反编译输出一致
CATALOG = {
    "mupload":  ("mupload [-m mode] [-u owner] local-path remote-path [client...]",
                 "(服务器命令) 向多客户端推送文件, 即试题分发通道", 1),
    "echo":     ("echo [-i] [-p server_home_path] [-d day]", "回显; -i 登记本服务器为对端 server", 0),
    "help":     ("help [-v] [command]", "客户端命令帮助", 0),
    "version":  ("version", "客户端版本", 0),
    "status":   ("status", "查询客户端状态(启动时间/评测路径/命令历史/文件历史)", 0),
    "ps":       ("ps", "列出客户端正在运行的异步命令", 0),
    "sh":       ('sh [-e] "command"', "在客户端执行 shell 命令; -e 回显输出", 1),
    "userset":  ("userset [-s] [-p password] [-r] [-f] user-name", "建/删比赛用户(noiuser组); -s 设为活动用户", 2),
    "file":     ("file [-s] [-m mode] [-u owner] [-v] local-file remote-file destination",
                 "文件传输; -s 推送到对端, 否则从对端拉取", 1),
    "eval":     ("eval [-e result-path] evaluate-home [question...]",
                 "最终测试: 调 /usr/bin/eval 评测并回传 .rlt", 1),
    "pickans":  ("pickans pick upload-dir", "备份选手答案(/home/$ACTIVEUSER)并上传", 1),
    "fcheck":   ("fcheck path checksum", "md5 校验客户端文件", 0),
    "clearenv": ("clearenv [path...]", "清场: 删 /home 下指定路径+全部 noiuser 用户", 3),
    "shutdown": ("shutdown [-r]", "关闭/重启客户端机器 (不可撤销!)", 3),
    "tasknames":("tasknames", "(bclient) 读题目列表/剩余自测次数", 0),
    "selfeval": ("selfeval -u user [question...]", "(bclient) 自测评测", 0),
}

DANGER_DESC = {1: "[敏感] ", 2: "[高危] ", 3: "[危险!!] "}


def is_iplike(s):
    return s == "all" or s.replace(".", "").isdigit()


# ---------------------------------------------------------------- 状态台账
class Registry:
    def __init__(self):
        self.lock = threading.Lock()
        self.clients = []      # 已登记的客户端 IP
        self.seen = set()      # 见过的来源 IP
        self.load()

    def load(self):
        try:
            with open(STATE_FILE, encoding="utf-8") as f:
                d = json.load(f)
            self.clients = d.get("clients", [])
            self.seen = set(d.get("seen", []))
        except Exception:
            pass

    def save(self):
        try:
            with open(STATE_FILE, "w", encoding="utf-8") as f:
                json.dump({"clients": self.clients,
                           "seen": sorted(self.seen)}, f, ensure_ascii=False, indent=1)
        except Exception:
            pass

    def add(self, ip):
        with self.lock:
            if ip not in self.clients:
                self.clients.append(ip)
            self.seen.add(ip)
            self.save()

    def remove(self, ip):
        with self.lock:
            if ip in self.clients:
                self.clients.remove(ip)
            self.save()

    def list(self):
        with self.lock:
            return list(self.clients)

    def known_ips(self):
        with self.lock:
            return sorted(set(self.clients) | self.seen)


# ---------------------------------------------------------------- UDP 引擎
class _Wait:
    def __init__(self, ips):
        self.expected = set(ips)
        self.results = []          # (ip, res, info)
        self.progress = []         # (ip, text)
        self.done = threading.Event()


class ArbiterNet(threading.Thread):
    """监听 UDP 7729: 收响应(转发给等待者) + 收客户端主动通知(selfevalresult)"""

    def __init__(self, registry, port=SERVER_PORT):
        super().__init__(daemon=True)
        self.registry = registry
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(("0.0.0.0", port))
        self.sock.settimeout(0.3)
        self.port = port
        self.waits = {}            # seq -> _Wait
        self.lock = threading.Lock()
        self.seq = int(time.time()) & 0x7fff
        self.on_notify = lambda msg: print(msg)

    # -- 发送并收集响应 (等价于反编译里的 SendCmd::run) --
    def send_collect(self, command, targets, timeout=5, broadcast=False, grab=True):
        with self.lock:
            self.seq = (self.seq + 1) & 0xffff
            seq = self.seq
        ips = []
        for t in targets:
            if t not in ips:
                ips.append(t)
        w = _Wait(ips)
        if not broadcast:
            with self.lock:
                self.waits[seq] = w

        # 0x4D = 0x45|0x08: 同步收响应 + 抓取客户端 stdout 回传 (反编译 SendCmd -v 的语义)
        code = CODE_REQUEST | CODE_NEEDRESP | CODE_RECEIPT | (CODE_QUIET if grab else 0)
        payload = command.encode("utf-8", "replace")
        if len(payload) + 6 >= 256:
            return None, ["命令过长 (>249 字节)"], ["命令过长 (>249 字节)"]

        pkt = struct.pack("<HHBB", seq, len(payload) + 6, code, 0) + payload
        dests = ["255.255.255.255"] if broadcast else ips
        sent = 0
        for d in dests:
            try:
                self.sock.sendto(pkt, (d, CLIENT_PORT))
                sent += 1
            except OSError as e:
                w.progress.append((d, "发送失败: %s" % e))
        if sent == 0:
            with self.lock:
                self.waits.pop(seq, None)
            return None, w.progress, []

        deadline = time.time() + max(1, timeout)
        while time.time() < deadline and not w.done.is_set():
            w.done.wait(0.2)
        with self.lock:
            self.waits.pop(seq, None)
        return w.results, w.progress, sorted(w.expected)

    # -- 收包循环 --
    def run(self):
        while True:
            try:
                data, addr = self.sock.recvfrom(2048)
            except socket.timeout:
                continue
            except OSError:
                continue
            if len(data) < 6:
                continue
            ip = addr[0]
            self.registry.seen.add(ip)
            seq, ln, code, _st = struct.unpack("<HHBB", data[:6])
            body = data[6:].decode("utf-8", "replace")

            if code & CODE_REQUEST:                      # 客户端主动请求(通知)
                self._handle_notify(ip, body)
                continue
            with self.lock:
                w = self.waits.get(seq)
            if w is None:
                continue
            if (code & 0x30) == CODE_RESP_DATA:
                w.progress.append((ip, body))
                self._maybe(w)
            elif (code & 0x30) == CODE_RESP_RES:
                res, info = -1, ""
                if body.startswith("&"):
                    parts = body[1:].split(" ", 1)
                    try:
                        res = int(parts[0])
                    except ValueError:
                        pass
                    info = parts[1] if len(parts) > 1 else ""
                w.results.append((ip, res, info))
                w.expected.discard(ip)
                self._maybe(w)

    def _maybe(self, w):
        if not w.expected:
            w.done.set()

    def _handle_notify(self, ip, body):
        toks = body.split()
        if toks and toks[0] == "selfevalresult" and len(toks) >= 6:
            d = dict(t.split(" ", 1) if " " in t else (t, "") for t in toks[1:])
            self.on_notify("[通知] %s 自测结果到达: 用户=%s 时间戳=%s 日次=%s "
                           "(结果目录 /home/self_evaluate/result/%s)"
                           % (ip, d.get("-u", "?"), d.get("-t", "?"),
                              d.get("-d", "?"), d.get("-t", "?")))
        else:
            self.on_notify("[通知] %s: %s" % (ip, body))


# ---------------------------------------------------------------- TCP 文件接收
def conf_remote_file_port(default=FILE_PORT):
    try:
        with open("/etc/arbiter.conf", encoding="utf-8") as f:
            for line in f:
                if "=" not in line:
                    continue
                k, v = line.split("=", 1)
                if k.strip() == "RemoteFilePort":
                    return int(v.strip())
    except Exception:
        pass
    return default


class FileReceiver(threading.Thread):
    """监听 TCP 7731, 实现 put 协议 (逆向自 FClientHandler/FClient):
       帧 = u32 len + len 字节(含 NUL); put → ok <size> <flag> → 裸数据 size 字节 → echo 0"""

    def __init__(self, port=FILE_PORT, root=RECV_ROOT):
        super().__init__(daemon=True)
        self.port, self.root = port, root
        self.on_file = lambda path, frm: print("[文件] 收到 %s (来自 %s)" % (path, frm))

    def run(self):
        srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            srv.bind(("0.0.0.0", self.port))
        except OSError as e:
            print("[文件] TCP %d 绑定失败: %s" % (self.port, e))
            return
        srv.listen(8)
        while True:
            try:
                conn, addr = srv.accept()
            except OSError:
                continue
            threading.Thread(target=self._serve, args=(conn, addr), daemon=True).start()

    @staticmethod
    def _readn(sock, n):
        buf = b""
        while len(buf) < n:
            c = sock.recv(n - len(buf))
            if not c:
                return None
            buf += c
        return buf

    def _read_cmd(self, sock):
        raw = self._readn(sock, 4)
        if raw is None:
            return None
        (n,) = struct.unpack("<I", raw)
        if n < 2 or n > 65536:
            return None
        data = self._readn(sock, n)
        if data is None:
            return None
        return data.rstrip(b"\0").decode("utf-8", "replace")

    def _write_cmd(self, sock, text):
        b = text.encode("utf-8") + b"\0"
        sock.sendall(struct.pack("<I", len(b)) + b)

    def _safe_path(self, remote):
        p = remote.strip().lstrip("/")
        parts = [x for x in p.split("/") if x not in ("", ".", "..")]
        return os.path.join(self.root, *parts) if parts else self.root

    def _serve(self, conn, addr):
        try:
            conn.settimeout(120)
            req = self._read_cmd(conn)
            if not req:
                return
            toks = req.split()
            if toks and toks[0] == "put":
                # put <id> <size> <flag> <local> <remote> <owner> <mode> [filter]
                size = int(toks[2]) if len(toks) > 2 else 0
                remote = toks[5] if len(toks) > 5 else "unnamed"
                self._write_cmd(conn, "ok %d 0" % size)
                data = self._readn(conn, size) if size > 0 else b""
                if data is not None:
                    path = self._safe_path(remote)
                    parent = os.path.dirname(path)
                    if os.path.exists(parent) and not os.path.isdir(parent):
                        os.unlink(parent)          # 收件区内同名文件挡路则清掉
                    os.makedirs(parent, exist_ok=True)
                    final = path
                    n = 1
                    while os.path.isdir(final):     # 远端是目录语义时落到文件
                        final = "%s.%d" % (path, n); n += 1
                    with open(final, "wb") as f:
                        f.write(data)
                    os.chmod(final, 0o755)
                    self.on_file(final, addr[0])
                self._write_cmd(conn, "echo 0")
            elif toks and toks[0] == "get":
                self._write_cmd(conn, "deny -1")
            else:
                self._write_cmd(conn, "deny -1")
        except Exception as e:
            print("[文件] 连接处理异常: %s" % e)
        finally:
            try:
                conn.close()
            except Exception:
                pass


# ---------------------------------------------------------------- 文件推送
def read_frame(sock):
    raw = b""
    while len(raw) < 4:
        c = sock.recv(4 - len(raw))
        if not c:
            return None
        raw += c
    (n,) = struct.unpack("<I", raw)
    buf = b""
    while len(buf) < n:
        c = sock.recv(n - len(buf))
        if not c:
            return None
        buf += c
    return buf.rstrip(bytes((0,))).decode("utf-8", "replace")


def tcp_put(ip, local, remote, port=FILE_PORT, mode=0o755, owner="root"):
    """等价 original MUploadCmd 展开的 "file -s -m mode [-u owner] local remote client"
    在服务器本地执行: FClient 连客户端 7731, put/ok/数据/echo (实测过的协议)"""
    with open(local, "rb") as f:
        data = f.read()
    s = socket.create_connection((ip, port), timeout=15)
    try:
        s.settimeout(15)
        req = "put %d %d 0 %s %s %s %o" % (int(time.time()) & 0xffff, len(data),
                                            local, remote, owner if owner else "root", mode)
        s.sendall(struct.pack("<I", len(req) + 1) + req.encode() + bytes((0,)))
        resp = read_frame(s)
        if resp is None or not resp.startswith("ok"):
            return False, "对端拒绝: %r" % resp
        s.sendall(data)
        resp2 = read_frame(s)
        if resp2 is None or not resp2.startswith("echo 0"):
            return False, "回执异常: %r" % resp2
        return True, "%d bytes" % len(data)
    finally:
        s.close()


# ---------------------------------------------------------------- 引擎门面
class Engine:
    def __init__(self, file_port=None):
        self.registry = Registry()
        self.net = ArbiterNet(self.registry)
        self.files = FileReceiver(port=file_port or conf_remote_file_port())
        self.net.on_notify = self._notify
        self.files.on_file = self._file
        self.notify_log = []
        self.file_log = []
        self.net.start()
        self.files.start()

    def _notify(self, msg):
        self.notify_log.append(msg)
        print(msg)

    def _file(self, path, frm):
        self.file_log.append((time.time(), path, frm))
        print("[文件] %s ← %s" % (path, frm))

    # targets 解析: 空默认 all; 支持 all/具体 IP/逗号分隔
    def resolve(self, args):
        if not args or args == ["all"]:
            ips = self.registry.list()
            if not ips:
                raise SystemExit("没有已登记客户端, 先: clients add <ip> 或 scan")
            return ips
        out = []
        for a in args:
            for p in a.split(","):
                p = p.strip()
                if p and p not in out:
                    out.append(p)
        return out

    def send(self, command, targets, timeout=5, broadcast=False, grab=True):
        results, progress, missing = self.net.send_collect(
            command, targets, timeout=timeout, broadcast=broadcast, grab=grab)
        lines = []
        for ip, text in (progress or []):
            for ln in text.splitlines():
                if ln.strip():
                    lines.append("  [%s] %s" % (ip, ln.rstrip()))
        for ip, res, info in (results or []):
            mark = "OK " if res == 0 else "ERR"
            lines.append("  [%s] %s %s res=%d %s" % (ip, mark, command.split()[0], res, info))
        for ip in (missing or []):
            lines.append("  [%s] 超时未响应" % ip)
        if broadcast:
            for ip in self.registry.known_ips():
                pass
        return results, lines

    def scan(self, timeout=3):
        _, lines = self.send("version", [], timeout=timeout, broadcast=True)
        # 广播响应会登记 seen
        for ip in self.registry.known_ips():
            if ip not in ("127.0.0.1",):
                self.registry.add(ip)
        return lines


# ---------------------------------------------------------------- CLI
class Console(cmdmod.Cmd):
    intro = (
        "\n" + VERSION + "\n"
        "UDP %d(命令) / TCP %d(文件) 已监听 | 命令回车执行, Tab 补全, help 查看内建\n"
        "发客户端命令: send -s \"status\" <ip|all>   或直接 status all\n\n" % (SERVER_PORT, FILE_PORT))
    prompt = "arbiter-plus> "

    def __init__(self, engine):
        super().__init__()
        self.engine = engine
        self._last_hint = ""

    # ---------- 调度 ----------
    def _dispatch_line(self, line):
        line = line.strip()
        if not line or line.startswith("#"):
            return
        try:
            toks = shlex.split(line)
        except ValueError as e:
            print("参数引号错误: %s" % e)
            return
        if not toks:
            return
        head = toks[0]
        if head in ("send",):
            self._cmd_send(toks[1:])
        elif head in ("scan",):
            self._cmd_scan()
        elif head in ("clients",):
            self._cmd_clients(toks[1:])
        elif head in ("received",):
            self._cmd_received()
        elif head == "mupload":
            self._cmd_mupload(toks[1:])
        elif head in ("webui",):
            self._cmd_webui()
        elif head == "quit" or head == "exit":
            return True
        elif head in CATALOG:
            self._shortcut(head, toks[1:])
        else:
            print("未知命令: %s (help 查看; Tab 补全)" % head)

    def default(self, line):
        self._dispatch_line(line)

    def emptyline(self):
        pass

    def do_quit(self, _):
        return True

    do_exit = do_quit

    # ---------- send ----------
    def _cmd_send(self, args):
        timeout = 5
        sync = False
        cmd_parts, targets = [], []
        i = 0
        while i < len(args):
            a = args[i]
            if a == "-t" and i + 1 < len(args):
                timeout = int(args[i + 1]); i += 2; continue
            if a == "-s":
                sync = True; i += 1; continue
            cmd_parts.append(a); i += 1
        # 末尾连续的 ip/all 视为目标
        while cmd_parts and is_iplike(cmd_parts[-1]) and len(cmd_parts) > 1:
            targets.insert(0, cmd_parts.pop())
        if not cmd_parts:
            print('用法: send [-s] [-t 秒] "客户端命令" <ip|all> ...')
            return
        command = " ".join(cmd_parts)
        name = command.split()[0]
        if name in CATALOG and CATALOG[name][2] >= 2 and not self._confirm(name):
            return
        try:
            ips = self.engine.resolve(targets)
        except SystemExit as e:
            print(e); return
        print(">>> %s  → %s (超时 %ds)" % (command, ",".join(ips), timeout))
        _, lines = self.engine.send(command, ips, timeout=timeout)
        print("\n".join(lines) if lines else "  (无响应)")

    # ---------- 一键快捷: 直接以客户端命令名开头 ----------
    def _shortcut(self, name, args):
        self._cmd_send(["-s"] + [name] + args)

    def _cmd_scan(self):
        print(">>> 广播探测 version (3s) ...")
        self.engine.scan()
        for ip in self.engine.registry.list():
            print("  发现: %s" % ip)
        if not self.engine.registry.list():
            print("  (无响应; 确认 aclient 已在 7730 监听)")

    # ---------- clients ----------
    def _cmd_clients(self, args):
        if not args or args[0] in ("ls", "list"):
            ips = self.engine.registry.list()
            print("已登记客户端 %d 台:" % len(ips))
            for ip in ips:
                print("  %s" % ip)
        elif args[0] == "add":
            for ip in args[1:]:
                self.engine.registry.add(ip)
                print("  + %s" % ip)
        elif args[0] in ("rm", "remove", "del"):
            for ip in args[1:]:
                self.engine.registry.remove(ip)
                print("  - %s" % ip)
        else:
            print("用法: clients [ls|add <ip>|rm <ip>]")

    def _cmd_received(self):
        if not self.engine.file_log:
            print("暂未收到文件 (目录: %s)" % RECV_ROOT)
            return
        for t, path, frm in self.engine.file_log[-20:]:
            print("  %s %s ← %s" % (time.strftime("%H:%M:%S", time.localtime(t)), path, frm))

    # ---------- mupload (完全对齐 original MUploadCmd::run) ----------
    def _cmd_mupload(self, args):
        """mupload [-m mode] [-u owner] local-path remote-path [client...]
        逻辑 = 反编译 MUploadCmd: 对每个 client 构造 file -s 并在本地执行"""
        mode, owner = "0755", ""
        rest = []
        i = 0
        while i < len(args):
            if args[i] == "-m" and i + 1 < len(args):
                mode = args[i + 1]; i += 2; continue
            if args[i] == "-u" and i + 1 < len(args):
                owner = args[i + 1]; i += 2; continue
            rest.append(args[i]); i += 1
        if len(rest) < 3:
            print("用法: mupload [-m mode] [-u owner] <local> <remote> <client|all> ...")
            return
        local, remote = rest[0], rest[1]
        try:
            ips = self.engine.resolve(rest[2:])
        except SystemExit as e:
            print(e); return
        if not os.path.isfile(local):
            print("本机文件不存在: %s" % local); return
        try:
            mode_i = int(mode, 8)
        except ValueError:
            print("mode 需为八进制, 如 0755"); return
        for ip in ips:
            # = MUploadCmd: file -s -m mode [-u owner] local remote client -> 本地执行
            ok, msg = tcp_put(ip, local, remote, mode=mode_i, owner=owner)
            res = 0 if ok else -1
            print("  [result] %s: res=%d info=%s" % (ip, res, msg))

    # ---------- webui ----------
    def _cmd_webui(self):
        start_webui(self.engine)

    # ---------- 危险确认 ----------
    def _confirm(self, name):
        if BATCH_YES:
            print("(--yes) 自动确认: %s" % name)
            return True
        usage, help_, lvl = CATALOG[name]
        print("%s%s  %s" % (DANGER_DESC.get(lvl, ""), usage,
                            help_ if lvl < 3 else "!!! 将影响客户端机器 !!!"))
        try:
            return input("确认执行? 输入 yes: ").strip().lower() == "yes"
        except EOFError:
            return False

    # ---------- Tab 补全 ----------
    def _complete_ips(self, text):
        ips = self.engine.registry.known_ips() + ["all", "127.0.0.1"]
        return [i + " " for i in ips if i.startswith(text)]

    def _complete_catalog(self, text):
        out = []
        for name, (usage, help_, lvl) in sorted(CATALOG.items()):
            if name.startswith(text):
                out.append(name + " ")
                self._last_hint = "  # %s%s — %s" % (DANGER_DESC.get(lvl, ""), usage, help_)
        return out

    def _show_hint(self):
        if self._last_hint:
            print("\n" + self._last_hint)
            self._last_hint = ""
            if readline:
                try:
                    readline.redisplay()
                except Exception:
                    pass

    def completer(self, text, state):
        try:
            return self._completer(text, state)
        except Exception:
            return None

    def _completer(self, text, state):
        line = readline.get_line_buffer() if readline else ""
        toks = line.lstrip().split()
        ends_space = line.endswith(" ")
        if not toks or (len(toks) == 1 and not ends_space and text):
            # 一级: 控制台命令 + 客户端命令
            opts = self._complete_catalog(text)
            for c in ("send ", "scan", "clients ", "received", "webui",
                      "mupload ", "quit"):
                if c.startswith(text):
                    opts.append(c)
            self._show_hint()
            return opts[state] if state < len(opts) else None
        head = toks[0]
        if head == "send":
            pos = len(toks) - (0 if ends_space else 1)
            if pos <= 1:
                opts = self._complete_catalog(text)
                self._show_hint()
                return opts[state] if state < len(opts) else None
            opts = self._complete_ips(text)
            name = toks[1] if len(toks) > 1 else ""
            if name in CATALOG:
                usage, help_, _lvl = CATALOG[name]
                for flag in usage.split():
                    if flag.startswith("-") and flag.startswith(text):
                        opts.append(flag + " ")
                self._last_hint = "  # %s — %s" % (usage, help_)
                self._show_hint()
            return opts[state] if state < len(opts) else None
        if head in CATALOG:
            opts = self._complete_ips(text)
            usage, help_, _lvl = CATALOG[head]
            for flag in usage.split():
                if flag.startswith("-") and flag.startswith(text):
                    opts.append(flag + " ")
            self._last_hint = "  # %s — %s" % (usage, help_)
            self._show_hint()
            return opts[state] if state < len(opts) else None
        if head == "clients":
            opts = [o + " " for o in ("ls", "add", "rm") if o.startswith(text)]
            if "add" in toks:
                opts += self._complete_ips(text)
            return opts[state] if state < len(opts) else None
        return None

    # cmd.Cmd 挂钩自定义 completer
    def preloop(self):
        if readline:
            readline.set_completer(self.completer)
            try:
                readline.parse_and_bind("tab: complete")
            except Exception:
                pass


# ---------------------------------------------------------------- WebUI
def start_webui(engine, port=8080):
    from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

    page = """<!doctype html><html><head><meta charset="utf-8"><title>ArbiterPlus</title>
<style>
body{font-family:system-ui,'Segoe UI',sans-serif;background:#10141a;color:#d7dde6;margin:0;padding:24px}
h1{font-size:20px;margin:0 0 4px}.sub{color:#7b8694;font-size:12px;margin-bottom:20px}
.panel{background:#171d26;border:1px solid #232c38;border-radius:10px;padding:16px;margin-bottom:16px}
table{border-collapse:collapse;width:100%}th,td{padding:8px 10px;text-align:left;border-bottom:1px solid #232c38;font-size:13px}
th{color:#8fa0b3;font-weight:600}.tag{display:inline-block;padding:1px 8px;border-radius:10px;background:#22303f;font-size:11px;margin-right:6px}
button{background:#26517e;color:#fff;border:0;border-radius:6px;padding:6px 12px;font-size:13px;cursor:pointer;margin:2px}
button:hover{background:#31649c}button.warn{background:#7e3b26}button.warn:hover{background:#9c4a30}
input[type=text]{width:70%;background:#0d1117;color:#d7dde6;border:1px solid #2c3743;border-radius:6px;padding:8px;font-size:13px}
pre{background:#0d1117;border:1px solid #232c38;border-radius:8px;padding:12px;font-size:12px;max-height:340px;overflow:auto;white-space:pre-wrap}
a{color:#6ea3d8}
</style></head><body>
<h1>ArbiterPlus 考场管理</h1><div class="sub">UDP 7729 · TCP 7731 · 状态文件 arbiter-plus.state.json</div>
<div class="panel"><b>机器</b> <button onclick="api('scan','')">扫描</button>
<button onclick="api('reload','')">刷新</button>
<span style="float:right"><input id="ip" placeholder="IP" size="12"><button onclick="api('add',document.getElementById('ip').value)">登记</button></span></div>
<div class="panel"><table id="tb"><tr><th>机器</th><th>操作</th></tr></table></div>
<div class="panel"><b>发命令</b><br><br>
<input list="cmds" id="cmd" type="text" placeholder='如: status   (Tab/点击选补全, 末尾用 all 或留空=全部机器)'>
<datalist id="cmds">%DATALIST%</datalist>
<button onclick="run(document.getElementById('cmd').value, '')">发送</button>
<button class="warn" onclick="run('shutdown', prompt('输入 yes 确认关机全部机器:')==='yes')">shutdown 全部</button></div>
<div class="panel"><b>输出</b><pre id="out">ready.</pre></div>
<script>
const CMDS=%CATALOGJSON%;
function refresh(){fetch('/api/state').then(r=>r.json()).then(d=>{
 const tb=document.getElementById('tb');tb.innerHTML='<tr><th>机器</th><th>一键命令</th></tr>';
 (d.clients||[]).forEach(ip=>{const tr=document.createElement('tr');
  tr.innerHTML='<td>'+ip+'</td><td>'+['status','version','ps','echo -i'].map(c=>
   '<button onclick="run(\\''+c+'\\',\\''+ip+'\\')">'+c+'</button>').join('')+
   ' <button class="warn" onclick="run(\\'sh -e id\\',\\''+ip+'\\')">id</button></td>';tb.appendChild(tr);});});}
function run(cmd,ip){document.getElementById('out').textContent='>>> '+cmd+' → '+(ip||'all')+'\\n...';
 fetch('/api/run',{method:'POST',body:JSON.stringify({cmd,ip})}).then(r=>r.text()).then(t=>{
  document.getElementById('out').textContent='>>> '+cmd+'\\n'+t;});}
function api(a,v){fetch('/api/'+a,{method:'POST',body:v}).then(()=>refresh());if(a==='scan')setTimeout(refresh,3500);}
refresh();setInterval(refresh,5000);
</script></body></html>"""

    datalist = "".join('<option value="%s">%s</option>' % (v, CATALOG[k][0])
                       for k, v in [(k, k + " ") for k in CATALOG])
    catalog_json = json.dumps({k: v[0] for k, v in CATALOG.items()}, ensure_ascii=False)
    page = page.replace("%DATALIST%", datalist).replace("%CATALOGJSON%", catalog_json)

    class H(BaseHTTPRequestHandler):
        def log_message(self, *a):
            pass

        def _send(self, code, body, ctype="text/html; charset=utf-8"):
            b = body.encode("utf-8")
            self.send_response(code)
            self.send_header("Content-Type", ctype)
            self.send_header("Content-Length", str(len(b)))
            self.end_headers()
            self.wfile.write(b)

        def do_GET(self):
            if self.path.startswith("/api/state"):
                self._send(200, json.dumps({"clients": engine.registry.list()}),
                                           "application/json")
            else:
                self._send(200, page)

        def do_POST(self):
            n = int(self.headers.get("Content-Length") or 0)
            body = self.rfile.read(n).decode("utf-8", "replace") if n else ""
            if self.path == "/api/run":
                try:
                    req = json.loads(body)
                    cmd = req.get("cmd", "").strip()
                    ip = req.get("ip", "").strip()
                except Exception:
                    self._send(400, "bad json", "text/plain"); return
                if not cmd:
                    self._send(400, "empty", "text/plain"); return
                name = cmd.split()[0]
                if name in CATALOG and CATALOG[name][2] >= 2:
                    self._send(200, "已拦截高危命令 %s (CLI 下需人工确认)" % name, "text/plain")
                    return
                try:
                    ips = engine.resolve([ip] if ip else [])
                except SystemExit as e:
                    self._send(200, str(e), "text/plain"); return
                timeout = 60 if name in ("eval", "pickans", "selfeval") else 8
                _, lines = engine.send(cmd, ips, timeout=timeout)
                self._send(200, "\n".join(lines) or "(无响应)", "text/plain; charset=utf-8")
            elif self.path == "/api/scan":
                engine.scan()
                self._send(200, "scanning...", "text/plain")
            elif self.path == "/api/add":
                engine.registry.add(body.strip())
                self._send(200, "ok", "text/plain")
            elif self.path == "/api/reload":
                self._send(200, "ok", "text/plain")
            else:
                self._send(404, "-", "text/plain")

    try:
        srv = ThreadingHTTPServer(("0.0.0.0", port), H)
    except OSError as e:
        print("WebUI 启动失败: %s" % e)
        return
    print("WebUI: http://127.0.0.1:%d  (Ctrl+C 停止)" % port)
    threading.Thread(target=srv.serve_forever, daemon=True).start()


# ---------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser(description="ArbiterPlus 考场管理控制台")
    ap.add_argument("--exec", dest="batch", help="非交互执行一条控制台命令")
    ap.add_argument("--web", action="store_true", help="同时启动 WebUI")
    ap.add_argument("--web-port", type=int, default=8080)
    ap.add_argument("--clients", help="预登记客户端, 逗号分隔")
    ap.add_argument("--yes", action="store_true", help="非交互模式跳过高危命令确认")
    ap.add_argument("--file-port", type=int, default=None,
                    help="文件接收端口 (默认读 /etc/arbiter.conf RemoteFilePort, 再退 7731)")
    args = ap.parse_args()

    engine = Engine(file_port=args.file_port)
    global BATCH_YES
    BATCH_YES = args.yes or bool(args.batch)
    if args.clients:
        for ip in args.clients.split(","):
            engine.registry.add(ip.strip())

    if args.web:
        start_webui(engine, args.web_port)

    if args.batch:
        Console(engine)._dispatch_line(args.batch)
        time.sleep(0.2)          # 让收尾日志落地
        return

    if args.web and not sys.stdin.isatty():
        print("WebUI 模式驻留中 (Ctrl+C 退出)")
        try:
            while True:
                time.sleep(3600)
        except KeyboardInterrupt:
            pass
        return

    try:
        Console(engine).cmdloop()
    except KeyboardInterrupt:
        print()


if __name__ == "__main__":
    main()
