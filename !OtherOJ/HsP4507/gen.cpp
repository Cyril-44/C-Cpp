#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: ./gen N mode seed Q_limit\n";
        return 1;
    }
    int N = atoi(argv[1]);
    int mode = atoi(argv[2]);
    uint64_t seed = atoll(argv[3]);
    long long Q_limit = atoll(argv[4]);

    cout << N << "\n" << mode << "\n" << seed << "\n" << Q_limit << "\n";
    return 0;
}
