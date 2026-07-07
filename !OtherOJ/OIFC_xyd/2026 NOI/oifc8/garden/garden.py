from math import gcd

def mod_inverse(b, m):
    """扩展欧几里得求逆元"""
    def egcd(a, b):
        if a == 0:
            return b, 0, 1
        g, y, x = egcd(b % a, a)
        return g, x - (b // a) * y, y
    g, x, _ = egcd(b, m)
    if g != 1:
        raise ValueError("No inverse")
    return x % m

def min_ap_mod(a, b, m, L):
    """计算 min_{i=0}^{L-1} (a + b*i) % m ，假设 a,b >=0"""
    if L <= 0:
        return float('inf')  # 或处理
    a %= m
    b %= m
    if b == 0:
        return a
    if L == 1:
        return a
    
    d = gcd(b, m)
    if d > 1:
        r0 = a % d
        a1 = a // d
        b1 = b // d
        m1 = m // d
        min_s = min_ap_mod(a1, b1, m1, L)
        return r0 + d * min_s
    
    # 现在 gcd(b, m) == 1
    if L >= m:
        return 0
    
    # 检查是否能达到 0
    inv = mod_inverse(b, m)
    i0 = ((-a) * inv) % m
    if i0 < L:
        return 0
    
    # 无 0，计算 kmax
    # 注意：Python 整数任意大
    kmax = (a + b * (L - 1)) // m
    if kmax == 0:
        return a  # 无 wrap，序列递增，最小为 a
    
    # 计算 wraps 产生的余数中的最小值
    # r_k = b - ((k*m - a) % b)   (非0)
    bb = b
    mm = m % bb
    aa = a % bb
    c = (-aa) % bb
    # 求 max_res = max( (c + mm * k) % bb for k=0 to kmax )
    # 使用 min 函数递归计算
    min_neg = min_ap_mod(c, mm, bb, kmax + 1)  # 注意这里模数 bb < m (原b < m)
    max_res = bb - min_neg if min_neg != 0 else bb - 1 if kmax + 1 >= 1 else 0  # 处理 hit 0 的情况
    
    min_from_wrap = bb - max_res
    return min(a, min_from_wrap)

# 主函数
def solve(X, Y, L, m):
    return min_ap_mod(X, Y, m, L)

l,x,y,m = map(int, input().split())
print(solve(x, y, l, m))