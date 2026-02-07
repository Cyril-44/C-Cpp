#include<stdio.h>
int main() {
    char a,b,c;
    a=getchar(), b=getchar(), c=getchar();
    if (a==b&&a==c) puts("Yes");
    else puts("No");
    return 0;
}