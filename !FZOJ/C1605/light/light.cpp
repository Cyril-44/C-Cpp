#include <cstdio>
inline bool C(int n, int m) {
    if (n < m || n < 0 || m < 0) return 0;
    return (n&m) == m;
}
int main() {
    int Q, op, n, m;
    scanf("%d", &Q);
    while (Q--) {
        scanf("%d%d%d", &op, &n, &m);
        bool ans = op == 1 ? (
            m == 0 ? n == 0 :
            2*m < n ? 0 :
            C(n-m + (2*m-n)/2, n-m)
        ) : (
            m == 0 ? n == 0 : 
            C(n-m + (m-1)/2, n-m)
        );
        putchar(ans | '0');
    }
    return 0;
}