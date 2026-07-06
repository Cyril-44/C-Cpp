#include <bits/stdc++.h>
int main() {
    double var = 99.9;
    printf("%llX\n", *(unsigned long long*)(double*)&var);
    return 0;
}