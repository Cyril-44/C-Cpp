#include "partial_common.h"
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        if (n <= 4) printPath(subsetPath(n)); else unsupported();
    }
}
