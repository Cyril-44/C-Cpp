#include <stdio.h>
const int N = 1005;
char s[N], t[N];
inline bool ok(char* ptr) {
    return  (*ptr | 32) == 'i' &&
        (*(ptr-1) | 32) == 'o' &&
        (*(ptr-2) | 32) == 'i' &&
        (*(ptr-3) | 32) == 'r';
}
int main() {
    scanf("%s %s", s, t);
    int n;
    bool flg1 = false;
    for (n = 0; !flg1 && s[n]; ++n) {
        if (n >= 3 && ok(s + n))
            flg1 = true;
    }
    bool flg2 = false;
    for (n = 0; !flg2 && t[n]; ++n) {
        if (n >= 3 && ok(t + n))
            flg2 = true;
    }
    if (flg1 && flg2) puts("Either is ok!");
    else if (flg1) printf("%s for sure!\n", s);
    else if (flg2) printf("%s for sure!\n", t);
    else puts("Try again!");
    return 0;
}