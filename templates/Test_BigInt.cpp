#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <bitset>
#include <vector>
#include <algorithm>
// #include "FastIO.cpp"
#include "BigInt.cpp"
int main()
{
    int T;
    scanf("%d", &T);
    Decimal a, b;
    while (T--)
    {
        a.in(), b.in();
        puts((a + b).c_str());
    }
    return 0;
}