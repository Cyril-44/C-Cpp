#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 200005;
int queries[N], son[N][2];
namespace HLD {
    
}
int main() {
    int n, m;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &son[i][0], &son[i][1]);
    scanf("%d", &m);
    for (int i = m; i >= 1; i--)
        scanf("%d", &queries[i]);
    
    return 0;
}
