#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // Calculate the initial rating without skipping any contests
        vector<int> prefixIncrease(n + 1, 0);
        vector<int> prefixDecrease(n + 1, 0);
        int x = 0;
        for (int i = 0; i < n; ++i) {
            if (a[i] > x) {
                ++x;
                prefixIncrease[i + 1] = 1;
            } else if (a[i] < x) {
                --x;
                prefixDecrease[i + 1] = 1;
            }
        }

        // Calculate prefix sums for increases and decreases
        for (int i = 1; i <= n; ++i) {
            prefixIncrease[i] += prefixIncrease[i - 1];
            prefixDecrease[i] += prefixDecrease[i - 1];
        }

        int maxRating = INT_MIN;
        for (int l = 0; l < n; ++l) {
            for (int r = l; r < n; ++r) {
                int skippedIncrease = prefixIncrease[r + 1] - prefixIncrease[l];
                int skippedDecrease = prefixDecrease[r + 1] - prefixDecrease[l];
                int rating = x + skippedDecrease - skippedIncrease;
                maxRating = max(maxRating, rating);
            }
        }

        cout << maxRating << endl;
    }
    return 0;
}
