n, m, k = map(int, input().split())
h = list(map(int, input().split()))
b = list(map(int, input().split()))
b = sorted(b)
p = 0
cnt = 0
for i in sorted(h):
    while p < m and b[p] < i:
        p += 1
    if p == m:
        break
    cnt += 1
    p += 1
if cnt >= k:
    print("Yes")
else:
    print("No")