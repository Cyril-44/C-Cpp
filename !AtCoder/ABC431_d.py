n = int(input())
a = []
for i in range(n):
    a.append(tuple(map(int, input().split())))
head, body = 0, 0
b = []
sum = 0
for (w, x, y) in a:
    if y > x:
        body += w
        sum += y
    else:
        b.append((w, x, y))
b = sorted(b, key = lambda x: x[2] - x[1])
for (w, x, y) in b:
    if head + w <= body:
        sum += x
        head += w
    else:
        sum += y
        body += w
print(sum)
