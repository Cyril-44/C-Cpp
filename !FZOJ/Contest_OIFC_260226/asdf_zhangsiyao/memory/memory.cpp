#include <stdio.h>
#define Case break; case
constexpr int N = 500005;
char s[N];
struct QR { int l, r, lc, rc, w; } a[N];
int main() {
    freopen("memory.in", "r", stdin);
    freopen("memory.out", "w", stdout);
    int T, n, k;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d %s", &n, &k, s+1);
        int diffs = 0, addmn = 0, addmx = 0, last = 0, tot = 0;
        for (int i = 1; i <= n; i++) {
            if (s[i] != '?') {
                if (i > 1 && s[i-1] != '?' && s[i] != s[i-1]) ++diffs;
                if (s[i-1] == '?') {
                    int mn = last && s[i] != s[last],
                        mx = i - last - (!last | (i - last & 1) ^ (s[i] != s[last]));
                    addmn += mn, addmx += mx;
                    a[++tot] = {last + 1, i - 1, s[last] & 1, s[i] & 1, mx - mn};
                }
                last = i;
            }
        }
        if (n != last) addmx += n - last, a[++tot] = {last + 1, n, s[last] & 1, -1, n - last};
        if (s[1] == '?') a[1].lc = -1;
        if (tot == 1 && s[1] == '?' && s[n] == '?') --addmx, --a[1].w;
        bool extra = ((k ^ diffs ^ addmn) & 1);
        if (diffs + addmn > k || k > diffs + addmx || s[1] != '?' && s[n] != '?' && extra) { puts("-1"); continue; }
        int need = k - diffs - addmn, left = addmx - addmn;

        // fprintf(stderr, "=============================\n");
        // fprintf(stderr, "Constructive range: [%d, %d] = %d + [%d,%d]\n", diffs + addmn, diffs + addmx, diffs, addmn, addmx);
        // for (int i = 1; i <= tot; i++) fprintf(stderr, "Number %d zero range [%d,%d], lc %d, rc %d, value %d\n", i, a[i].l, a[i].r, a[i].lc, a[i].rc, a[i].w);
        // fprintf(stderr, "need=%d, left=%d, isextra=%d\n", need, left, extra*1);

        for (int t = 1, i, ch, counter; t <= tot; t++) {
            left -= a[t].w;
            if (need > left) {
                int must = need - left;
                // fprintf(stderr, "Entering MUST=%d!\n", must);
#define fill01R(init) for (i = a[t].r, ch = init, counter = must; counter--; i--, ch ^= 1) s[i] = ch;
#define fill0R() for (; i >= a[t].l; i--) s[i] = '0';
#define fill0L() for (i = a[t].l; i <= a[t].r - must; i++) s[i] = '0';
#define fill01L(init) for (ch = init; i <= a[t].r; i++, ch ^= 1) s[i] = ch;
                switch (a[t].lc * 3 + a[t].rc) {
                case 0: // 0____0  ==>  0001010 +4
                    if (must & 1) ++must;
                    fill01R('1'); fill0R();
                Case 1: // 0____1  ==>  0010101 +5 (nd-4)
                    if (must & 1) ++must;
                    fill01R('0'); fill0R();
                Case 3: // 1____0  ==>  1001010 +5 (nd-4)
                    if (must & 1) ++must;
                    fill01R('1'); fill0R();
                Case 4: // 1____1  ==>  1010101 +6
                    if (must & 1) ++must;
                    must -= 2; fill01R('0'); fill0R(); must += 2;
                Case 2: // 1____?  ==>  1001010 +5 / 10010101 +6
                    --must; fill0L(); fill01L('1');
                Case -1: // 0____? ==>  0001010 +4 / 00010101 +5
                    fill0L(); fill01L('1');
                Case -2: // ?____1 ==>  0010101 +5 / 10000101 +4
                    if (must & 1) { fill01R('0'); fill0R(); }
                    else if (need - must >= 1 && a[t].r - must >= 1 && s[n] == '?') { ++must; extra = true; fill01R('0'); fill0R(); }
                    else { --must; fill01R('0'); ++must; fill0R(); s[1] = '1'; }
                Case -3: // ?____0 ==>  0101010 +6 / 10001010 +5
                    if (must & 1) {
                        if (need - must >= 1 && a[t].r - must >= 1 && s[n] == '?') { ++must; extra = true; fill01R('1'); fill0R(); }
                        else { --must; fill01R('1'); ++must; fill0R(); s[1] = '1'; }
                    } else { fill01R('1'); fill0R(); }
                Case -4: // ?____?
                    fill0L(); fill01L('1');
                }
                need -= must;
            } else {
#define fill0() for (int i = a[t].l; i <= a[t].r; i++) s[i] = '0';
#define fill1() for (int i = a[t].l; i <= a[t].r; i++) s[i] = '1';
                switch (a[t].lc * 3 + a[t].rc) {
                case 0: // 0____0
                case -4: // ?____?
                case -1: // 0____?
                    fill0();
                Case 1: // 0____1
                case 3: // 1____0
                    fill0();
                Case -3: // ?____0
                    fill0();
                    if (extra && s[n] != '?') { --need; s[1] = '1'; extra = false; }
                Case 2: // 1____?
                    if (need) {
                        if (extra) { --need; fill0(); }
                        else { if (n > a[t].l) need -= 2; fill0(); s[n] = '1'; }
                    } else { fill1(); }
                Case -2: // ?____1
                    if (need) {
                        if (extra) { --need; fill0(); extra = false; }
                        else if (s[n] == '?') { --need; fill0(); extra = true; }
                        else { if (a[t].r > 1) need -= 2; fill0(); s[1] = '1'; }
                    } else { fill1(); }
                Case 4: // 1____1
                    if (need >= 2) { need -= 2; fill0(); }
                    else { fill1(); }
                }
            }
            // fprintf(stderr, "After range[%2d,%2d], need=%2d, left=%2d, str=%s\n", a[t].l, a[t].r, need, left, s+1);
        }
        puts(s + 1);
    }
    return 0;
}