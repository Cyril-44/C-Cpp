#include <stdio.h>
#include <string.h>
#include <vector>
constexpr int N = 500005;
char s[N];
struct SuffixArray {
    inline SuffixArray(char *s) : n(strlen(s)), sa(n+1), rk(n+1) {}
    int n;
    std::vector<int> sa, rk;
};
int main() {
    int t, k;
    scanf("%s%d%d", s, &t, &k);
    return 0;
}
