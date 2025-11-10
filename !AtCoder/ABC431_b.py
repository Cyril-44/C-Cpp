n = int(input())
a = []
for i in range(n):
    a.append(tuple(map(int, input().split())))

tot = 0
sum0 = 0
for (w, h, b) in a:
    tot += w
    sum0 += b

topw = tot // 2

INF = 10 ** 18
dp = [-INF] * (topw + 1)
dp[0] = 0

for (w, h, b) in a:
    for curr_w in range(topw, w - 1, -1):
        if dp[curr_w - w] != -INF:
            dp[curr_w] = max(dp[curr_w], dp[curr_w - w] + (h - b))

print(sum0 + max(dp))