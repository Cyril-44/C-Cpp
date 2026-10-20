#include <bits/stdc++.h>
using namespace std;

// Usage: checker input contestant_output answer
// Exit codes: AC=0, WA=1, judge failure=3. Whitespace is unrestricted;
// exactly q pairs of integer tokens are required in each output file.
static constexpr long long MOD = 998244353;

bool read_integer(istream& input, long long& result) {
    string text;
    if (!(input >> text) || text.empty()) return false;
    size_t position = 0;
    bool negative = false;
    if (text[position] == '+' || text[position] == '-') {
        negative = text[position] == '-';
        if (++position == text.size()) return false;
    }
    const unsigned long long limit = static_cast<unsigned long long>(LLONG_MAX) + (negative ? 1ULL : 0ULL);
    unsigned long long value = 0;
    for (; position < text.size(); ++position) {
        if (text[position] < '0' || text[position] > '9') return false;
        const unsigned digit = unsigned(text[position] - '0');
        if (value > (limit - digit) / 10ULL) return false;
        value = value * 10ULL + digit;
    }
    if (negative && value == static_cast<unsigned long long>(LLONG_MAX) + 1ULL) result = LLONG_MIN;
    else result = negative ? -static_cast<long long>(value) : static_cast<long long>(value);
    return true;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto failure = [](const string& message) { cerr << "JUDGE FAILURE: " << message << '\n'; return 3; };
    auto wrong = [](const string& message) { cerr << "WA: " << message << '\n'; return 1; };
    if (argc != 4) return failure("usage: checker input contestant_output answer");
    ifstream input(argv[1], ios::binary), contestant(argv[2], ios::binary), answer(argv[3], ios::binary);
    if (!input || !answer || !contestant) return failure("cannot open one or more required files");
    long long n, q;
    if (!read_integer(input, n) || !read_integer(input, q) || n < 1 || n > 100000 || q < 1 || q > 100000)
        return failure("invalid input header");
    const long long maximum_cost = n * 1000000000LL;
    vector<pair<long long, int>> expected;
    expected.reserve(size_t(q));
    for (int step = 1; step <= q; ++step) {
        long long cost, ways;
        if (!read_integer(answer, cost) || !read_integer(answer, ways))
            return failure("missing or non-integer answer at operation " + to_string(step));
        if (cost < 0 || cost > maximum_cost || ways < 0 || ways >= MOD)
            return failure("answer value out of range at operation " + to_string(step));
        expected.emplace_back(cost, int(ways));
    }
    string extra;
    if (answer >> extra) return failure("extra token in answer file");
    for (int step = 1; step <= q; ++step) {
        long long cost, ways;
        if (!read_integer(contestant, cost) || !read_integer(contestant, ways))
            return wrong("missing or non-integer answer at operation " + to_string(step));
        if (cost < 0 || cost > maximum_cost || ways < 0 || ways >= MOD)
            return wrong("value out of range at operation " + to_string(step));
        if (cost != expected[step - 1].first || ways != expected[step - 1].second)
            return wrong("operation " + to_string(step) + ": expected " +
                to_string(expected[step - 1].first) + " " + to_string(expected[step - 1].second) +
                ", found " + to_string(cost) + " " + to_string(ways));
    }
    if (contestant >> extra) return wrong("extra token after the final answer");
    cerr << "AC: " << q << " answers match\n";
    return 0;
}
