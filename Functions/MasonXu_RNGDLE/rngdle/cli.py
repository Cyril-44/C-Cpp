# Rich-based TUI with Unicode block digits, rarity label and scrollable badge list
import random
import time
import threading
from typing import Optional, List, Tuple

import sys
import termios
import tty
import select
from collections import Counter
from rich.console import Console, Group
from rich.panel import Panel
from rich.live import Live
from rich.align import Align
from rich.table import Table
from rich.text import Text
from rich.columns import Columns
from rich.padding import Padding
from rich import box

from .badges import BADGES
from .utils import have_badge

console = Console()

# 5x5 solid block digit patterns (monospace safe)
BLOCK = '█'
SPACE = ' '
DIGITS = {
    '0': [
        ' ███ ',
        '█   █',
        '█   █',
        '█   █',
        ' ███ '
    ],
    '1': [
        '  █  ',
        ' ██  ',
        '  █  ',
        '  █  ',
        ' ███ '
    ],
    '2': [
        ' ███ ',
        '█   █',
        '  ██ ',
        ' █   ',
        '█████'
    ],
    '3': [
        ' ███ ',
        '█   █',
        '  ██ ',
        '█   █',
        ' ███ '
    ],
    '4': [
        '█  █ ',
        '█  █ ',
        '█████',
        '   █ ',
        '   █ '
    ],
    '5': [
        '█████',
        '█    ',
        '████ ',
        '    █',
        '████ '
    ],
    '6': [
        ' ███ ',
        '█    ',
        '████ ',
        '█   █',
        ' ███ '
    ],
    '7': [
        '█████',
        '   █ ',
        '  █  ',
        ' █   ',
        '█    '
    ],
    '8': [
        ' ███ ',
        '█   █',
        ' ███ ',
        '█   █',
        ' ███ '
    ],
    '9': [
        ' ███ ',
        '█   █',
        ' ████',
        '    █',
        ' ███ '
    ],
    ' ': [
        '     ',
        '     ',
        '     ',
        '     ',
        '     '
    ]
}

RARITY_LABELS = [
    (3e7, 'COMMON', 'green'),
    (3e6, 'UNUSUAL', 'yellow'),
    (3e5, 'RARE', 'blue'),
    (3e4, 'EPIC', 'magenta'),
    (3e3, 'LEGENDARY', 'red'),
    (300, 'MYTHIC', 'cyan'),
    (30, 'ULTRA', 'bright_red'),
]


def rarity_for_poss(poss: Optional[int]) -> Tuple[str, str]:
    if poss is None:
        return ('UNKNOWN', 'white')
    for thresh, label, style in RARITY_LABELS:
        if poss > thresh:
            return (label, style)
    return ('SUPER', 'bright_green')


import readchar

class KeyWatcher:
    def __init__(self):
        self.lock = threading.Lock()
        self.key = None
        self.thread = None

    def start_once(self):
        # spawn a reader thread that captures a single keypress
        if self.thread and self.thread.is_alive():
            return
        self.key = None
        self.thread = threading.Thread(target=self._read)
        self.thread.daemon = True
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


def render_big_number_lines(s: str) -> List[str]:
    # s is n-char string; render each digit from DIGITS horizontally (keeps original behavior)
    rows = ['' for _ in range(5)]
    for ch in s:
        pat = DIGITS.get(ch, DIGITS[' '])
        for i in range(5):
            rows[i] += pat[i] + ' '  # spacing
    return rows


def render_big_number_text(s: str, revealed_mask: List[bool], confirmed_style: str = 'bold white', rolling_style: str = 'grey50') -> Text:
    # Build a rich.Text where confirmed digits use confirmed_style and unrevealed/rolling use rolling_style
    # s: string of digits length n
    rows = ['' for _ in range(5)]
    n = len(s)
    for row in range(5):
        line_text = Text('')
        for idx, ch in enumerate(s):
            pat = DIGITS.get(ch, DIGITS[' '])[row]
            style = confirmed_style if (idx < len(revealed_mask) and revealed_mask[idx]) else rolling_style
            line_text.append(pat + ' ', style=style)
        rows[row] = line_text
    # join rows with newlines into a single Text
    out = Text('')
    for i, rt in enumerate(rows):
        out.append(rt)
        if i != 4:
            out.append('\n')
    return out


def _trim_leading_zeros_and_spaces(s: str, revealed_mask: List[bool]) -> Tuple[str, List[bool]]:
    """Trim leading zeros and spaces for display. Returns (display_str, display_mask).
    If all characters are zeros or spaces, returns ('0', [True]) to show a single zero.
    """
    n = len(s)
    start = 0
    while start < n and (s[start] == '0' or s[start] == ' '):
        start += 1
    if start >= n:
        # all zeros or spaces
        # prefer to show a single '0'
        return '0', [True]
    disp = s[start:]
    mask = revealed_mask[start:]
    return disp, mask


def build_badge_table(badges_history: List[Tuple[str,str,str,int]], start: int, height: int) -> Table:
    tbl = Table.grid(padding=(0,1))
    tbl.add_column()
    end = min(len(badges_history), start + height)
    for name, label, style, ep in badges_history[start:end]:
        txt = Text(f"{name} [{label}] +{ep}EP", style=style)
        tbl.add_row(txt)
    return tbl


def percent_from_points(total_points: int) -> float:
    max_total = 0
    for b in BADGES[1:]:
        if b.poss > 0:
            max_total += int(100000 / (b.poss ** 0.5))
    if max_total == 0:
        return 0.0
    pct = max(0.0, min(100.0, 100.0 * (total_points / max_total)))
    return pct


# RANKS array ported from original.cpp (1-based indexing; pad index 0)
RANKS = [0,
65,66,67,69,71,72,72,73,74,74,75,75,75,75,76,76,77,77,78,79,
79,80,80,80,81,81,81,81,82,82,82,82,82,83,83,83,83,83,83,84,
84,84,84,85,85,85,85,85,85,86,86,87,87,87,87,87,88,88,88,88,
89,89,89,89,89,90,90,90,90,90,90,90,90,91,91,91,91,91,91,92,
92,92,92,92,92,92,93,93,93,93,93,93,93,93,94,94,94,94,94,95,
95,95,95,95,95,95,96,96,96,96,96,97,97,97,97,97,97,97,98,98,
98,98,98,98,98,99,99,99,99,99,99,99,100,100,100,100,100,100,100,100,
100,100,101,101,101,101,101,101,101,102,102,102,102,102,102,102,102,103,103,103,
103,103,103,103,103,104,104,104,104,104,104,104,105,105,105,105,105,105,105,105,
106,106,106,106,106,106,106,107,107,107,107,107,107,107,107,107,107,108,108,108,
108,108,108,108,108,109,109,109,109,109,109,109,109,109,110,110,110,110,110,110,
110,110,110,110,111,111,111,111,111,111,111,111,111,112,112,112,112,112,112,112,
112,112,113,113,113,113,113,113,113,113,113,114,114,114,114,114,114,114,114,115,
115,115,115,115,115,115,115,115,115,116,116,116,116,116,116,116,116,116,117,117,
117,117,117,117,117,117,117,117,117,118,118,118,118,118,118,118,118,118,119,119,
119,119,119,119,119,119,119,120,120,120,120,120,120,120,120,120,120,121,121,121,
121,121,121,121,121,121,122,122,122,122,122,122,122,122,122,123,123,123,123,123,
123,123,123,123,124,124,124,124,124,124,124,124,124,124,125,125,125,125,125,125,
125,125,125,126,126,126,126,126,126,126,126,126,127,127,127,127,127,127,127,127,
127,127,128,128,128,128,128,128,128,128,128,129,129,129,129,129,129,129,129,129,
130,130,130,130,130,130,130,130,130,131,131,131,131,131,131,131,131,131,132,132,
132,132,132,132,132,132,133,133,133,133,133,133,133,133,133,134,134,134,134,134,
134,134,134,135,135,135,135,135,135,135,135,135,136,136,136,136,136,136,136,136,
136,137,137,137,137,137,137,137,137,137,138,138,138,138,138,138,138,138,139,139,
139,139,139,139,139,140,140,140,140,140,140,140,140,141,141,141,141,141,141,141,141,
142,142,142,142,142,142,142,142,143,143,143,143,143,143,143,143,144,144,144,144,
144,144,144,144,145,145,145,145,145,145,145,145,146,146,146,146,146,146,146,146,
147,147,147,147,147,147,147,147,148,148,148,148,148,148,148,149,149,149,149,149,
149,149,150,150,150,150,150,150,150,150,151,151,151,151,151,151,151,152,152,152,152,
152,152,152,152,153,153,153,153,153,153,153,154,154,154,154,154,154,154,155,155,155,
155,155,155,155,156,156,156,156,156,156,156,157,157,157,157,157,157,157,158,158,158,
158,158,158,159,159,159,159,159,159,159,160,160,160,160,160,160,161,161,161,161,161,
161,162,162,162,162,162,162,163,163,163,163,163,163,163,164,164,164,164,164,164,165,
165,165,165,165,165,166,166,166,166,166,166,167,167,167,167,167,168,168,168,168,168,
168,169,169,169,169,169,170,170,170,170,170,171,171,171,171,171,172,172,172,172,172,
173,173,173,173,173,173,174,174,174,174,174,175,175,175,175,175,176,176,176,176,176,
177,177,177,177,177,178,178,178,178,179,179,179,179,179,180,180,180,180,180,181,181,
181,181,182,182,182,182,183,183,183,183,183,184,184,184,184,185,185,185,185,186,186,
186,186,187,187,187,187,188,188,188,188,189,189,189,189,190,190,190,190,191,191,191,
192,192,192,192,193,193,193,193,194,194,194,195,195,195,196,196,196,196,197,197,197,
198,198,198,199,199,199,200,200,200,200,201,201,201,202,202,202,203,203,203,204,204,204,
205,205,206,206,206,207,207,207,208,208,208,209,209,210,210,210,211,211,212,212,212,
213,213,214,214,215,215,215,216,216,217,217,218,218,219,219,220,220,220,221,222,222,223,
223,224,224,225,225,226,226,227,227,228,228,229,230,230,231,231,232,233,233,234,235,235,
236,237,237,238,239,239,240,241,242,242,243,244,245,246,246,247,248,249,250,251,252,253,
254,255,255,256,258,259,260,261,262,263,264,265,267,268,269,270,272,273,275,276,278,279,
281,283,284,286,288,290,292,294,296,298,300,303,305,307,310,313,315,318,321,324,328,331,
335,339,343,347,352,357,361,367,372,378,384,391,398,406,414,423,432,443,455,469,484,501,
523,549,583,631,705,814,1011]


def ranks_lookup(sum_points: int) -> int:
    """Replicate original.cpp rk computation: find highest i (1..999) where sum >= RANKS[i]."""
    rk = 0
    for i in range(1, min(1000, len(RANKS))):
        if sum_points >= RANKS[i]:
            rk = i
    return rk


def main():
    # parse optional --digits / -n argument
    n_digits = 9
    try:
        import argparse
        parser = argparse.ArgumentParser(add_help=False)
        parser.add_argument('-n','--digits', type=int, default=9)
        args, _ = parser.parse_known_args()
        n_digits = max(1, args.digits)
    except Exception:
        n_digits = 9

    fd = None
    orig_attrs = None
    is_tty = False
    try:
        fd = sys.stdin.fileno()
        is_tty = sys.stdin.isatty()
        if is_tty:
            orig_attrs = termios.tcgetattr(fd)
    except Exception:
        fd = None
        orig_attrs = None
        is_tty = False

    try:
        console.clear()
        console.rule("MasonXu RNGDLE - Enter: draw, q: quit, l: list badges")
        kw = KeyWatcher()
        badges_history: List[Tuple[str,str,str,int]] = []  # newest first
        roll_history: List[Tuple[str,int,str,int]] = []  # (number, ep, label, rk) newest first
        main_scroll = 0
        scroll = 0

        def label_to_style(label: str) -> str:
            mapping = {
                'WORST TRASH': 'yellow',
                'TRASH': 'yellow',
                'COMMON': 'green',
                'UNUSUAL': 'bright_yellow',
                'RARE': 'blue',
                'PURPLE': 'magenta',
                'LEGENDARY': 'red',
                'MYTHIC': 'cyan',
                'ULTRA': 'bright_red',
                'SUPER': 'bright_green'
            }
            return mapping.get(label, 'white')

        def build_roll_history_panel(history, start, height, width):
            # Aligned tabular history: Number | EP | Rarity | Rank
            tbl = Table(show_header=True, header_style='bold', box=None, pad_edge=False)
            tbl.add_column('Number', justify='left', no_wrap=True)
            tbl.add_column('EP', justify='right', width=8)
            tbl.add_column('Rarity', justify='center', width=14)
            tbl.add_column('rk', justify='right', width=6)
            end = min(len(history), start + height)
            for num, ep, label, rk in history[start:end]:
                style = label_to_style(label)
                tbl.add_row(num, str(ep), Text(label, style=style), str(rk))
            panel = Panel(tbl, title=f'Roll History ({len(history)})', padding=(0,1), width=width)
            return panel

        while True:
            # render main page with roll history occupying most of the terminal
            h = max(3, console.size.height - 6)
            history_panel = build_roll_history_panel(roll_history, main_scroll, h, console.size.width)
            console.clear()
            console.rule("MasonXu RNGDLE - Enter: draw, q: quit, l: list badges")
            console.print(history_panel)
            console.print('\nPress Enter to draw, l to list, q to quit    (Use Up/Down/PageUp/PageDown to scroll history)', style='bold')

            k = readchar.readkey()
            # support scrolling on main page
            if k == readchar.key.UP:
                main_scroll = max(0, main_scroll-1)
                continue
            if k == readchar.key.DOWN:
                main_scroll = min(max(0, len(roll_history)-1), main_scroll+1)
                continue
            if k == readchar.key.PAGE_UP:
                main_scroll = max(0, main_scroll - (console.size.height//2))
                continue
            if k == readchar.key.PAGE_DOWN:
                main_scroll = min(max(0, len(roll_history)-1), main_scroll + (console.size.height//2))
                continue

            if k == 'q':
                console.print('bye')
                return
            if k == 'l':
                # full list
                tbl = Table(title='Badge List', show_header=True)
                tbl.add_column('ID', width=4)
                tbl.add_column('Name')
                tbl.add_column('Desc')
                for i,b in enumerate(BADGES[1:], start=1):
                    tbl.add_row(str(i), b.name, b.desc)
                console.print(tbl)
                console.print('\n(press any key)')
                readchar.readkey()
                continue
            # treat Enter (or other keys) as draw
            # clear previous badges for a fresh draw
            badges_history.clear()
            scroll = 0
            # generate an n_digits number (allow leading zeros)
            xid = random.randint(0, 10**n_digits - 1)
            s = str(xid).rjust(n_digits, '0')
            revealed = [' ']*n_digits

            with Live(console=console, screen=True, refresh_per_second=20) as live:
                # per-digit reveal
                full_skip = False
                for pos in range(n_digits):
                    kw.start_once()
                    skip_flag = False
                    for _ in range(10):
                        k = kw.pop()
                        if k is not None:
                            skip_flag = True
                            full_skip = True
                            break
                        # display random digit at current pos; unrevealed digits shown in rolling_style (grey)
                        disp_list = list(revealed)
                        disp_list[pos] = str(random.randint(0,9))
                        disp = ''.join(disp_list)
                        revealed_mask = [ch != ' ' for ch in revealed]
                        # hide leading zeros/spaces in display
                        display_str, display_mask = _trim_leading_zeros_and_spaces(disp, revealed_mask)
                        number_render = render_big_number_text(display_str, display_mask, confirmed_style='bold white', rolling_style='grey50')
                        # panel and rarity placeholder with bottom placeholder
                        left_panel = Panel(Align.center(number_render), border_style='white')
                        bottom_panel = Panel(Text('EP: 0 | UNKNOWN', style='white'), padding=(0,1))
                        left_group = Group(left_panel, bottom_panel)
                        right = build_badge_table(badges_history, scroll, console.size.height - 6)
                        live.update(Columns([left_group, right], equal=True, expand=True))
                        time.sleep(0.06)
                    if skip_flag:
                        # reveal remaining digits immediately and mark full skip
                        revealed[pos:] = list(s[pos:])
                        break
                    revealed[pos] = s[pos]
                # final number
                # final number: all digits revealed
                final_disp = ''.join(revealed)
                revealed_mask = [ch != ' ' for ch in revealed]
                display_str, display_mask = _trim_leading_zeros_and_spaces(final_disp, revealed_mask)
                number_render = render_big_number_text(display_str, display_mask, confirmed_style='bold white', rolling_style='grey50')

                # compute obtained badges and total EP
                obtained = []
                total_ep = 0
                for i in range(1, len(BADGES)):
                    try:
                        # prepare a 9-digit number for badge checks: take rightmost 9 digits and pad left with zeros
                        check_num_str = ''.join(revealed)[-9:].rjust(9, '0')
                        check_num = int(check_num_str)
                        if have_badge(check_num, i):
                            poss = BADGES[i].poss
                            ep = int(100000 / (poss ** 0.5)) if poss>0 else 0
                            lbl, st = rarity_for_poss(poss)
                            obtained.append((i, BADGES[i].name, poss, ep, lbl, st))
                            total_ep += ep
                    except Exception:
                        continue

                # during badge animation show neutral border and accumulating EP but do not recolor yet
                # insert badges with animation (head insert) and support skip
                skip_remaining = False
                if full_skip:
                    # user requested full skip during reveal: insert all obtained at once
                    for rem in obtained:
                        _, rname, rposs, rep, rlbl, rst = rem
                        badges_history.insert(0, (rname, rlbl, rst, rep))
                else:
                    for j, (idx, name, poss, ep, lbl, st) in enumerate(obtained):
                        # check for user skip
                        kw.start_once()
                        k = kw.pop()
                        if k is not None:
                            # user pressed: immediately insert all remaining badges
                            for rem in obtained[j:]:
                                _, rname, rposs, rep, rlbl, rst = rem
                                badges_history.insert(0, (rname, rlbl, rst, rep))
                            skip_remaining = True
                            break
                        badges_history.insert(0, (name, lbl, st, ep))
                        # neutral styling during animation
                        left_panel = Panel(Align.center(number_render), border_style='white')
                        bottom_panel = Panel(Text(f"EP: {total_ep} | ... calculating rarity ...", style='white'), padding=(0,1))
                        left_group = Group(left_panel, bottom_panel)
                        right = build_badge_table(badges_history, scroll, console.size.height - 6)
                        live.update(Columns([left_group, right], equal=True, expand=True))
                        time.sleep(0.12)
                    if skip_remaining:
                        # already inserted remaining badges, continue
                        pass

                # after animation (or immediate insert), compute final rarity and stats, then enter interactive scrolling loop
                # compute rk using ranks lookup (match original.cpp logic)
                rk = ranks_lookup(total_ep)
                # map rk to label & color same as original
                if rk < 1:
                    overall_label = 'WORST TRASH'
                    overall_style = 'yellow'
                elif rk < 10:
                    overall_label = 'TRASH'
                    overall_style = 'yellow'
                elif rk < 500:
                    overall_label = 'COMMON'
                    overall_style = 'green'
                elif rk < 750:
                    overall_label = 'UNUSUAL'
                    overall_style = 'bright_yellow'
                elif rk < 900:
                    overall_label = 'RARE'
                    overall_style = 'blue'
                elif rk < 950:
                    overall_label = 'PURPLE'
                    overall_style = 'magenta'
                elif rk < 975:
                    overall_label = 'LEGENDARY'
                    overall_style = 'red'
                elif rk < 990:
                    overall_label = 'MYTHIC'
                    overall_style = 'cyan'
                elif rk < 995:
                    overall_label = 'ULTRA'
                    overall_style = 'bright_red'
                else:
                    overall_label = 'SUPER'
                    overall_style = 'bright_green'

                if rk >= 500:
                    top_bottom = f"TOP {(1000-rk)/10.0:.1f}%"
                else:
                    top_bottom = f"BOTTOM {(rk+1)/10.0:.1f}%"

                # append roll result to roll_history (number, ep, label, rk)
                number_str = ''.join(revealed)
                # store trimmed display version for history
                display_number_str, _ = _trim_leading_zeros_and_spaces(number_str, [ch != ' ' for ch in number_str])
                try:
                    roll_history.insert(0, (display_number_str, total_ep, overall_label, rk))
                except Exception:
                    pass

                # badge counts per rarity
                counts = Counter([lbl for (_, lbl, _, _) in badges_history])
                counts_str = ', '.join(f"{k}:{v}" for k,v in counts.items()) if counts else ''

                while True:
                    left_panel = Panel(Align.center(number_render), border_style=overall_style, box=box.HEAVY)
                    bottom_panel = Panel(Text(f"EP: {total_ep} | {overall_label} • {top_bottom} | Badges: {len(badges_history)} {counts_str}", style=overall_style), padding=(0,1))
                    left_group = Group(left_panel, bottom_panel)
                    right = build_badge_table(badges_history, scroll, console.size.height - 6)
                    live.update(Columns([left_group, right], equal=True, expand=True))

                    key = readchar.readkey()
                    if key == readchar.key.UP:
                        scroll = max(0, scroll-1)
                    elif key == readchar.key.DOWN:
                        scroll = min(max(0, len(badges_history)-1), scroll+1)
                    elif key == readchar.key.PAGE_UP:
                        scroll = max(0, scroll - (console.size.height//2))
                    elif key == readchar.key.PAGE_DOWN:
                        scroll = min(max(0, len(badges_history)-1), scroll + (console.size.height//2))
                    elif key == 'q':
                        return
                    elif key == '\r' or key == '\n':
                        break

    finally:
        # restore terminal attributes to avoid leaving terminal unusable
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
            # ensure prompt on next line
            print()
        except Exception:
            pass

if __name__ == '__main__':
    main()
