import sys

input = sys.stdin.read
data = input().split()

index = 0
id_ = int(data[index])
index += 1
n = int(data[index])
index += 1
m = int(data[index])
index += 1

MOD = 1 << 64  # 2**64, unsigned long long overflow is mod 2^64

class Matrix:
    def __init__(self, a=0, b=0, c=0, d=0):
        self.a = a
        self.b = b
        self.c = c
        self.d = d

    def __mul__(self, other):
        return Matrix(
            (self.a * other.a + self.b * other.c) & (MOD - 1),
            (self.a * other.b + self.b * other.d) & (MOD - 1),
            (self.c * other.a + self.d * other.c) & (MOD - 1),
            (self.c * other.b + self.d * other.d) & (MOD - 1)
        )

# Precompute fib[1..n+1]
fib = [0] * (n + 2)
fib[1] = 1
fib[2] = 1
for i in range(3, n + 2):
    fib[i] = (fib[i - 1] + fib[i - 2]) & (MOD - 1)

class Node:
    def __init__(self):
        self.sum = 0          # 区间和
        self.sqsum = 0        # 区间平方和
        self.fa = 0            # 斐波那契线性表示系数 F_{pos-1}
        self.fb = 0            # 斐波那契线性表示系数 F_{pos}
        self.lazy_a = 0
        self.lazy_b = 0

tree = [Node() for _ in range(4 * (n + 1))]

def push_down(p, len_):
    if tree[p].lazy_a == 0 and tree[p].lazy_b == 0:
        return
    left_len = len_ // 2
    right_len = len_ - left_len
    l = p * 2
    r = p * 2 + 1

    # 左子区间
    la = tree[p].lazy_a
    lb = tree[p].lazy_b
    old_sum_l = tree[l].sum
    old_sq_l = tree[l].sqsum
    add_sum_l = (la * fib[left_len + 1] + lb * fib[left_len]) & (MOD - 1)
    add_sq_l = (
        la * la * fib[left_len * 2 + 1] +
        lb * lb * fib[left_len * 2 - 1] +
        2 * la * lb * fib[left_len * 2]
    ) & (MOD - 1)
    tree[l].sum = (old_sum_l + add_sum_l) & (MOD - 1)
    tree[l].sqsum = (old_sq_l + add_sq_l + 2 * ((old_sum_l * add_sum_l) & (MOD - 1))) & (MOD - 1)

    # 合并到左子的lazy
    new_fa_l = (tree[l].fa + lb) & (MOD - 1)
    new_fb_l = (tree[l].fb + la + lb) & (MOD - 1)
    tree[l].lazy_a = (tree[l].lazy_a + la) & (MOD - 1)
    tree[l].lazy_b = (tree[l].lazy_b + lb) & (MOD - 1)
    tree[l].fa = new_fa_l
    tree[l].fb = new_fb_l

    if right_len > 0:
        # 右子区间
        ra = (la * fib[2] + lb * fib[3]) & (MOD - 1)  # F_{right_start-1}
        rb = (la * fib[3] + lb * fib[4]) & (MOD - 1)  # F_{right_start}
        old_sum_r = tree[r].sum
        old_sq_r = tree[r].sqsum
        add_sum_r = (ra * fib[right_len + 1] + rb * fib[right_len]) & (MOD - 1)
        add_sq_r = (
            ra * ra * fib[right_len * 2 + 1] +
            rb * rb * fib[right_len * 2 - 1] +
            2 * ra * rb * fib[right_len * 2]
        ) & (MOD - 1)
        tree[r].sum = (old_sum_r + add_sum_r) & (MOD - 1)
        tree[r].sqsum = (old_sq_r + add_sq_r + 2 * ((old_sum_r * add_sum_r) & (MOD - 1))) & (MOD - 1)

        new_fa_r = (tree[r].fa + rb) & (MOD - 1)
        new_fb_r = (tree[r].fb + ra + rb) & (MOD - 1)
        tree[r].lazy_a = (tree[r].lazy_a + ra) & (MOD - 1)
        tree[r].lazy_b = (tree[r].lazy_b + rb) & (MOD - 1)
        tree[r].fa = new_fa_r
        tree[r].fb = new_fb_r

    tree[p].lazy_a = 0
    tree[p].lazy_b = 0

def push_up(p):
    l = p * 2
    r = p * 2 + 1
    tree[p].sum = (tree[l].sum + tree[r].sum) & (MOD - 1)
    tree[p].sqsum = (tree[l].sqsum + tree[r].sqsum) & (MOD - 1)

def build(p, cl, cr):
    if cl == cr:
        tree[p].sum = 0
        tree[p].sqsum = 0
        tree[p].fa = fib[cl - 1] if cl >= 2 else (1 if cl == 1 else 0)
        tree[p].fb = fib[cl]
        return
    mid = (cl + cr) // 2
    build(p * 2, cl, mid)
    build(p * 2 + 1, mid + 1, cr)
    push_up(p)
    tree[p].fa = fib[cl - 1] if cl >= 2 else (1 if cl == 1 else 0)
    tree[p].fb = fib[cl]

def update(p, cl, cr, l, r):
    if l <= cl and cr <= r:
        len_ = cr - cl + 1
        la = fib[cl - l + 1]
        lb = fib[cl - l + 2]
        old_sum = tree[p].sum
        old_sq = tree[p].sqsum
        add_sum = (la * fib[len_ + 1] + lb * fib[len_]) & (MOD - 1)
        add_sq = (
            la * la * fib[len_ * 2 + 1] +
            lb * lb * fib[len_ * 2 - 1] +
            2 * la * lb * fib[len_ * 2]
        ) & (MOD - 1)
        tree[p].sum = (old_sum + add_sum) & (MOD - 1)
        tree[p].sqsum = (old_sq + add_sq + 2 * ((old_sum * add_sum) & (MOD - 1))) & (MOD - 1)
        tree[p].lazy_a = (tree[p].lazy_a + la) & (MOD - 1)
        tree[p].lazy_b = (tree[p].lazy_b + lb) & (MOD - 1)
        new_fa = (tree[p].fa + lb) & (MOD - 1)
        new_fb = (tree[p].fb + la + lb) & (MOD - 1)
        tree[p].fa = new_fa
        tree[p].fb = new_fb
        return
    push_down(p, cr - cl + 1)
    mid = (cl + cr) // 2
    if l <= mid:
        update(p * 2, cl, mid, l, r)
    if r > mid:
        update(p * 2 + 1, mid + 1, cr, l, r)
    push_up(p)

def query(p, cl, cr, l, r):
    if l <= cl and cr <= r:
        return tree[p].sqsum
    push_down(p, cr - cl + 1)
    mid = (cl + cr) // 2
    ans = 0
    if l <= mid:
        ans = (ans + query(p * 2, cl, mid, l, r)) & (MOD - 1)
    if r > mid:
        ans = (ans + query(p * 2 + 1, mid + 1, cr, l, r)) & (MOD - 1)
    return ans

build(1, 1, n)

ans_list = []
for _ in range(m):
    opt = int(data[index])
    index += 1
    l = int(data[index])
    index += 1
    r = int(data[index])
    index += 1
    if opt == 1:
        update(1, 1, n, l, r)
    else:
        res = query(1, 1, n, l, r)
        ans_list.append(str(res))

print('\n'.join(ans_list))