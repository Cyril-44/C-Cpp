#include <cstdio>
const int N = 1005;
char s[N];
int main() {
    scanf("%s", s);
    int last = 0;
    for (int i = 0; s[i]; i++) {
        if (s[i] == '#') {
            if (last) {
                printf("%d,%d\n", last, i+1);
                last = 0;
            }
            else last = i+1;
        }
    }
    return 0;
}