#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// 计算阶乘
int factorial(int n) {
    return (n == 0 || n == 1) ? 1 : n * factorial(n - 1);
}

// 计算对应关系的数量
int countCorrespondences(const string& A, const string& B) {
    if (A.length() != B.length()) return 0;

    unordered_map<char, int> freqA, freqB;
    for (char c : A) freqA[c]++;
    for (char c : B) freqB[c]++;

    vector<int> freqCountA, freqCountB;
    for (const auto& pair : freqA) freqCountA.push_back(pair.second);
    for (const auto& pair : freqB) freqCountB.push_back(pair.second);

    sort(freqCountA.begin(), freqCountA.end());
    sort(freqCountB.begin(), freqCountB.end());

    if (freqCountA != freqCountB) return 0;

    unordered_map<int, int> freqCountMap;
    for (int count : freqCountA) freqCountMap[count]++;

    int result = 1;
    for (const auto& pair : freqCountMap) {
        result *= factorial(pair.second);
    }

    return result;
}

int main() {
    string A, B;
    cin >> A >> B;

    int result = countCorrespondences(A, B);
    cout << result << endl;

    return 0;
}
