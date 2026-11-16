#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int MAX_LEVEL = 1'000'025;

struct Node {
    long long sumF = 0;
    long long evenWeight = 0;
    int sumG = 0;
    int evenCount = 0;
    int lazy = 0;
    int cnt = 0;
};

Node seg[4 * (MAX_LEVEL + 1)];
int worries[MAX_LEVEL + 1];

long long weightSum(int left, int right) {
    return 1LL * (left + right - 2) * (right - left + 1) / 2;
}

void build(int p, int left, int right) {
    Node &node = seg[p];
    node.evenCount = right - left + 1;
    node.evenWeight = weightSum(left, right);
    if (left == right) return;

    int mid = (left + right) / 2;
    build(p * 2, left, mid);
    build(p * 2 + 1, mid + 1, right);
}

void pull(int p) {
    const Node &a = seg[p * 2];
    const Node &b = seg[p * 2 + 1];
    Node &node = seg[p];
    node.sumF = a.sumF + b.sumF;
    node.sumG = a.sumG + b.sumG;
    node.evenCount = a.evenCount + b.evenCount;
    node.evenWeight = a.evenWeight + b.evenWeight;
}

void apply(int p, int left, int right, int delta) {
    Node &node = seg[p];
    const int length = right - left + 1;
    const long long allWeight = weightSum(left, right);
    const int oddCount = length - node.evenCount;
    const int half = delta >= 0 ? delta / 2 : (delta - 1) / 2;
    node.sumG += static_cast<int>(1LL * half * length);
    node.sumF += 1LL * half * allWeight;
    if (delta % 2 != 0) {
        node.sumG += node.evenCount;
        node.sumF += node.evenWeight + node.evenCount - oddCount;
        node.evenCount = oddCount;
        node.evenWeight = allWeight - node.evenWeight;
    }

    node.lazy += delta;
    if (left == right) node.cnt += delta;
}

void push(int p, int left, int right) {
    int delta = seg[p].lazy;
    if (delta == 0 || left == right) return;

    int mid = (left + right) / 2;
    apply(p * 2, left, mid, delta);
    apply(p * 2 + 1, mid + 1, right, delta);
    seg[p].lazy = 0;
}

void rangeAdd(int p, int left, int right, int ql, int qr, int delta) {
    if (ql <= left && right <= qr) {
        apply(p, left, right, delta);
        return;
    }

    push(p, left, right);
    int mid = (left + right) / 2;
    if (ql <= mid) rangeAdd(p * 2, left, mid, ql, qr, delta);
    if (qr > mid) rangeAdd(p * 2 + 1, mid + 1, right, ql, qr, delta);
    pull(p);
}

void setCount(int p, int left, int right, int index, int value) {
    if (left == right) {
        Node &node = seg[p];
        node.cnt = value;
        node.lazy = 0;
        node.evenCount = (value % 2 == 0);
        node.evenWeight = node.evenCount ? left - 1 : 0;
        node.sumG = (value + 1) / 2;
        node.sumF = 1LL * node.sumG * (left - 1) + (value & 1);
        return;
    }

    push(p, left, right);
    int mid = (left + right) / 2;
    if (index <= mid)
        setCount(p * 2, left, mid, index, value);
    else
        setCount(p * 2 + 1, mid + 1, right, index, value);
    pull(p);
}

int getCount(int p, int left, int right, int index) {
    if (left == right) return seg[p].cnt;

    push(p, left, right);
    int mid = (left + right) / 2;
    if (index <= mid)
        return getCount(p * 2, left, mid, index);
    return getCount(p * 2 + 1, mid + 1, right, index);
}

int firstBad(int p, int left, int right, int start, bool wantOdd) {
    if (right < start) return MAX_LEVEL + 1;
    const Node &node = seg[p];
    if (left >= start) {
        if (wantOdd ? node.evenCount == 0
                    : node.evenCount == right - left + 1)
            return MAX_LEVEL + 1;
    }
    if (left == right) return left;

    push(p, left, right);
    int mid = (left + right) / 2;
    int answer = firstBad(p * 2, left, mid, start, wantOdd);
    if (answer <= MAX_LEVEL) return answer;
    return firstBad(p * 2 + 1, mid + 1, right, start, wantOdd);
}

void changeWorry(int level, int newAmount) {
    int delta = newAmount - worries[level];
    worries[level] = newAmount;
    if (delta == 0) return;

    int oldCount = getCount(1, 2, MAX_LEVEL, level);
    setCount(1, 2, MAX_LEVEL, level, oldCount + delta);
    int carry = (oldCount + delta) / 2 - oldCount / 2;
    int index = level + 1;

    while (carry != 0 && index <= MAX_LEVEL) {
        if (carry == 1 || carry == -1) {
            bool wantOdd = (carry == 1);
            int bad = firstBad(1, 2, MAX_LEVEL, index, wantOdd);
            int end = min(MAX_LEVEL, bad);
            if (index <= end) rangeAdd(1, 2, MAX_LEVEL, index, end, carry);
            break;
        }

        oldCount = getCount(1, 2, MAX_LEVEL, index);
        setCount(1, 2, MAX_LEVEL, index, oldCount + carry);
        int nextCarry = (oldCount + carry) / 2 - oldCount / 2;
        carry = nextCarry;
        ++index;
    }
}

long long answerQuery(int k, int ones) {
    if (k <= ones) return k;
    int remaining = k - ones;

    if (seg[1].sumG <= remaining) {
        return ones + seg[1].sumF +
               1LL * (remaining - seg[1].sumG) * MAX_LEVEL;
    }

    int p = 1, left = 2, right = MAX_LEVEL;
    long long fBefore = 0;
    int gBefore = 0;
    while (left < right) {
        push(p, left, right);
        int mid = (left + right) / 2;
        if (gBefore + seg[p * 2].sumG > remaining) {
            p *= 2;
            right = mid;
        } else {
            fBefore += seg[p * 2].sumF;
            gBefore += seg[p * 2].sumG;
            p = p * 2 + 1;
            left = mid + 1;
        }
    }

    int x = left - 1;
    return ones + fBefore + 1LL * (remaining - gBefore) * x;
}

}  // namespace

int main() {
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);

    int cid, q;
    if (scanf("%d%d", &cid, &q) != 2) return 0;

    build(1, 2, MAX_LEVEL);
    int ones = 0;
    while (q--) {
        int type, x;
        scanf("%d%d", &type, &x);
        if (type == 1) {
            int y;
            scanf("%d", &y);
            if (x == 1)
                ones = y;
            else
                changeWorry(x, y);
        } else {
            printf("%lld\n", answerQuery(x, ones));
        }
    }
    return 0;
}
