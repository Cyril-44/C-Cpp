#include <stdio.h>
#include <algorithm>
const int N=1000005;
char s[N];
int main(){
    int id, n, q, l, r;
    scanf("%d%d%d %s", &id, &n, &q, s+1);
    while (q--) {
        scanf("%d%d", &l, &r);
        int ans = 0;
        for(int i = 1; i <= r - l + 1; i++) {
            ans ^= i + std::min(i, r - l + 2 - i);
            fprintf(stderr, "%d ", std::min(i, r - l + 2 - i));
        }
        fprintf(stderr, "\n");
        printf("%d\n", ans);
    }
    return 0;
}