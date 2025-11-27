#include <cstdio>
const int N = 100005;
char s[N];
int main() {
    int t, k;
    scanf("%d%d", &t, &k);
    while (t--) {
        scanf("%s", s);
        long long prod = 1;
        for (int i = 0; s[i]; i++) {
            prod *= (s[i] ^ '0');
            if (prod > k) prod = k + 1;
            if (prod == 0) break;
        }
        puts(prod <= k ? "kawaii" : "dame");
    }
    return 0;
}