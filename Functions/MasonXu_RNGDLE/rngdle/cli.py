# Rich-based TUI with Unicode block digits, rarity label and scrollable badge list
import random
import time
import threading
import sys
import termios
import tty
import csv
from datetime import datetime
from collections import Counter
from typing import Optional, List, Tuple
import os
import readchar
from rich.console import Console, Group
from rich.panel import Panel
from rich.live import Live
from rich.align import Align
from rich.table import Table
from rich.text import Text
from rich.columns import Columns
from rich import box
from rich.layout import Layout
from .rngdle import *

console = Console()

# ---------------------------------------------------------------------------
# Block digit patterns (5x5)
# ---------------------------------------------------------------------------
BLOCK = '█'
SPACE = ' '
DIGITS = {
    '0': [
        " ██████ ",
        "██    ██",
        "██    ██",
        "██    ██",
        "██    ██",
        "██    ██",
        " ██████ ",
    ],
    '1': [
        "   ██   ",
        " ████   ",
        "   ██   ",
        "   ██   ",
        "   ██   ",
        "   ██   ",
        " ██████ ",
    ],
    '2': [
        " ██████ ",
        "██    ██",
        "      ██",
        "  ████  ",
        "██      ",
        "██      ",
        "████████",
    ],
    '3': [
        " ██████ ",
        "██    ██",
        "      ██",
        "  █████ ",
        "      ██",
        "██    ██",
        " ██████ ",
    ],
    '4': [
        "██    ██",
        "██    ██",
        "██    ██",
        "████████",
        "      ██",
        "      ██",
        "      ██",
    ],
    '5': [
        "████████",
        "██      ",
        "██      ",
        "███████ ",
        "      ██",
        "██    ██",
        " ██████ ",
    ],
    '6': [
        " ██████ ",
        "██      ",
        "██      ",
        "███████ ",
        "██    ██",
        "██    ██",
        " ██████ ",
    ],
    '7': [
        "████████",
        "      ██",
        "     ██ ",
        "    ██  ",
        "   ██   ",
        "  ██    ",
        " ██     ",
    ],
    '8': [
        " ██████ ",
        "██    ██",
        "██    ██",
        " ██████ ",
        "██    ██",
        "██    ██",
        " ██████ ",
    ],
    '9': [
        " ██████ ",
        "██    ██",
        "██    ██",
        " ███████",
        "      ██",
        "      ██",
        " ██████ ",
    ],
    ' ': [
        "        ",
        "        ",
        "        ",
        "        ",
        "        ",
        "        ",
        "        ",
    ],
}



# ---------------------------------------------------------------------------
# Rendering helpers
# ---------------------------------------------------------------------------
def render_big_number_text(
    s: str,
    rolling_digit: Tuple[int,str] = (-1,''),
    confirmed_style: str = 'bold white',
    rolling_style: str = 'grey30',
) -> Text:
    out = Text('')
    def fill_leading_zeroes_to_blanks(s: str):
        for idx, ch in enumerate(s):
            if (ch != '0'):
                return ' '*idx + s[idx:]
        return s
    if rolling_digit[0] != -1:
        s = fill_leading_zeroes_to_blanks(s)[:rolling_digit[0]] + rolling_digit[1] + ' '*(len(s)-rolling_digit[0]-1)
    for row in range(7):
        line = Text('')
        for idx, ch in enumerate(s):
            pat = DIGITS[ch][row]
            style = rolling_style if (idx == rolling_digit[0]) else confirmed_style
            line.append(pat + ' ', style=style)
        out.append(line)
        if (row != 6):
            out.append('\n')
    return out

def build_badge_table(
    badges: List[Tuple[str, str, str, int]],  # (name, lbl, st, ep)
    start: int,
    height: int,
) -> Panel:
    total = len(badges)
    tbl = Table(
        show_header=True,
        header_style="bold",
        box=None,
        pad_edge=False,
        expand=True,
        padding=(0, 0),
    )
    tbl.add_column("Name", ratio=6, justify="left", no_wrap=True)
    tbl.add_column("Label", ratio=3, justify="center", no_wrap=True)
    tbl.add_column("EP", ratio=3, justify="right", no_wrap=True)
    tbl.add_column("", width=1, justify="right")  # 滚动条

    end = min(total, start + height)

    # ---- 滚动条计算 ----
    if total <= height or height <= 0:
        bar_positions = set()
    else:
        bar_h = max(1, int(height * height / total))
        max_start = max(1, total - height)
        bar_pos = int((start / max_start) * (height - bar_h)) if max_start > 0 else 0
        bar_positions = set(range(bar_pos, bar_pos + bar_h))

    # ---- 内容行 ----
    for i, (name, lbl, st, ep) in enumerate(badges[start:end]):
        style = label_to_style(lbl)

        # 滚动条字符
        if i in bar_positions:
            bar_char = Text("█", style="bright_cyan")
        else:
            bar_char = Text("│", style="dim")

        tbl.add_row(
            Text(name, style=st),
            Text(lbl, style=style),
            str(ep),
            bar_char,
        )

    # ---- 补空行，使滚动条对齐 ----
    for i in range(end - start, height):
        bar_char = Text("█", style="bright_cyan") if i in bar_positions else Text("│", style="dim")
        tbl.add_row("", "", "", bar_char)

    return Panel(
        tbl,
        title=f"Badges ({total})",
        title_align="left",
        padding=(0, 1),
        border_style="dim",
    )


def build_roll_history_panel(
    history: List[Tuple[str, int, str, int]],
    start: int,
    height: int,
    n_digits: int = 9,
) -> Panel:
    """左侧 History：刚好放下内容 + 右侧滚动条"""
    total = len(history)
    num_w = max(n_digits, 4) + 1
    ep_w = 7
    rarity_w = 12
    rk_w = 5

    tbl = Table(
        show_header=True,
        header_style="bold",
        box=None,
        pad_edge=False,
        expand=True,
        padding=(0, 0),
    )
    tbl.add_column("Num",    width=num_w,    justify="left",   no_wrap=True)
    tbl.add_column("EP",     ratio=2,     justify="right",  no_wrap=True)
    tbl.add_column("Rarity", ratio=3, justify="center", no_wrap=True)
    tbl.add_column("rk",     ratio=1,     justify="right",  no_wrap=True)
    tbl.add_column("",       width=1,        justify="right")  # 滚动条列

    end = min(total, start + height)

    # 计算滚动条位置
    if total <= height or height <= 0:
        # 不需要滚动
        bar_positions = set()
    else:
        # 滚动条高度至少 1
        bar_h = max(1, int(height * height / total))
        # 当前滚动位置映射到滚动条
        max_start = max(1, total - height)
        bar_pos = int((start / max_start) * (height - bar_h)) if max_start > 0 else 0
        bar_positions = set(range(bar_pos, bar_pos + bar_h))

    for i, (num, ep, label, rk) in enumerate(history[start:end]):
        style = label_to_style(label)
        # 滚动条字符
        if i in bar_positions:
            bar_char = Text("█", style="bright_cyan")
        else:
            bar_char = Text("│", style="dim")
        tbl.add_row(num, str(ep), Text(label, style=style), str(rk), bar_char)

    # 如果内容不足 height 行，补空行使滚动条对齐
    for i in range(end - start, height):
        bar_char = Text("█", style="bright_cyan") if i in bar_positions else Text("│", style="dim")
        tbl.add_row("", "", "", "", bar_char)

    return Panel(
        tbl,
        title=f"History ({total})",
        title_align="left",
        padding=(0, 1),
        border_style="dim",
    )


def build_rarity_stats_panel(
    history: List[Tuple[str, int, str, int]],
) -> Panel:
    """右侧：完整显示稀有度统计（不强制宽度，让它自然撑开）"""
    tbl = Table(
        show_header=True,
        header_style="bold",
        box=None,
        pad_edge=False,
        expand=True,
        padding=(0, 1),
    )
    tbl.add_column("Rarity", ratio=3, no_wrap=True)
    tbl.add_column("Count",  ratio=2, justify="right", no_wrap=True)
    tbl.add_column("Prob",   ratio=2, justify="right", no_wrap=True)

    total = max(1, len(history))
    counts = Counter(lbl for _, _, lbl, _ in history)

    for r in RARITY_ORDER:
        c = counts.get(r, 0)
        pct = (c / total) * 100.0
        style = label_to_style(r)
        tbl.add_row(
            Text(r, style=style),
            str(c),
            Text(f"{pct:5.1f}%", style=style),
        )

    for r, c in counts.items():
        if r not in RARITY_ORDER:
            pct = (c / total) * 100.0
            style = label_to_style(r)
            tbl.add_row(
                Text(r, style=style),
                str(c),
                Text(f"{pct:5.1f}%", style=style),
            )

    return Panel(
        tbl,
        title="Rarity Stats",
        title_align="left",
        padding=(0, 1),
        border_style="dim",
    )

# ---------------------------------------------------------------------------
# Key watcher (kept for skip support during animation)
# ---------------------------------------------------------------------------
class KeyWatcher:
    def __init__(self):
        self.lock = threading.Lock()
        self.key = None
        self.thread = None

    def start_once(self):
        if self.thread and self.thread.is_alive():
            return
        self.key = None
        self.thread = threading.Thread(target=self._read, daemon=True)
        self.thread.start()

    def _read(self):
        try:
            k = readchar.readkey()
            with self.lock:
                self.key = k
        except Exception:
            pass

    def pop(self):
        with self.lock:
            k = self.key
            self.key = None
            return k


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    # optional --digits / -n
    n_digits = 9
    try:
        import argparse
        parser = argparse.ArgumentParser(add_help=False)
        parser.add_argument('-n', '--digits', type=int, default=9)
        args, _ = parser.parse_known_args()
        n_digits = max(1, args.digits)
    except Exception:
        pass

    fd = None
    orig_attrs = None
    is_tty = False
    try:
        fd = sys.stdin.fileno()
        is_tty = sys.stdin.isatty()
        if is_tty:
            orig_attrs = termios.tcgetattr(fd)
    except Exception:
        pass

    try:
        kw = KeyWatcher()
        badges_history: List[Tuple[str, str, str, int]] = []   # newest first
        roll_history: List[Tuple[str, int, str, int]] = []     # (number, ep, label, rk)
        main_scroll = 0

        # ------------------------------------------------------------------
        # Helper renderers (kept local so they close over the right state)
        # ------------------------------------------------------------------
        def render_main_screen():
            h = max(3, console.size.height - 5)
            history_panel = build_roll_history_panel(roll_history, main_scroll, h, n_digits)
            stats_panel   = build_rarity_stats_panel(roll_history)
            return Columns([history_panel, stats_panel], equal=False, expand=True)

        def display_final_screen(res):
            total_ep           = res["total_ep"]
            obtained           = res["obtained_badges"]
            overall_label      = res["overall_label"]
            overall_style      = res["overall_style"]
            rk                 = res["rk"]
            display_number_str = res["display_number_str"]

            top_bottom = top_bottom_text(rk)
            number_render = render_big_number_text(display_number_str)

            query_badges = [(name, lbl, st, ep) for _, name, _, ep, lbl, st in obtained]
            query_scroll = 0
            counts = Counter(lbl for _, lbl, _, _ in query_badges)
            counts_str = ', '.join(f"{k}:{v}" for k, v in counts.items()) if counts else ''

            while True:
                # 计算可用高度（给一点边距）
                term_h = console.size.height
                badge_h = max(5, term_h - 12)          # 右侧徽章列表可用高度

                # 左侧：大数字 + 底部信息
                left_panel = Panel(
                    Align.center(number_render),
                    border_style=overall_style,
                    box=box.HEAVY,
                )
                bottom_panel = Panel(
                    Text(
                        f"EP: {total_ep} | {overall_label} • {top_bottom} | "
                        f"Badges: {len(query_badges)} {counts_str}",
                        style=overall_style
                    ),
                    padding=(0, 1),
                )

                right = build_badge_table(query_badges, query_scroll, badge_h)

                root = Layout()
                root.split_column(
                    Layout(left_panel, size=9),
                    Layout(bottom_panel, size=3),
                    Layout(right, ratio=1),
                )

                live.update(root)

                key = readchar.readkey()
                if key == readchar.key.UP:
                    query_scroll = max(0, query_scroll - 1)
                elif key == readchar.key.DOWN:
                    query_scroll = min(max(0, len(query_badges) - 1), query_scroll + 1)
                elif key == readchar.key.PAGE_UP:
                    query_scroll = max(0, query_scroll - (term_h // 2))
                elif key == readchar.key.PAGE_DOWN:
                    query_scroll = min(max(0, len(query_badges) - 1),
                                    query_scroll + (term_h // 2))
                elif key in ('\r', '\n', 'q'):
                    break

        def do_draw_animation():
            badges_history.clear()
            scroll = 0
            xid = random.randint(0, 10 ** n_digits - 1)
            s = str(xid).rjust(n_digits, '0')

            full_skip = False
            kw.start_once()

            # ---------- 滚动数字阶段 ----------
            for pos in range(n_digits):
                skip_flag = False
                for _ in range(10 if pos < n_digits - 1 else 15):
                    if kw.pop() is not None:
                        skip_flag = True
                        full_skip = True
                        break

                    display_number = render_big_number_text(s, (pos, str(random.randint(0, 9))))
                    left_panel = Panel(
                        Align.center(display_number),
                        border_style='white',
                        box=box.HEAVY,
                    )

                    root = Layout()
                    root.split_column(
                        Layout(left_panel, ratio=1),   # 全屏只显示大数字
                    )
                    live.update(root)
                    time.sleep(0.1)

                if skip_flag:
                    break

            # ---------- 计算结果 ----------
            res = evaluate_roll(s, n_digits=n_digits, flg=True)

            display_str   = res["display_number_str"]
            total_ep      = res["total_ep"]
            obtained      = res["obtained_badges"]
            overall_label = res["overall_label"]
            rk            = res["rk"]

            # ---------- 徽章揭示动画 ----------
            if full_skip:
                for _, rname, _, rep, rlbl, rst in obtained:
                    badges_history.insert(0, (rname, rlbl, rst, rep))
            else:
                current_ep = 0
                for j, (idx, name, poss, ep, lbl, st) in enumerate(obtained):
                    if kw.pop() is not None:
                        # 一次性把剩余徽章全部加入
                        for _, rname, _, rep, rlbl, rst in obtained[j:]:
                            badges_history.insert(0, (rname, rlbl, rst, rep))
                        break

                    badges_history.insert(0, (name, lbl, st, ep))
                    current_ep += ep

                    term_h = console.size.height
                    badge_h = max(5, term_h - 12)

                    left_panel = Panel(
                        Align.center(render_big_number_text(display_str)),
                        border_style='white',
                        box=box.HEAVY,
                    )
                    bottom_panel = Panel(
                        Text(f"EP: {current_ep} | ... calculating rarity ...", style='white'),
                        padding=(0, 1),
                    )
                    right = build_badge_table(badges_history, scroll, badge_h)

                    root = Layout()
                    root.split_column(
                        Layout(left_panel, size=9),
                        Layout(bottom_panel, size=3),
                        Layout(right, ratio=1),
                    )
                    live.update(root)
                    time.sleep(0.3)

            # 写入历史并进入最终结果页
            roll_history.insert(0, (display_str, total_ep, overall_label, rk))
            display_final_screen(res)

        def show_badge_list():
            tbl = Table(title='Badge List', show_header=True)
            tbl.add_column('ID', width=4)
            tbl.add_column('Name')
            tbl.add_column('Desc')
            for i, b in enumerate(BADGES[1:], start=1):
                tbl.add_row(str(i), b.name, b.desc)
            live.update(Group(tbl, Text('\n(press any key to return)', style='dim')))
            readchar.readkey()

        def export_csv():
            try:
                fname = f"rngdle_rolls_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
                with open(fname, 'w', newline='') as f:
                    writer = csv.writer(f)
                    writer.writerow(['number', 'ep', 'rarity', 'rank'])
                    for num, ep, lbl, rk in roll_history:
                        writer.writerow([num, ep, lbl, rk])
                msg = Text(f'Exported {len(roll_history)} rows to {fname}', style='bold green')
            except Exception as ex:
                msg = Text(f'Failed to export: {ex}', style='bold red')
            live.update(Align.center(msg, vertical='middle'))
            time.sleep(0.8)

        def inquire_number():
            # 1. 先停掉 Live 的刷新，并退出 alternate screen
            live.stop()                    # 关键！停止刷新
            console.show_cursor(True)      # 确保光标可见

            try:
                # 恢复原始终端属性（echo 等）
                if is_tty and orig_attrs is not None:
                    termios.tcsetattr(fd, termios.TCSADRAIN, orig_attrs)

                console.print()            # 换行，避免和上一个画面粘在一起
                console.print(
                    "[bold cyan]Query mode[/] - Enter Inquire Number (Enter with null to cancel):"
                )
                raw = input("> ").strip()
            except Exception:
                raw = ""
            finally:
                # 2. 输入结束后重新启动 Live
                live.start()
                console.show_cursor(False)

            if not raw:
                return

            num_str = ''.join(c for c in raw if c.isdigit())
            if not num_str:
                live.update(Text.from_markup("[red]Invalid Input[/]"))
                time.sleep(0.8)
                return

            query_str = num_str[-n_digits:].rjust(n_digits, '0')
            res = evaluate_roll(query_str, n_digits=n_digits, flg=True)
            display_final_screen(res)

        # ------------------------------------------------------------------
        # Single Live context for the whole session
        # ------------------------------------------------------------------
        header = Text(
            "MasonXu RNGDLE - [Enter] [q] [l] [n] [c]  "
            "(Up/Down/PageUp/PageDown to scroll)",
            style='bold'
        )
        footer = Text.from_markup(
            'Press [bright_white on grey23]Enter[/] to draw, '
            '[bright_white on grey23]l[/] to list, '
            '[bright_white on grey23]q[/] to quit, '
            '[bright_white on grey23]e[/] to export CSV, '
            '[bright_white on grey23]n[/] to fast roll, '
            '[bright_white on grey23]c[/] to inquire a number',
            style='bold'
        )

        with Live(render_main_screen(),
                  console=console,
                  screen=True,
                  refresh_per_second=20) as live:

            while True:
                # always refresh the main view
                live.update(Group(header, render_main_screen(), footer))

                k = readchar.readkey()

                # scrolling
                if k == readchar.key.UP:
                    main_scroll = max(0, main_scroll - 1)
                elif k == readchar.key.DOWN:
                    main_scroll = min(max(0, len(roll_history) - 1), main_scroll + 1)
                elif k == readchar.key.PAGE_UP:
                    main_scroll = max(0, main_scroll - (console.size.height // 2))
                elif k == readchar.key.PAGE_DOWN:
                    main_scroll = min(max(0, len(roll_history) - 1),
                                      main_scroll + (console.size.height // 2))

                # quit
                elif k == 'q':
                    live.update(Text('bye', style='bold'))
                    time.sleep(0.3)
                    break

                # fast roll
                elif k == 'n':
                    xid = random.randint(0, 10 ** n_digits - 1)
                    s = str(xid).rjust(n_digits, '0')
                    res = evaluate_roll(s, n_digits=n_digits, flg=False)
                    roll_history.insert(0, (
                        res["display_number_str"],
                        res["total_ep"],
                        res["overall_label"],
                        res["rk"]
                    ))

                # export
                elif k == 'e':
                    export_csv()

                # badge list
                elif k == 'l':
                    show_badge_list()

                # inquire
                elif k == 'c':
                    inquire_number()

                # normal draw
                elif k in ('\r', '\n'):
                    do_draw_animation()

    finally:
        try:
            if is_tty and orig_attrs is not None:
                termios.tcsetattr(fd, termios.TCSADRAIN, orig_attrs)
        except Exception:
            pass
        try:
            console.show_cursor(True)
        except Exception:
            pass
        try:
            print()
        except Exception:
            pass


if __name__ == '__main__':
    main()