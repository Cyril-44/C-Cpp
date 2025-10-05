#include "testlib.h"
#include <bits/stdc++.h>
constexpr int TestCases = 20;
constexpr std::pair<int,int> N[] = {
    {25, 50}, {25, 50}, {45, 50}, // Subtask 1
    {500, 1000}, {800, 1000}, {1000, 1000}, {1000, 1000}, // Subtask 2
    {500, 1000}, {500, 1000}, {500, 1000}, // Subtask 3
    {800, 1000}, {800, 1000}, {800, 1000}, {800, 1000}, {800, 1000}, 
    {100, 1000}, {1000, 1000}, {1000, 1000}, {1000, 1000}, {1000, 1000} // Subtask 4
};
using FuncType = std::vector<int>(*)(int);
const FuncType AGen[] = {
    [](int n) -> std::vector<int> {
        std::vector<int> a(n);
        for (int i = 0; i < n; i++)
            a[i] = rnd.next(0, 3000);
        return a;
    }, 
    [](int n) -> std::vector<int> {
        std::vector<int> a(n);
        std::vector<int> b(50);
        std::set<int> vis;
        for (int i = 0, tp; i < 50; i++) {
            do tp = rnd.next(0, 500000);
            while (vis.count(tp));
            vis.insert(tp);
            b[i] = tp;
        }
        for (int i = 0; i < n; i++) {
            a[i] = b[rnd.next(0, 49)];
        }
        return a;
    },
    [](int n) -> std::vector<int> {
        std::vector<int> a(n);
        for (int i = 0; i < n; i++)
            a[i] = rnd.next(0, 500000);
        return a;
    }
};
const FuncType A[] = {
    AGen[2], AGen[2], AGen[2], // Subtask 1
    AGen[1], AGen[1], AGen[1], AGen[1], // Subtask 2
    AGen[0], AGen[0], AGen[0], // Subtask 3
    AGen[2], AGen[2], AGen[2], AGen[2], AGen[2], 
    AGen[2], AGen[2], AGen[2], AGen[2], AGen[2] // Subtask 4
};
void printTestCase(int testCase, const std::vector<std::vector<int>>& a) {
    ++testCase;
    std::ofstream os("sequence" + std::to_string(testCase) + ".in");
    os << a.size() << std::endl;
    for (auto b : a) {
        os << b.size() << std::endl;
        for (int c : b) os << c << ' ';
        os << std::endl;
    }
    os.close();
}
int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    for (int i = 0; i < TestCases; i++) {
        int t = rnd.next(1, 10);
        std::vector<std::vector<int>> data(t);
        for (int j = 0; j < t; j++) {
            int n = rnd.next(N[i].first, N[i].second);
            data[j] = A[i](n);
        }
        printTestCase(i, data);
    }
    return 0;
}