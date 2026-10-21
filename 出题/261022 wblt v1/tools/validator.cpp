#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

// Standalone validator; no testlib dependency. All defaults match scoring.json.
namespace {
constexpr std::uint64_t MOD = 998244353;
constexpr std::uint64_t DEFAULT_MAX = 3000000;
constexpr std::uint64_t DEFAULT_SMALL = 2000;
constexpr std::uint64_t DEFAULT_MEDIUM = 20000;

class Reader {
    static constexpr std::size_t CAPACITY = 1 << 20;
    unsigned char buffer_[CAPACITY];
    std::size_t pos_ = 0, size_ = 0;
    std::uint64_t line_ = 1, column_ = 1;
public:
    int peek() {
        if (pos_ == size_) {
            size_ = std::fread(buffer_, 1, CAPACITY, stdin);
            pos_ = 0;
            if (size_ == 0) {
                if (std::ferror(stdin)) fail("input read error");
                return EOF;
            }
        }
        return buffer_[pos_];
    }
    int take() {
        const int ch = peek();
        if (ch != EOF) {
            ++pos_;
            if (ch == '\n') { ++line_; column_ = 1; }
            else ++column_;
        }
        return ch;
    }
    [[noreturn]] void fail(const std::string& message) const {
        throw std::runtime_error("line " + std::to_string(line_) + ", column " +
                                 std::to_string(column_) + ": " + message);
    }
    std::uint64_t integer(std::uint64_t low, std::uint64_t high, const char* name) {
        if (peek() < '0' || peek() > '9') fail(std::string("expected decimal integer for ") + name);
        std::uint64_t result = 0;
        while (peek() >= '0' && peek() <= '9') {
            const std::uint64_t digit = static_cast<std::uint64_t>(take() - '0');
            if (result > high / 10 || (result == high / 10 && digit > high % 10))
                fail(std::string(name) + " exceeds " + std::to_string(high));
            result = result * 10 + digit;
        }
        if (result < low) fail(std::string(name) + " is below " + std::to_string(low));
        return result;
    }
    void space() {
        if (take() != ' ') fail("expected exactly one ASCII space");
    }
    void endline() {
        int ch = take();
        if (ch == '\r') ch = take();
        if (ch != '\n') fail("expected end of line (LF or CRLF)");
    }
    void eof() {
        if (peek() != EOF) fail("extra token or byte after the last operation");
    }
};

std::uint64_t argument_number(const std::string& text, const char* option) {
    if (text.empty()) throw std::runtime_error(std::string(option) + " needs an integer");
    std::uint64_t value = 0;
    for (const char ch : text) {
        if (ch < '0' || ch > '9' || value > (1000000000ULL - (ch - '0')) / 10)
            throw std::runtime_error(std::string("invalid numeric argument for ") + option);
        value = value * 10 + static_cast<unsigned>(ch - '0');
    }
    return value;
}

int run(int argc, char** argv) {
    std::uint64_t n_max = DEFAULT_MAX, m_max = DEFAULT_MAX;
    std::uint64_t small = DEFAULT_SMALL, medium = DEFAULT_MEDIUM;
    int group = 0;
    for (int i = 1; i < argc; ++i) {
        const std::string option = argv[i];
        if (option == "--help" || option == "-h") {
            std::cout << "Usage: validator [--group 0..5] [--max N] [--max-n N] [--max-m M]\n"
                         "                 [--small-max N] [--medium-max N] < case.in\n"
                         "Group 0 (default) checks global validity only. Defaults: max=3000000,\n"
                         "small-max=2000, medium-max=20000. Exit 0 valid, 1 invalid, 2 bad options.\n";
            return 0;
        }
        if (option != "--group" && option != "--max" && option != "--max-n" &&
            option != "--max-m" && option != "--small-max" && option != "--medium-max")
            throw std::runtime_error("unknown option: " + option);
        if (i + 1 == argc) throw std::runtime_error("missing value after " + option);
        const auto value = argument_number(argv[++i], option.c_str());
        if (option == "--group") {
            if (value > 5) throw std::runtime_error("--group must be 0..5");
            group = static_cast<int>(value);
        } else {
            if (value == 0) throw std::runtime_error(option + " must be positive");
            if (option == "--max") n_max = m_max = value;
            if (option == "--max-n") n_max = value;
            if (option == "--max-m") m_max = value;
            if (option == "--small-max") small = value;
            if (option == "--medium-max") medium = value;
        }
    }
    if (small >= medium) throw std::runtime_error("--small-max must be below --medium-max");

    // A static buffer avoids a 1 MiB automatic object on Windows' default stack.
    static Reader input;
    try {
        const auto n = input.integer(1, n_max, "n");
        input.endline();
        for (std::uint64_t i = 0; i < n; ++i) {
            if (i) input.space();
            input.integer(0, MOD - 1, "a_i");
        }
        input.endline();
        const auto m = input.integer(1, m_max, "m");
        input.endline();
        std::uint64_t queries = 0, calibrations = 0;
        bool has_large_k = false;
        for (std::uint64_t i = 0; i < m; ++i) {
            const auto type = input.integer(1, 3, "operation type");
            input.space();
            const auto left = input.integer(1, n, "l");
            input.space();
            const auto right = input.integer(left, n, "r");
            if (type == 1) {
                if (right - left + 1 < 3) input.fail("calibration interval must have length at least 3");
                input.space();
                const auto k = input.integer(1, right - left - 1, "k");
                ++calibrations;
                has_large_k |= k > 1;
            } else if (type == 2) {
                input.space();
                input.integer(0, MOD - 1, "u");
                input.space();
                input.integer(0, MOD - 1, "v");
            } else {
                ++queries;
            }
            input.endline();
        }
        input.eof();
        if (queries == 0) input.fail("at least one operation 3 is required");
        const auto size = n > m ? n : m;
        const int actual = size <= small ? 1 : calibrations == 0 ? 2 :
                           !has_large_k ? 3 : size <= medium ? 4 : 5;
        if (group != 0 && group != actual)
            input.fail("input belongs to group " + std::to_string(actual) +
                       ", but --group " + std::to_string(group) + " was requested");
        std::cerr << "VALID group=" << actual << " n=" << n << " m=" << m
                  << " op1=" << calibrations << " queries=" << queries << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "INVALID " << error.what() << '\n';
        return 1;
    }
}
}  // namespace

int main(int argc, char** argv) {
    // Read CRLF literally under the Windows C runtime as well.
#ifdef _WIN32
    _setmode(0, _O_BINARY);
#endif
    try { return run(argc, argv); }
    catch (const std::exception& error) {
        std::cerr << "OPTIONS " << error.what() << '\n';
        return 2;
    }
}
