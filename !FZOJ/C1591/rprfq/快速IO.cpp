class IO {
    char ibuf[1 << 16], obuf[1 << 16], *ipl = ibuf, *ipr = ibuf, *op = obuf;
    public:
    ~IO() { write(); }
    void read() { if (ipl == ipr) ipr = (ipl = ibuf) + fread(ibuf, 1, 1 << 15, stdin); }
    void write() { fwrite(obuf, 1, op - obuf, stdout), op = obuf; }
    char getchar() { return (read(), ipl != ipr) ? *ipl++ : EOF; }
    void putchar(char c) { *op++ = c; if (op - obuf > (1 << 15)) write(); }
    template <typename V> IO& operator >> (V& v) {
        int s = 1; char c = getchar(); v = 0;
        for (; !isdigit(c); c = getchar()) if (c == '-') s = -s;
        for (; isdigit(c); c = getchar()) v = (v << 1) + (v << 3) + (c ^ 48);
        return v *= s, *this;
    }
    IO& operator << (char c) { return putchar(c), *this; }
    template <typename V> IO& operator << (V v) {
        if (!v) putchar('0'); if(v < 0) putchar('-'), v = -v;
        char stk[100], *top = stk;
        for (; v; v /= 10) *++top = v % 10 + '0';
        while (top != stk) putchar(*top--); return *this;
    }
} io;
