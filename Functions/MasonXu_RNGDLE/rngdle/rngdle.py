from typing import Optional, List, Tuple

from .badges import BADGES, have_badge

# ---------------------------------------------------------------------------
# Rarity / coloring logic (fully modular)
# ---------------------------------------------------------------------------

# Thresholds used by individual badge rarity (poss → label)
RARITY_THRESHOLDS = [
    (3e7,  'COMMON',    'green'),
    (3e6,  'UNUSUAL',   'yellow'),
    (3e5,  'RARE',      'blue'),
    (3e4,  'EPIC',      'magenta'),
    (3e3,  'LEGENDARY', 'red'),
    (300,  'MYTHIC',    'cyan'),
    (30,   'ULTRA',     'bright_red'),
]

# Overall roll rarity derived from rank (rk)
OVERALL_RARITY_RULES = [
    (1,   'WORST TRASH', 'yellow'),
    (10,  'TRASH',       'yellow'),
    (500, 'COMMON',      'green'),
    (750, 'UNUSUAL',     'bright_yellow'),
    (900, 'RARE',        'blue'),
    (950, 'PURPLE',      'magenta'),
    (975, 'LEGENDARY',   'red'),
    (990, 'MYTHIC',      'cyan'),
    (995, 'ULTRA',       'bright_red'),
    # else → SUPER
]

# Canonical order + style map for stats table
RARITY_ORDER = [
    'WORST TRASH', 'TRASH', 'COMMON', 'UNUSUAL', 'RARE',
    'PURPLE', 'LEGENDARY', 'MYTHIC', 'ULTRA', 'SUPER'
]

STYLE_MAP = {
    'WORST TRASH': 'yellow',
    'TRASH':       'yellow',
    'COMMON':      'green',
    'UNUSUAL':     'bright_yellow',
    'RARE':        'blue',
    'PURPLE':      'magenta',
    'LEGENDARY':   'red',
    'MYTHIC':      'cyan',
    'ULTRA':       'bright_red',
    'SUPER':       'bright_green',
    'EPIC':        'magenta',   # used by badge-level rarity
}


def rarity_for_poss(poss: Optional[int]) -> Tuple[str, str]:
    """Badge-level rarity from possibility count."""
    if poss is None:
        return 'UNKNOWN', 'white'
    for thresh, label, style in RARITY_THRESHOLDS:
        if poss > thresh:
            return label, style
    return 'SUPER', 'bright_green'


def overall_rarity_from_rk(rk: int) -> Tuple[str, str]:
    """Overall roll rarity + style from rank index."""
    for upper, label, style in OVERALL_RARITY_RULES:
        if rk < upper:
            return label, style
    return 'SUPER', 'bright_green'


def label_to_style(label: str) -> str:
    """Lookup Rich style for a rarity label."""
    return STYLE_MAP.get(label, 'white')


def top_bottom_text(rk: int) -> str:
    if rk >= 500:
        return f"TOP {(1000 - rk) / 10.0:.1f}%"
    return f"BOTTOM {(rk + 1) / 10.0:.1f}%"


# ---------------------------------------------------------------------------
# Rank lookup (ported from original.cpp)
# ---------------------------------------------------------------------------
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
    """Highest i (1..999) where sum_points >= RANKS[i]."""
    rk = 0
    for i in range(1, min(1000, len(RANKS))):
        if sum_points >= RANKS[i]:
            rk = i
    return rk


def trim_leading_zeros(s: str) -> str:
    n = len(s)
    start = 0
    while start < n and s[start] in ('0', ' '):
        start += 1
    if start >= n:
        return '0'
    return s[start:]

def evaluate_roll(final_number_str: str, n_digits: int = 9, flg: bool = True) -> dict:
    """
    Evaluate a final roll number string and compute badges, EP, rank, and overall rarity.

    Returns dict with keys:
      - display_number_str, total_ep, rk, overall_label, overall_style, obtained_badges
    """
    s = final_number_str
    check_num_str = s[-n_digits:].rjust(n_digits, '0')
    total_ep = 0
    obtained = []

    # ALWAYS compute total_ep by scanning badges; only append details when flg is True
    for i in range(1, len(BADGES)):
        try:
            check_num = int(check_num_str)
            if have_badge(check_num, i):
                poss = BADGES[i].poss
                ep = int(100000 / (poss ** 0.5)) if poss > 0 else 0
                total_ep += ep
                if flg:
                    lbl, st = rarity_for_poss(poss)
                    obtained.append((i, BADGES[i].name, poss, ep, lbl, st))
        except Exception:
            continue

    rk = ranks_lookup(total_ep)
    overall_label, overall_style = overall_rarity_from_rk(rk)
    display_number_str = trim_leading_zeros(final_number_str)

    return {
        "display_number_str": display_number_str,
        "total_ep": total_ep,
        "rk": rk,
        "overall_label": overall_label,
        "overall_style": overall_style,
        "obtained_badges": obtained,
    }

