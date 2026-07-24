# Small internal key reader to avoid external dependency on readchar in frozen builds
import sys, termios, tty, select

KEY_UP = '\x1b[A'
KEY_DOWN = '\x1b[B'
KEY_RIGHT = '\x1b[C'
KEY_LEFT = '\x1b[D'
KEY_PAGE_UP = '\x1b[5~'
KEY_PAGE_DOWN = '\x1b[6~'


def read_key(timeout: float = None) -> str:
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        ch = sys.stdin.read(1)
        if ch == '\x1b':
            # possible escape sequence; read more bytes non-blocking
            seq = ch
            while True:
                r, _, _ = select.select([fd], [], [], 0.02)
                if r:
                    more = sys.stdin.read(1)
                    seq += more
                    # common sequences end with '~' or letters
                    if more.isalpha() or more == '~':
                        break
                    # safety: limit length
                    if len(seq) > 6:
                        break
                else:
                    break
            return seq
        else:
            return ch
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old)
