#include <cstdio>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <string>
#define debug(...) fprintf(stderr, __VA_ARGS__)
const int N = 100005, MOD = 10007;
char s[N];
inline void upd(int a[2], int *b, const char& c) {
	switch (c) {
	case '+':
		a[0] = a[0] * b[0] % MOD;
		a[1] = (a[0] * b[1] % MOD + a[1] * b[0] % MOD + a[1] * b[1] % MOD) % MOD;
		break;
	case '*':
		a[0] = (a[0] * b[0] % MOD + a[0] * b[1] % MOD + a[1] * b[0] % MOD) % MOD;
		a[1] = a[1] * b[1] % MOD;
		break;
	default: throw std::runtime_error("Fucking bitch");
	}
}
int main() {
	int l;
	scanf("%d %s", &l, s+1);
	static char sta[N];
    static int f[N][2];
    int top(1);
	f[1][0] = f[1][1] = 1;
    for (int i = 1; i <= l; i++) switch (s[i]) {
    case '+':
		if (sta[top] == '*') {
			for (; top > 1 && sta[top] == '*'; --top) upd(f[top-1], f[top], '*');
		}
		sta[++top] = '+';
		f[top][0] = (s[i+1] != '(' ? 1 : 0), f[top][1] = (s[i+1] != '(' ? 1 : 0);
		break;
	case '*':
	case '(':
		sta[++top] = s[i];
		f[top][0] = (s[i+1] != '(' ? 1 : 0), f[top][1] = (s[i+1] != '(' ? 1 : 0);
		break;
	case ')':
		for (; top > 0 && sta[top] != '('; --top) upd(f[top-1], f[top], sta[top]);
		upd(f[top-1], f[top], sta[top-1]); --top;
		break;
    }
	while (top > 1) upd(f[top-1], f[top], sta[top]), --top;
	printf("%d\n", f[1][0]);
	return 0;
}