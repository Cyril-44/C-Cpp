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

import readchar
from rich.console import Console, Group
from rich.panel import Panel
from rich.live import Live
from rich.align import Align
from rich.table import Table
from rich.text import Text
from rich.columns import Columns
from rich import box
from .rngdle import *

console = Console()

# ---------------------------------------------------------------------------
# Block digit patterns (5x5)
# ---------------------------------------------------------------------------
BLOCK = '█'
SPACE = ' '
DIGITS = {
    '0': [' ███ ', '█   █', '█   █', '█   █', ' ███ '],
    '1': ['  █  ', ' ██  ', '  █  ', '  █  ', ' ███ '],
    '2': [' ███ ', '█   █', '  ██ ', ' █   ', '█████'],
    '3': [' ███ ', '█   █', '  ██ ', '█   █', ' ███ '],
    '4': ['█  █ ', '█  █ ', '█████', '   █ ', '   █ '],
    '5': ['█████', '█    ', '████ ', '    █', '████ '],
    '6': [' ███ ', '█    ', '████ ', '█   █', ' ███ '],
    '7': ['█████', '   █ ', '  █  ', ' █   ', '█    '],
    '8': [' ███ ', '█   █', ' ███ ', '█   █', ' ███ '],
    '9': [' ███ ', '█   █', ' ████', '    █', ' ███ '],
    ' ': ['     ', '     ', '     ', '     ', '     '],
}



# ---------------------------------------------------------------------------
# Rendering helpers
# ---------------------------------------------------------------------------
def render_big_number_text(
    s: str,
    revealed_mask: List[bool],
    confirmed_style: str = 'bold white',
    rolling_style: str = 'grey50',
) -> Text:
    rows = []
    for row in range(5):
        line = Text('')
        for idx, ch in enumerate(s):
            pat = DIGITS.get(ch, DIGITS[' '])[row]
            style = confirmed_style if (idx < len(revealed_mask) and revealed_mask[idx]) else rolling_style
            line.append(pat + ' ', style=style)
        rows.append(line)

    out = Text('')
    for i, rt in enumerate(rows):
        out.append(rt)
        if i != 4:
            out.append('\n')
    return out


def build_badge_table(badges_history: List[Tuple[str, str, str, int]], start: int, height: int) -> Table:
    tbl = Table.grid(padding=(0, 1))
    tbl.add_column()
    end = min(len(badges_history), start + height)
    for name, label, style, ep in badges_history[start:end]:
        tbl.add_row(Text(f"{name} [{label}] +{ep}EP", style=style))
    return tbl

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
    # +2 给滚动条
    needed = num_w + ep_w + rarity_w + rk_w + 5

    tbl = Table(
        show_header=True,
        header_style="bold",
        box=None,
        pad_edge=False,
        expand=False,
        padding=(0, 0),
    )
    tbl.add_column("Num",    width=num_w,    justify="left",   no_wrap=True)
    tbl.add_column("EP",     width=ep_w,     justify="right",  no_wrap=True)
    tbl.add_column("Rarity", width=rarity_w, justify="center", no_wrap=True)
    tbl.add_column("rk",     width=rk_w,     justify="right",  no_wrap=True)
    tbl.add_column("",       width=1,        justify="center")  # 滚动条列

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
        width=needed,
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
        expand=False,
        padding=(0, 1),
    )
    tbl.add_column("Rarity", min_width=12, no_wrap=True)
    tbl.add_column("Count",  width=6,      justify="right", no_wrap=True)
    tbl.add_column("Prob",   width=8,      justify="right", no_wrap=True)

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
        console.clear()
        console.rule("MasonXu RNGDLE - Enter: draw, q: quit, l: list badges")
        kw = KeyWatcher()
        badges_history: List[Tuple[str, str, str, int]] = []   # newest first
        roll_history: List[Tuple[str, int, str, int]] = []     # (number, ep, label, rk)
        main_scroll = 0
        scroll = 0

        while True:
            # ---- main page: Roll History (left) + Rarity Stats (right) ----
            h = max(3, console.size.height - 8)

            history_panel = build_roll_history_panel(roll_history, main_scroll, h, n_digits)
            stats_panel   = build_rarity_stats_panel(roll_history)

            # 左边刚好放下，右边自然撑开，Columns 自动分配剩余空间
            console.print(Columns([history_panel, stats_panel], equal=False, expand=True))

            console.clear()
            console.rule("MasonXu RNGDLE - Enter: draw, q: quit, l: list badges | e: export CSV")
            console.print(Columns([history_panel, stats_panel], equal=False, expand=True))
            console.print(
                '\nPress Enter to draw, l to list, q to quit, e to export CSV, n to fast roll'
                '    (Up/Down/PageUp/PageDown to scroll history)',
                style='bold'
            )

            k = readchar.readkey()

            # scrolling
            if k == readchar.key.UP:
                main_scroll = max(0, main_scroll - 1)
                continue
            if k == readchar.key.DOWN:
                main_scroll = min(max(0, len(roll_history) - 1), main_scroll + 1)
                continue
            if k == readchar.key.PAGE_UP:
                main_scroll = max(0, main_scroll - (console.size.height // 2))
                continue
            if k == readchar.key.PAGE_DOWN:
                main_scroll = min(max(0, len(roll_history) - 1),
                                  main_scroll + (console.size.height // 2))
                continue

            if k == 'q':
                console.print('bye')
                return
            
            if k == 'n':
                # 快速抽取：跳过所有动画和 Badge 显示，直接写入 Roll History
                xid = random.randint(0, 10 ** n_digits - 1)
                s = str(xid).rjust(n_digits, '0')

                # 复用 evaluate_roll，flg=False 表示不计算/显示 badge 列表（快速模式）
                res = evaluate_roll(s, n_digits=n_digits, flg=False)

                roll_history.insert(0, (res["display_number_str"], res["total_ep"], res["overall_label"], res["rk"]))
                continue



            if k == 'e':
                try:
                    fname = f"rngdle_rolls_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
                    with open(fname, 'w', newline='') as f:
                        writer = csv.writer(f)
                        writer.writerow(['number', 'ep', 'rarity', 'rank'])
                        for num, ep, lbl, rk in roll_history:
                            writer.writerow([num, ep, lbl, rk])
                    console.print(f'Exported {len(roll_history)} rows to {fname}', style='bold green')
                except Exception as ex:
                    console.print(f'Failed to export: {ex}', style='bold red')
                time.sleep(0.8)
                continue

            if k == 'l':
                tbl = Table(title='Badge List', show_header=True)
                tbl.add_column('ID', width=4)
                tbl.add_column('Name')
                tbl.add_column('Desc')
                for i, b in enumerate(BADGES[1:], start=1):
                    tbl.add_row(str(i), b.name, b.desc)
                console.print(tbl)
                console.print('\n(press any key)')
                readchar.readkey()
                continue

            # ---- draw ----
            badges_history.clear()
            scroll = 0
            xid = random.randint(0, 10 ** n_digits - 1)
            s = str(xid).rjust(n_digits, '0')
            revealed = [' '] * n_digits

            with Live(console=console, screen=True, refresh_per_second=20) as live:
                full_skip = False
                for pos in range(n_digits):
                    kw.start_once()
                    skip_flag = False
                    for _ in range(10):
                        if kw.pop() is not None:
                            skip_flag = True
                            full_skip = True
                            break
                        disp_list = list(revealed)
                        disp_list[pos] = str(random.randint(0, 9))
                        disp = ''.join(disp_list)
                        revealed_mask = [ch != ' ' for ch in revealed]
                        display_str, display_mask = trim_leading_zeros(disp, revealed_mask)
                        number_render = render_big_number_text(display_str, display_mask)
                        left_panel = Panel(Align.center(number_render), border_style='white')
                        bottom_panel = Panel(Text('EP: 0 | UNKNOWN', style='white'), padding=(0, 1))
                        left_group = Group(left_panel, bottom_panel)
                        right = build_badge_table(badges_history, scroll, console.size.height - 6)
                        live.update(Columns([left_group, right], equal=True, expand=True))
                        time.sleep(0.06)
                    if skip_flag:
                        revealed[pos:] = list(s[pos:])
                        break
                    revealed[pos] = s[pos]

                # final number
                final_disp = ''.join(revealed)
                revealed_mask = [ch != ' ' for ch in revealed]
                display_str, display_mask = trim_leading_zeros(final_disp, revealed_mask)
                number_render = render_big_number_text(display_str, display_mask)

                # Evaluate roll using the new helper.
                # 如果你在 main loop 中维护了一个开关 compute_badges（默认 True），这里传入它；
                # 否则直接传 True 来保留原有行为。
                # 例如： compute_badges = True  # 在 main() 初始化处定义
                res = evaluate_roll(final_disp, n_digits=n_digits, flg=True)

                total_ep = res["total_ep"]
                obtained = res["obtained_badges"]
                overall_label = res["overall_label"]
                overall_style = res["overall_style"]
                rk = res["rk"]

                # badge reveal animation (复用原有逻辑)
                if full_skip:
                    for _, rname, _, rep, rlbl, rst in obtained:
                        badges_history.insert(0, (rname, rlbl, rst, rep))
                else:
                    for j, (idx, name, poss, ep, lbl, st) in enumerate(obtained):
                        kw.start_once()
                        if kw.pop() is not None:
                            for _, rname, _, rep, rlbl, rst in obtained[j:]:
                                badges_history.insert(0, (rname, rlbl, rst, rep))
                            break
                        badges_history.insert(0, (name, lbl, st, ep))
                        left_panel = Panel(Align.center(number_render), border_style='white')
                        bottom_panel = Panel(
                            Text(f"EP: {total_ep} | ... calculating rarity ...", style='white'),
                            padding=(0, 1)
                        )
                        left_group = Group(left_panel, bottom_panel)
                        right = build_badge_table(badges_history, scroll, console.size.height - 6)
                        live.update(Columns([left_group, right], equal=True, expand=True))
                        time.sleep(0.12)

                # final overall rarity (已经由 evaluate_roll 计算，但保留原有变量)
                top_bottom = top_bottom_text(rk)

                # store in history (trimmed number)
                number_str = ''.join(revealed)
                display_number_str, _ = trim_leading_zeros(
                    number_str, [ch != ' ' for ch in number_str]
                )
                roll_history.insert(0, (display_number_str, total_ep, overall_label, rk))

                counts = Counter(lbl for _, lbl, _, _ in badges_history)
                counts_str = ', '.join(f"{k}:{v}" for k, v in counts.items()) if counts else ''

                # interactive result view (保持不变)
                while True:
                    left_panel = Panel(
                        Align.center(number_render),
                        border_style=overall_style,
                        box=box.HEAVY
                    )
                    bottom_panel = Panel(
                        Text(
                            f"EP: {total_ep} | {overall_label} • {top_bottom} | "
                            f"Badges: {len(badges_history)} {counts_str}",
                            style=overall_style
                        ),
                        padding=(0, 1)
                    )
                    left_group = Group(left_panel, bottom_panel)
                    right = build_badge_table(badges_history, scroll, console.size.height - 6)
                    live.update(Columns([left_group, right], equal=True, expand=True))

                    key = readchar.readkey()
                    if key == readchar.key.UP:
                        scroll = max(0, scroll - 1)
                    elif key == readchar.key.DOWN:
                        scroll = min(max(0, len(badges_history) - 1), scroll + 1)
                    elif key == readchar.key.PAGE_UP:
                        scroll = max(0, scroll - (console.size.height // 2))
                    elif key == readchar.key.PAGE_DOWN:
                        scroll = min(max(0, len(badges_history) - 1),
                                    scroll + (console.size.height // 2))
                    elif key == 'q':
                        return
                    elif key in ('\r', '\n'):
                        break


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