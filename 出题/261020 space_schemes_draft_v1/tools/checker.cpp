#include <bits/stdc++.h>
using namespace std;

// Usage: checker input contestant_output answer
// Exit codes: 0 = AC, 1 = WA, 3 = judge failure.
static constexpr long long MOD = 998244353;

bool parse_integer(const string& text, long long& result) {
    if (text.empty()) return false;
    size_t i = 0;
    bool negative = false;
    if (text[i] == '+' || text[i] == '-') {
        negative = text[i] == '-';
        if (++i == text.size()) return false;
    }
    const unsigned long long limit = negative
        ? static_cast<unsigned long long>(LLONG_MAX) + 1ULL
        : static_cast<unsigned long long>(LLONG_MAX);
    unsigned long long value = 0;
    for (; i < text.size(); ++i) {
        if (text[i] < '0' || text[i] > '9') return false;
        const unsigned digit = unsigned(text[i] - '0');
        if (value > (limit - digit) / 10ULL) return false;
        value = value * 10ULL + digit;
    }
    if (negative && value == static_cast<unsigned long long>(LLONG_MAX) + 1ULL)
        result = LLONG_MIN;
    else
        result = negative ? -static_cast<long long>(value) : static_cast<long long>(value);
    return true;
}

bool read_integer(istream& stream, long long& result) {
    string text;
    return bool(stream >> text) && parse_integer(text, result);
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto failure = [](const string& message) {
        cerr << "JUDGE FAILURE: " << message << '\n';
        return 3;
    };
    auto wrong_answer = [](const string& message) {
        cerr << "WA: " << message << '\n';
        return 1;
    };
    if (argc != 4) return failure("usage: checker input contestant_output answer");

    ifstream input(argv[1], ios::binary);
    ifstream contestant(argv[2], ios::binary);
    ifstream answer(argv[3], ios::binary);
    if (!input) return failure("cannot open input file");
    if (!answer) return failure("cannot open answer file");
    if (!contestant) return failure("cannot open contestant output file");
    long long n, q;
    if (!read_integer(input, n) || !read_integer(input, q) ||
        n < 1 || n > 100000 || q < 1 || q > 300000)
        return failure("input header is invalid");

    // Verify the entire answer first, so a bad jury answer is never reported
    // merely as a contestant wrong answer.
    vector<pair<int, int>> expected;
    expected.reserve(size_t(q));
    for (int step = 1; step <= q; ++step) {
        long long space, ways;
        if (!read_integer(answer, space) || !read_integer(answer, ways))
            return failure("answer: missing or non-integer token at operation " + to_string(step));
        if (space < 0 || space > n || ways < 0 || ways >= MOD)
            return failure("answer: out-of-range value at operation " + to_string(step));
        expected.emplace_back(int(space), int(ways));
    }
    string extra;
    if (answer >> extra) return failure("answer contains extra tokens");

    for (int step = 1; step <= q; ++step) {
        long long space, ways;
        if (!read_integer(contestant, space) || !read_integer(contestant, ways))
            return wrong_answer("missing or non-integer token at operation " + to_string(step));
        if (space < 0 || space > n || ways < 0 || ways >= MOD)
            return wrong_answer("out-of-range value at operation " + to_string(step));
        if (space != expected[step - 1].first || ways != expected[step - 1].second)
            return wrong_answer("operation " + to_string(step) + ": expected " +
                to_string(expected[step - 1].first) + " " +
                to_string(expected[step - 1].second) + ", found " +
                to_string(space) + " " + to_string(ways));
    }
    if (contestant >> extra) return wrong_answer("extra token after the last answer");
    cerr << "AC: " << q << " answers match\n";
    return 0;
}
