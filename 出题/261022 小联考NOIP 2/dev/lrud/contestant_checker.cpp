// Usage: checker <input-file> <output-file>
#include <bits/stdc++.h>
using namespace std;

long long optimum(int n) {
    long long square = 1LL * n * n;
    if (n & 1) return (square - 1) / 2;
    if (n % 4 == 2) return (square + n - 2) / 2;
    if (n == 4 || n == 8) return (square + n - 4) / 2;
    return (square + n - 6) / 2;
}

int reject(const string& message) {
    cerr << "Wrong Answer: " << message << '\n';
    return 1;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input-file> <output-file>\n";
        return 2;
    }

    ifstream input(argv[1]);
    ifstream output(argv[2]);
    if (!input) return reject("cannot open input file");
    if (!output) return reject("cannot open output file");

    int t;
    if (!(input >> t) || t < 1 || t > 5) return reject("invalid T in input");
    vector<int> ns(t);
    for (int& n : ns) {
        if (!(input >> n) || n < 2 || n > 1000) return reject("invalid N in input");
    }
    string extra;
    if (input >> extra) return reject("extra tokens in input");

    const string symbols = "QWEADZXC";
    const int dr[] = {-1,-1,-1,0,0,1,1,1};
    const int dc[] = {-1,0,1,-1,1,-1,0,1};
    for (int tc = 0; tc < t; ++tc) {
        const int n = ns[tc];
        long long claimed;
        string path;
        if (!(output >> claimed)) return reject("missing claimed maximum in case " + to_string(tc + 1));
        if (!(output >> path)) return reject("missing path in case " + to_string(tc + 1));
        if (claimed != optimum(n))
            return reject("case " + to_string(tc + 1) + ": claimed value is not optimal");
        if (path.size() != size_t(1LL * n * n - 1))
            return reject("case " + to_string(tc + 1) + ": path length must be N^2-1");

        vector<unsigned char> seen(size_t(n) * n, 0);
        seen[0] = 1;
        int row = 0, col = 0, right = 0;
        for (size_t step = 0; step < path.size(); ++step) {
            size_t direction = symbols.find(path[step]);
            if (direction == string::npos)
                return reject("case " + to_string(tc + 1) + ": invalid direction at step " + to_string(step + 1));
            row += dr[direction];
            col += dc[direction];
            if (row < 0 || row >= n || col < 0 || col >= n)
                return reject("case " + to_string(tc + 1) + ": path leaves the grid at step " + to_string(step + 1));
            if (seen[size_t(row) * n + col]++)
                return reject("case " + to_string(tc + 1) + ": a cell is visited more than once");
            right += path[step] == 'D';
        }
        if (row != n - 1 || col != n - 1)
            return reject("case " + to_string(tc + 1) + ": path does not end at (N,N)");
        if (right != claimed)
            return reject("case " + to_string(tc + 1) + ": D count differs from the claimed value");
    }
    if (output >> extra) return reject("extra tokens in output");

    cout << "Accepted: all paths are valid and optimal\n";
    return 0;
}
