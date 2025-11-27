#include <stdio.h>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
constexpr int N = 1000005, FIXED = 1 << 8;
char s[N << 1];
bool f[N];
int main() {
    std::vector<int> a;
    for (int T = 1; ~scanf("%s", s); ++T) {
        int n = strlen(s);
        memcpy(s+n, s, n);
        int last = 0;
        std::vector<std::string> all;
        all.reserve(n / FIXED);
        for (int i = 1; i < (n<<1); i++)
            if (i - last >= n || s[i] == s[i-1]) {
                all.emplace_back(s+last, s+i);
                last = i;
            }
        for (auto &i : all)
            if (i.size() > 1 && i.back() == i.front())
                i.pop_back();
        std::sort(all.begin(), all.end(), [](const std::string &x, const std::string &y) {
            return x.length() > y.length();
        });

        memset(f, 0, sizeof f);
        for (const auto &str : all) {
            std::vector<int> pi(str.length());
            pi[0] = 0;
            int m = str.length();
            for (int i = 1; i < m; i++) {
                int j = pi[i-1];
                while (j && str[i] != str[j]) j = pi[j];
                if (str[i] != str[j]) pi[i] = 0;
                else pi[i] = j + 1;
            }
            if (pi[m-1] && m % (m - pi[m-1]) == 0) {
                int len = m - pi[m-1];
                for (int i = 1; i <= m; i++)
                    if ((i - 1) % len) f[i] = true;
            }
            else for (int i = 1; i <= m; i++) f[i] = true;
        }
        f[1] = true;
        printf("Case %d: ", T);
        for (int i = n; i >= 1; i--)
            putchar(f[i] | '0');
        putchar('\n');
    }
    return 0;
} // ababbabcababbabc abab babcaba b babc
