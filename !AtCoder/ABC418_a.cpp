#include <cstdio>
#include <algorithm>
#include <cstring>
const int N = 25;
char s[N];
int main() {
    int n;
    scanf("%d %s", &n, s);
    puts(s[n-3] == 't' && s[n-2] == 'e' && s[n-1] == 'a' ? "Yes" : "No");
    return 0;
}