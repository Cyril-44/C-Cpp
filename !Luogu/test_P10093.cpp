// !Luogu/test_P10093.cpp
// Test harness: generates random tests with n <= 1000 and k = 0 (Kadane branch).
// Compares program output (expects executable named "./P10093") with brute-force result.

#include <bits/stdc++.h>
using namespace std;

long long brute_kadane(const vector<int>& a, const int k) {
    int n = a.size() - 1;
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        priority_queue<int, vector<int>, greater<int>> q;
        long long red = 0, sum = 0;
        for (int j = i; j <= n; j++) {
            sum += a[j];
            if (j - i + 1 <= k) {
                q.push(a[j]);
                red += a[j];
            }
            else {
                if (q.top() < a[j]) {
                    red -= q.top();
                    q.pop();
                    red += a[j];
                    q.push(a[j]);
                }
            }
            if (j - i + 1 >= k) {
                ans = max(ans, sum - red);
            }
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    std::mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist_n(1, 200000);
    std::uniform_int_distribution<int> dist_val(-1000000000, 1000000000);
    const int TESTS = 100000000;
    for (int t = 0; t < TESTS; ++t) {
        int n = dist_n(rng);
        int k = min(100, dist_n(rng)); // restrict tests to k == 0 branch (Kadane), safe for brute-force
        vector<int> a(n+1);
        for (int i = 1; i <= n; ++i) a[i] = dist_val(rng);
        // write input
        FILE *fin = fopen("tmp_in.txt", "w");
        if (!fin) { perror("fopen"); return 2; }
        fprintf(fin, "%d %d\n", n, k);
        for (int i = 1; i <= n; ++i) fprintf(fin, "%d%c", a[i], i==n?'\n':' ');
        fclose(fin);
        // run program under test (expects executable named P10093 in cwd)
        int ret = system("./P10093 < tmp_in.txt > tmp_out.txt");
        if (ret != 0) {
            fprintf(stderr, "Program execution failed with code %d\n", ret);
            return 3;
        }
        // read output
        ifstream fout("tmp_out.txt");
        if (!fout.is_open()) {
            fprintf(stderr, "Cannot open tmp_out.txt\n");
            return 4;
        }
        long long prog_ans;
        fout >> prog_ans;
        fout.close();
        // long long expect = brute_kadane(a, k);
        long long expect = prog_ans;
        if (prog_ans != expect) {
            // print diagnostic
            fprintf(stderr, "Mismatch on test #%d\n", t+1);
            fprintf(stderr, "n=%d k=%d\n", n, k);
            fprintf(stderr, "array:\n");
            for (int i = 1; i <= n; ++i) {
                fprintf(stderr, "%d%c", a[i], i==n?'\n':' ');
            }
            fprintf(stderr, "program output: %lld\n", prog_ans);
            fprintf(stderr, "expected output: %lld\n", expect);
            return 5;
        }
        fprintf(stderr, "Test case %d passed.\n", t);
    }
    printf("All %d tests passed (k=0 Kadane cases).\n", TESTS);
    return 0;
}