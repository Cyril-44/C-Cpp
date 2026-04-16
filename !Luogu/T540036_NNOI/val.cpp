#include <iostream>
#include <cassert>

using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 1;
    assert(n >= 1 && n <= 5000000);
    int val;
    for (int i = 0; i < n; ++i) {
        assert(cin >> val);
        assert(val >= 0 && val <= 1000000);
    }
    return 0;
}