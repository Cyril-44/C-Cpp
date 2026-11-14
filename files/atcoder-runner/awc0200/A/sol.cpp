#include <iostream>
#include <string>
using namespace std;

int main() {
    int N, K;
    cin >> N >> K;

    string cloth;
    cloth.reserve(N);
    for (int i = 0; i < N; ++i) {
        cloth += ((i / K) % 2 == 0) ? 'R' : 'B';
    }
    cout << cloth << '\n';
    return 0;
}
