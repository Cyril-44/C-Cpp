#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    int type = atoi(argv[1]); // 1: small, 2: mid, 3: large, 4: zero
    int seed = atoi(argv[2]);
    mt19937 rng(seed);

    int n;
    if (type == 1) n = rng() % 10 + 1;
    else if (type == 2) n = rng() % 1000 + 1;
    else if (type == 4) n = 100000;
    else n = 5000000;

    // 确保 n 有较多约数，增加强度
    if (type >= 3) {
        int common_n[] = {5000000, 4324320};
        n = common_n[rng() % 2];
    }

    printf("%d\n", n);
    for (int i = 0; i < n; ++i) {
        if (type == 4) printf("0%c", i == n - 1 ? '\n' : ' ');
        else printf("%lu%c", rng() % 1000001, i == n - 1 ? '\n' : ' ');
    }
    return 0;
}