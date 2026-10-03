#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 998244353;

class FastInput {
    static constexpr int SIZE = 1 << 16;
    char buffer[SIZE];
    int pos = 0, len = 0;

    int getChar() {
        if (pos == len) {
            len = (int)fread(buffer, 1, SIZE, stdin);
            pos = 0;
            if (len == 0) return EOF;
        }
        return buffer[pos++];
    }

public:
    template<class T>
    bool read(T &value) {
        int c;
        do {
            c = getChar();
            if (c == EOF) return false;
        } while (c < '0' || c > '9');

        value = 0;
        do {
            value = value * 10 + (c - '0');
            c = getChar();
        } while (c >= '0' && c <= '9');

        return true;
    }
};

long long modPow(long long x, int exponent) {
    long long result = 1;
    while (exponent) {
        if (exponent & 1) result = result * x % MOD;
        x = x * x % MOD;
        exponent >>= 1;
    }
    return result;
}

int main() {
    FastInput input;
    int n = 0;
    if (!input.read(n)) return 0;

    vector<int> factorial(n + 1), inverseFactorial(n + 1);

    factorial[0] = 1;
    for (int i = 1; i <= n; ++i)
        factorial[i] = 1LL * factorial[i - 1] * i % MOD;

    inverseFactorial[n] = (int)modPow(factorial[n], MOD - 2);
    for (int i = n; i >= 1; --i)
        inverseFactorial[i - 1] =
            1LL * inverseFactorial[i] * i % MOD;

    auto combination = [&](int total, int chosen) -> long long {
        if (chosen < 0 || chosen > total) return 0;
        return 1LL * factorial[total] * inverseFactorial[chosen] % MOD
            * inverseFactorial[total - chosen] % MOD;
    };

    long long first = 0, previous = 0, last = 0;
    input.read(first);       // a[0]
    input.read(previous);    // a[1]

    long long upperWays = 1;
    int start = 1;

    auto finishRun = [&](int end) {
        int length = end - start + 1;
        upperWays = (upperWays + combination(n, end)
            - combination(n - length, end - length) + MOD) % MOD;
    };

    // 读入内部元素，同时处理等值连续段。
    for (int i = 2; i <= n; ++i) {
        long long current = 0;
        input.read(current);

        if (current != previous) {
            finishRun(i - 1);
            start = i;
        }
        previous = current;
    }

    finishRun(n);
    input.read(last);        // a[n+1]

    long long minimumScore = 2LL * n * (last - first);
    long long ways = upperWays * upperWays % MOD;

    cout << minimumScore << ' ' << ways << '\n';
    return 0;
}