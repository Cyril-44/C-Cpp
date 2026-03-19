#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
constexpr int N = 2022, F = 515;
int a[N];
/*
                          __ 515 ---+---(a1)-------- a1
                         /__ 515    |___(515-a1) --- a2
 S --> a1 --> a2 --> a3 {___ 515
                         \__ 515
                          \_ 515
*/
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::queue<int> q1, q2;
    for (int i = 1; i <= n; i++)
        if (a[i] < F) q1.push(a[i]);
        else q2.push(a[i]);
    
    return 0;
}