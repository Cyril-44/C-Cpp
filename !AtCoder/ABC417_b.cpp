#include <cstdio>
#include <set>
const int N = 105;
int a[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::set<int> st;
    for (int x; m--; ) {
        scanf("%d", &x);
        st.insert(x);
    }
    
    return 0;
}