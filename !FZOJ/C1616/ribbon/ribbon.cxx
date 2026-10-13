#include <bits/stdc++.h>
using namespace std;
int n, a[200005], rt[200005], st1[200005], st2[200005], top1, top2;
int al[200005], ar[200005], lu[200005], ld[200005];
struct segtree {
    int tr[4000005], ch[4000005][2], idx;
    int clone(int x) {
        idx++;
        tr[idx] = tr[x];
        ch[idx][0] = ch[x][0], ch[idx][1] = ch[x][1];
        return idx;
    }
    void push_up(int x) { tr[x] = tr[ch[x][0]] + tr[ch[x][1]]; }
    void update(int &x, int l, int r, int w, int k) {
        x = clone(x);
        if (l == r) {
            tr[x] += k;
            return;
        }
        int mid = l + r >> 1;
        if (mid >= w)
            update(ch[x][0], l, mid, w, k);
        else
            update(ch[x][1], mid + 1, r, w, k);
        push_up(x);
    }
    int query(int x1, int x2, int l, int r, int L, int R) {
        if (l >= L && r <= R) return tr[x2] - tr[x1];
        int mid = l + r >> 1, cnt = 0;
        if (mid >= L) cnt = query(ch[x1][0], ch[x2][0], l, mid, L, R);
        if (mid < R) cnt += query(ch[x1][1], ch[x2][1], mid + 1, r, L, R);
        return cnt;
    }
} seg;
struct segtree2 {
    int tr[800005];
    void push_up(int x) { tr[x] = max(tr[x << 1], tr[x << 1 | 1]); }
    void update(int x, int l, int r, int w, int k) {
        if (l == r) {
            tr[x] = k;
            return;
        }
        int mid = l + r >> 1;
        if (mid >= w)
            update(x << 1, l, mid, w, k);
        else
            update(x << 1 | 1, mid + 1, r, w, k);
        push_up(x);
    }
    int query(int x, int l, int r, int L, int R) {
        if (l >= L && r <= R) return tr[x];
        int mid = l + r >> 1, cnt = 0;
        if (mid >= L) cnt = query(x << 1, l, mid, L, R);
        if (mid < R) cnt = max(cnt, query(x << 1 | 1, mid + 1, r, L, R));
        return cnt;
    }
} seg2;
void work(int x, int op) {
    if (op == 1) {
        int top1_ = top1;
        while (top1_ && a[st1[top1_]] < a[x]) top1_--;
        st1[++top1] = x;
        int l = 1, r = top1, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (top1 - mid + 1 == seg.query(rt[st1[mid] + 1], rt[i], 1, n, a[i], n))
                r = mid - 1;
            else
                l = mid + 1;
        }
        int xl = st1[l - 1] + 1, xr = st1[l], xx = top1 - l + 1;
        l = xl, r = xr, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (xx == seg.query(rt[mid - 1], rt[i], 1, n, a[i], n))
                r = mid - 1;
            else
                l = mid + 1;
        }
    }
}
int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) { scanf("%d", &a[i]); }
    for (int i = 1; i <= n; i++) {
        rt[i] = rt[i - 1];
        seg.update(rt[i], 1, n, a[i], 1);
        while (top1 && a[st1[top1]] < a[i]) top1--;
        st1[++top1] = i;
        int l = 1, r = top1, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (top1 - mid + 1 == seg.query(rt[st1[mid] - 1], rt[i], 1, n, a[i], n))
                r = mid - 1;
            else
                l = mid + 1;
        }
        int xl = st1[l - 1] + 1, xr = st1[l], xx = top1 - l + 1;
        l = xl, r = xr, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (xx == seg.query(rt[mid - 1], rt[i], 1, n, a[i], n))
                r = mid - 1;
            else
                l = mid + 1;
        }
        al[i] = l;
        while (top2 && a[st2[top2]] > a[i]) top2--;
        st2[++top2] = i;
        l = 1, r = top2;
        while (l <= r) {
            mid = l + r >> 1;
            if (top2 - mid + 1 == seg.query(rt[st2[mid] - 1], rt[i], 1, n, 1, a[i]))
                r = mid - 1;
            else
                l = mid + 1;
        }
        xl = st2[l - 1] + 1, xr = st2[l], xx = top2 - l + 1;
        l = xl, r = xr, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (xx == seg.query(rt[mid - 1], rt[i], 1, n, 1, a[i]))
                r = mid - 1;
            else
                l = mid + 1;
        }
        al[i] = max(al[i], l);
        lu[i] = seg2.query(1, 1, n, a[i], n);
        ld[i] = seg2.query(1, 1, n, 1, a[i]);
        seg2.update(1, 1, n, a[i], i);
    }
    seg.idx = 0;
    memset(rt, 0, sizeof(rt));
    memset(seg.tr, 0, sizeof(seg.tr));
    memset(seg.ch, 0, sizeof(seg.ch));
    top1 = top2 = 0;
    for (int i = n; i >= 1; i--) {
        rt[i] = rt[i + 1];
        seg.update(rt[i], 1, n, a[i], 1);
    }
    return 0;
}