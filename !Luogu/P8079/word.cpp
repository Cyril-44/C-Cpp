#include "word.h"
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
constexpr int B = 3*3*3*3*3, M=26*26*26*26*26;
constexpr double EP = 0.1;
static const char bests[26][6] = {
    "slier","lares","lares","tores","tarns","arles","lares","lares","snare","ousel","ranis","nares","tares","aides","tries","lares","raise","aides","plate","nares","snare","riles","nares","cones","kanes","aeons"
};
struct Word {
    char w[5];
    int operator[](int p) const { return w[p] - 'a'; }
    Word() : w{} {}
    Word(const char *s) { memcpy(w, s, 5); }
    void puts() const { for (char ch : w) fputc(ch, stderr); fputc('\n', stderr); }
    uint32_t hash() const {
        uint32_t res = 0;
        for (char ch : w) res = res * 26 + ch;
        return res;
    }
};
static std::vector<Word> wordsBegWith[26], allWords;
static std::unordered_set<uint32_t> existance, exBegWith[26];
static std::vector<Word> currentWordSet;
inline int simuGuess(const Word& g, const Word& t) {
    uint32_t tbits = 0;
    for (int i = 0; i < 5; i++) tbits |= 1 << t[i];
    int res = 0;
    for (int i = 0; i < 5; i++) {
        int curbit;
        if (g[i] == t[i]) curbit = 2;
        else if (tbits >> g[i] & 1) curbit = 1;
        else curbit = 0;
        res = res * 3 + curbit;
    }
    return res;
}
static char retWord[6];
static Word lastWord;
inline const char* returnWord(const Word &word) {
    lastWord = word;
    memcpy(retWord, word.w, 5);
    return retWord;
}
inline double getEntropy(const Word &word, int r) {
    int cnt[B]{};
    for (const Word &candidate : currentWordSet)
        ++cnt[simuGuess(word, candidate)];
    double entropy = existance.count(word.hash()) * EP * (5 - r);
    for (int s = 0; s < B; s++) if (cnt[s]) {
        double p = cnt[s] / (double)currentWordSet.size();
        entropy += p * -log2(p);
    }
    return entropy;
}
void init(int num_scramble, const char *scramble) {
    for (int i = 0; i < num_scramble; i++) {
        wordsBegWith[scramble[i*5] - 'a'].emplace_back(scramble + i*5);
        allWords.emplace_back(scramble + i*5);
        exBegWith[scramble[i*5] - 'a'].insert(allWords.back().hash());
    }
}
const char *guess(int num_testcase, int remaining_guesses, char initial_letter, bool *gold, bool *silver) {
    if (remaining_guesses == 5) { // New round
        currentWordSet = wordsBegWith[initial_letter - 'a'];
        existance = exBegWith[initial_letter - 'a'];
        return returnWord(bests[initial_letter - 'a']);
    }
        uint32_t resMask = 0, banMask = 0;
        for (int i = 0; i < 5; i++)
            if (!gold[i]) (silver[i] ? resMask : banMask) |= 1u << lastWord[i];
        std::vector<Word> newWordSet;
        for (const Word& word : currentWordSet) {
            bool flg = true;
            uint32_t curMask = 0;
            for (int i = 0; i < 5; i++) {
                if (gold[i]) {
                    if (lastWord[i] != word[i])
                        { flg = false; break; }
                } else {
                    curMask |= 1u << word[i];
                    if (lastWord[i] == word[i] || (banMask >> word[i] & 1))
                        { flg = false; break; }
                }
            }
            if ((curMask & resMask) != resMask) flg = false;
            if (flg) newWordSet.push_back(word);
            else existance.erase(word.hash());
        }
        currentWordSet.swap(newWordSet);
    Word best; double maxEntropy = 0;
    for (const Word& word : allWords) {
        double currentEntropy = getEntropy(word, remaining_guesses);
        if (currentEntropy > maxEntropy) {
            maxEntropy = currentEntropy;
            best = word;
        }
    }
    // fprintf(stderr, "maxEntropy: %g, guessing ", maxEntropy); best.puts();
    return returnWord(best);
}

