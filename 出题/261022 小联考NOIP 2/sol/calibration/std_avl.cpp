// Independent full reference: stores a itself in a persistent AVL sequence.
// Local lazy maps are a[j] <- multiplier*a[j] + intercept.
// No difference array, difference moments, or boundary corrections are used.
// g++ -std=c++17 -O2 -DNDEBUG std_avl.cpp -o std_avl
// -DAVL_AUDIT validates all live nodes and reference counts after every operation.
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <vector>

namespace independent {
using Word = std::uint32_t;
using Wide = std::uint64_t;
constexpr Word MOD = 998244353;
Word plus(Word x, Word y) { const Word z = x + y; return z >= MOD ? z - MOD : z; }
Word minus(Word x, Word y) { return x >= y ? x - y : x + MOD - y; }
Word times(Word x, Word y) { return static_cast<Word>(Wide(x) * y % MOD); }

class Input {
    static constexpr size_t CAPACITY = 1 << 18;
    char bytes[CAPACITY];
    size_t at = 0, used = 0;
    int next_char() {
        if (at == used) {
            used = std::fread(bytes, 1, CAPACITY, stdin);
            at = 0;
            if (!used) return EOF;
        }
        return bytes[at++];
    }
public:
    Word next() {
        int c;
        do { c = next_char(); } while (c != EOF && c <= ' ');
        if (c == EOF) std::exit(2);
        Word value = 0;
        while ('0' <= c && c <= '9') {
            value = value * 10 + static_cast<Word>(c - '0');
            c = next_char();
        }
        return value;
    }
};

class Output {
    static constexpr size_t CAPACITY = 1 << 18;
    char bytes[CAPACITY];
    size_t used = 0;
public:
    void answer(Word value) {
        if (used + 16 >= CAPACITY) flush();
        char digits[16];
        int count = 0;
        do { digits[count++] = char('0' + value % 10); value /= 10; } while (value);
        while (count) bytes[used++] = digits[--count];
        bytes[used++] = '\n';
    }
    void flush() { std::fwrite(bytes, 1, used, stdout); used = 0; }
    ~Output() { flush(); }
};

class Sequence {
    struct Vertex {
        Word left, right, length, references, total;
        Word multiplier, intercept, height;
    };
    std::vector<Vertex> pool;
    Word free_slot = 0;
    Word alive = 0, maximum_alive = 0;
    Wide allocations = 0, reused = 0;

    Word allocate(Vertex value) {
        Word id;
        if (free_slot) {
            id = free_slot;
            free_slot = pool[id].left;
            pool[id] = value;
            ++reused;
        } else {
            id = static_cast<Word>(pool.size());
            pool.push_back(value);
        }
        ++allocations;
        maximum_alive = std::max(maximum_alive, ++alive);
        return id;
    }

    Word leaf(Word value) { return allocate({0, 0, 1, 1, value, 1, 0, 1}); }

    Word parent(Word left, Word right) {
        assert(left && right);
        const Vertex a = pool[left], b = pool[right];
        hold(left);
        hold(right);
        return allocate({left, right, a.length + b.length, 1, plus(a.total, b.total),
                         1, 0, 1 + std::max(a.height, b.height)});
    }

    std::pair<Word, Word> expose(Word root) {
        const Vertex v = pool[root];
        assert(v.length > 1);
        Word left = map(v.left, v.multiplier, v.intercept);
        Word right = map(v.right, v.multiplier, v.intercept);
        return {left, right};
    }

    // Inputs are borrowed; the returned root owns one reference.
    Word rebalance(Word left, Word right) {
        if (height(left) > height(right) + 1) {
            auto children = expose(left);
            const Word ll = children.first, lr = children.second;
            Word result;
            if (height(ll) >= height(lr)) {
                const Word lower = parent(lr, right);
                result = parent(ll, lower);
                drop(lower);
            } else {
                auto middle = expose(lr);
                const Word a = parent(ll, middle.first);
                const Word b = parent(middle.second, right);
                result = parent(a, b);
                drop(a); drop(b); drop(middle.first); drop(middle.second);
            }
            drop(ll); drop(lr);
            return result;
        }
        if (height(right) > height(left) + 1) {
            auto children = expose(right);
            const Word rl = children.first, rr = children.second;
            Word result;
            if (height(rr) >= height(rl)) {
                const Word lower = parent(left, rl);
                result = parent(lower, rr);
                drop(lower);
            } else {
                auto middle = expose(rl);
                const Word a = parent(left, middle.first);
                const Word b = parent(middle.second, rr);
                result = parent(a, b);
                drop(a); drop(b); drop(middle.first); drop(middle.second);
            }
            drop(rl); drop(rr);
            return result;
        }
        return parent(left, right);
    }

    Word build(const std::vector<Word> &values, Word begin, Word end) {
        if (end - begin == 1) return leaf(values[begin]);
        const Word middle = begin + (end - begin) / 2;
        const Word a = build(values, begin, middle), b = build(values, middle, end);
        const Word root = parent(a, b);
        drop(a); drop(b);
        return root;
    }

public:
    explicit Sequence(Word size) {
        pool.reserve(size_t(size) * 2 + 4096);
        pool.push_back({});
    }
    Word length(Word root) const { return root ? pool[root].length : 0; }
    Word height(Word root) const { return root ? pool[root].height : 0; }
    Word hold(Word root) { if (root) ++pool[root].references; return root; }

    void drop(Word root) {
        if (!root || --pool[root].references) return;
        const Word left = pool[root].left, right = pool[root].right;
        drop(left);
        drop(right);
        pool[root] = {};
        pool[root].left = free_slot;
        free_slot = root;
        --alive;
    }

    Word from_values(const std::vector<Word> &values) {
        return build(values, 0, static_cast<Word>(values.size()));
    }

    Word map(Word root, Word multiplier, Word intercept) {
        if (!root) return 0;
        if (multiplier == 1 && !intercept) return hold(root);
        Vertex v = pool[root];
        v.references = 1;
        v.total = plus(times(multiplier, v.total), times(intercept, v.length));
        if (v.length == 1) {
            v.multiplier = 1; v.intercept = 0;
        } else {
            v.multiplier = times(multiplier, v.multiplier);
            v.intercept = plus(times(multiplier, v.intercept), intercept);
        }
        hold(v.left); hold(v.right);
        return allocate(v);
    }

    Word concatenate(Word a, Word b) {
        if (!a) return hold(b);
        if (!b) return hold(a);
        if (height(a) > height(b) + 1) {
            auto children = expose(a);
            const Word combined = concatenate(children.second, b);
            const Word result = rebalance(children.first, combined);
            drop(children.first); drop(children.second); drop(combined);
            return result;
        }
        if (height(b) > height(a) + 1) {
            auto children = expose(b);
            const Word combined = concatenate(a, children.first);
            const Word result = rebalance(combined, children.second);
            drop(children.first); drop(children.second); drop(combined);
            return result;
        }
        return parent(a, b);
    }

    std::pair<Word, Word> split(Word root, Word prefix_length) {
        if (!prefix_length) return {0, hold(root)};
        if (prefix_length == length(root)) return {hold(root), 0};
        auto children = expose(root);
        const Word left = children.first, right = children.second, cut = length(left);
        Word first, second;
        if (prefix_length < cut) {
            auto parts = split(left, prefix_length);
            first = parts.first;
            second = concatenate(parts.second, right);
            drop(parts.second);
        } else if (prefix_length > cut) {
            auto parts = split(right, prefix_length - cut);
            first = concatenate(left, parts.first);
            second = parts.second;
            drop(parts.first);
        } else {
            return {left, right};
        }
        drop(left); drop(right);
        return {first, second};
    }

    Word extract(Word root, Word begin, Word count) {
        if (!begin && count == length(root)) return hold(root);
        auto children = expose(root);
        const Word left = children.first, right = children.second, cut = length(left);
        Word answer;
        if (begin + count <= cut) answer = extract(left, begin, count);
        else if (begin >= cut) answer = extract(right, begin - cut, count);
        else {
            const Word a = extract(left, begin, cut - begin);
            const Word b = extract(right, 0, begin + count - cut);
            answer = concatenate(a, b);
            drop(a); drop(b);
        }
        drop(left); drop(right);
        return answer;
    }

    Word replace(Word root, Word begin, Word end, Word replacement) {
        auto outer = split(root, end);
        auto inner = split(outer.first, begin);
        const Word first = concatenate(inner.first, replacement);
        const Word answer = concatenate(first, outer.second);
        drop(outer.first); drop(outer.second); drop(inner.first); drop(inner.second); drop(first);
        return answer;
    }

    Word affine(Word root, Word begin, Word end, Word multiplier, Word intercept) {
        if (multiplier == 1 && !intercept) return hold(root);
        if (!begin && end == length(root)) return map(root, multiplier, intercept);
        auto children = expose(root);
        Word left = children.first, right = children.second;
        const Word cut = length(left);
        if (begin < cut) {
            const Word changed = affine(left, begin, std::min(end, cut), multiplier, intercept);
            drop(left); left = changed;
        }
        if (end > cut) {
            const Word changed = affine(right, begin > cut ? begin - cut : 0, end - cut,
                                        multiplier, intercept);
            drop(right); right = changed;
        }
        const Word answer = parent(left, right);
        drop(left); drop(right);
        return answer;
    }

    Word sum(Word root, Word begin, Word end) const {
        const Vertex v = pool[root];
        if (!begin && end == v.length) return v.total;
        const Word cut = length(v.left);
        Word answer = 0;
        if (begin < cut) answer = sum(v.left, begin, std::min(end, cut));
        if (end > cut) answer = plus(answer, sum(v.right, begin > cut ? begin - cut : 0, end - cut));
        return plus(times(v.multiplier, answer), times(v.intercept, end - begin));
    }

    Word continuation(Word seed, Word desired, Word step) {
        Word block = hold(seed), copies = 1;
        while (length(block) < desired) {
            const Word shifted = map(block, 1, times(copies, step));
            const Word doubled = parent(block, shifted);
            drop(block); drop(shifted);
            block = doubled;
            copies *= 2;
        }
        if (length(block) == desired) return block;
        auto parts = split(block, desired);
        drop(block); drop(parts.second);
        return parts.first;
    }

#ifdef AVL_AUDIT
private:
    void audit_visit(Word id, std::vector<Word> &incoming, std::vector<unsigned char> &seen) const {
        if (seen[id]) return;
        seen[id] = 1;
        const Vertex v = pool[id];
        assert(v.length && v.references);
        if (v.length == 1) {
            assert(!v.left && !v.right && v.height == 1);
            assert(v.multiplier == 1 && !v.intercept);
            return;
        }
        assert(v.left && v.right);
        ++incoming[v.left]; ++incoming[v.right];
        audit_visit(v.left, incoming, seen);
        audit_visit(v.right, incoming, seen);
        const Vertex a = pool[v.left], b = pool[v.right];
        assert(a.height <= b.height + 1 && b.height <= a.height + 1);
        assert(v.height == 1 + std::max(a.height, b.height));
        assert(v.length == a.length + b.length);
        const Word wanted = plus(times(v.multiplier, plus(a.total, b.total)),
                                 times(v.intercept, v.length));
        assert(wanted == v.total);
    }
public:
    void audit(Word root) const {
        std::vector<Word> incoming(pool.size());
        std::vector<unsigned char> seen(pool.size());
        incoming[root] = 1;
        audit_visit(root, incoming, seen);
        Word count = 0;
        for (Word id = 1; id < pool.size(); ++id) {
            if (seen[id]) { assert(incoming[id] == pool[id].references); ++count; }
            else { assert(!pool[id].length && !pool[id].references); }
        }
        assert(count == alive && count <= 2 * length(root) - 1);
        for (Word id = free_slot; id; id = pool[id].left) {
            assert(!seen[id]); seen[id] = 2;
            ++count;
        }
        assert(size_t(count) + 1 == pool.size());
    }
#endif

    void profile() const {
#ifdef PROFILE
        std::fprintf(stderr, "{\"reference\":\"direct-array-avl\",\"node_bytes\":%zu,"
                     "\"slots\":%zu,\"peak_live\":%u,\"live\":%u,"
                     "\"allocations\":%llu,\"reused\":%llu}\n", sizeof(Vertex), pool.size() - 1,
                     maximum_alive, alive, static_cast<unsigned long long>(allocations),
                     static_cast<unsigned long long>(reused));
#endif
    }
};
}  // namespace independent

int main() {
    using namespace independent;
    static Input input;
    static Output output;
    const Word n = input.next();
    std::vector<Word> values(n);
    for (Word &value : values) value = input.next();
    Sequence sequence(n);
    Word root = sequence.from_values(values);
    std::vector<Word>().swap(values);
    const Word m = input.next();
    for (Word operation = 0; operation < m; ++operation) {
        const Word kind = input.next(), l = input.next(), r = input.next();
        if (kind == 1) {
            const Word k = input.next();
            if (r - l != k) {
                const Word a_l = sequence.sum(root, l - 1, l);
                const Word a_lk = sequence.sum(root, l + k - 1, l + k);
                const Word step = minus(a_lk, a_l);
                const Word seed = sequence.extract(root, l - 1, k);
                const Word repeated = sequence.continuation(seed, r - l + 1, step);
                const Word next = sequence.replace(root, l - 1, r, repeated);
                sequence.drop(root); sequence.drop(seed); sequence.drop(repeated);
                root = next;
            }
        } else if (kind == 2) {
            const Word u = input.next(), v = input.next();
            const Word next = sequence.affine(root, l - 1, r, u, v);
            sequence.drop(root);
            root = next;
        } else {
            output.answer(sequence.sum(root, l - 1, r));
        }
#ifdef AVL_AUDIT
        sequence.audit(root);
#endif
    }
    output.flush();
    sequence.profile();
    sequence.drop(root);
    return 0;
}
