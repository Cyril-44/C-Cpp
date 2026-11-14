#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> A(N);
    for (int &price : A) cin >> price;

    int up = 0, down = 0, flat = 0;
    for (int i = 1; i < N; ++i) {
        if (A[i] > A[i - 1]) ++up;
        else if (A[i] < A[i - 1]) ++down;
        else ++flat;
    }

    cout << up << ' ' << down << ' ' << flat << '\n';
    return 0;
}
