#include <cstdio>
#include <cmath>
#include <vector>
#include <cstring>
constexpr int N = 8869, B=3*3*3*3*3;
struct Word {
    char w[5];
    int operator[](int p) const { return w[p] - 'a'; }
    Word() : w{} {}
    Word(char *s) { memcpy(w, s, 5); }
    inline void puts() { putchar('"'); for (char ch : w) putchar(ch); putchar('"'), putchar(','); }
};
char scram[N][6];
std::vector<Word> words;
inline int simuGuess(const Word& g, const Word& t) {
    int stag[26]{}, stat[26]{}, res[5]{};
    for (int i = 0; i < 5; i++)
        ++stag[g[i]], ++stat[t[i]];
    for (int i = 0; i < 5; i++) {
        if (g[i] == t[i]) res[i] = 2, --stat[g[i]];
        else if (stat[g[i]]) res[i] = 1, --stat[g[i]];
        else res[i] = 0;
    }
    int fin = 0;
    for (int i = 0; i < 5; i++)
        fin = fin * 3 + res[i];
    return fin;
}
inline double entropy(const Word& word) {
    int cnt[B]{};
    for (const Word& w : words) ++cnt[simuGuess(word, w)];
    double e = 0;
    for (int s = 0; s < B; s++) {
        double p = cnt[s] / (double)words.size();
        if (p != 0) e += p * -log2(p);
    }
    return e;
}
int main() {
    for (char *word : scram) scanf("%5s", word);
    for (char beg = 'a'; beg <= 'z'; beg++) {
        words.clear();
        for (char *word : scram) if (*word == beg) words.emplace_back(word);
        double maxEntropy = 0;
        Word best;
        for (char *word : scram) {
            double en = entropy(word);
            if (en > maxEntropy) {
                maxEntropy = en, best = word;
            }
        }
        fprintf(stderr, "Finished '%c', with best Entropy = %g\n", beg, maxEntropy);
        best.puts();
    }
    return 0;
}