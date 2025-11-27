n : int = int(input())
arr : list[int] = map(int, input().split())
arr = sorted(arr)
for i in range(1, n):
	arr[i] += arr[i-1]
ans : int = -10**18
for i in range(0, n):
	ans = max(ans, -arr[i] + arr[n-1] - arr[i])
print(ans)

