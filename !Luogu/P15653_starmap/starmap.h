#ifndef STARMAP_H
#define STARMAP_H

#include <vector>

void init(int c, int t);
void starmap(int n, int m, int k, int p, std::vector<int> u,
    std::vector<int> v);
void report(int c);
void invert(std::vector<int> s);

#endif // STARMAP_H
