#include <cstdio>
typedef long long LL;
const int N = 1000050;
int p[N / 10];
LL S[N / 10];
bool mark[N];
int main() {
  freopen("theory.in", "r", stdin);
  freopen("theory.out", "w", stdout);
  int m = 0;
  for (int i = 2; i < N; ++i) {
    if (!mark[i]) p[m++] = i;
    for (int j = 0; j < m && (LL)p[j] * i < N; ++j) {
      mark[i * p[j]] = 1;
      if (!(i % p[j])) break;
    } 
  }
  for (int i = 1; i <= m; ++i) S[i] = S[i - 1] + p[i - 1];
  int T, n, k;
  scanf("%d", &T);
  while (T--) {
    scanf("%d%d", &n, &k);
    if (k > m || S[k] > n) {
      printf("-1\n");
      continue;
    }
    int l = k, r = m;
    while (l < r) {
      int mid = r + (l - r) / 2;
      if (S[mid] - S[mid - k] > n) r = mid - 1;
      else l = mid;
    }
    printf("%d\n", (int)(S[l] - S[l - k]));
  }
  return 0;
}
