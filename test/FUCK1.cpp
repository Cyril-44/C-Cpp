#include <vector>
#include <algorithm>
class Matrix {
    int rows, cols;
    std::vector< std::vector<int> > a;
public:
    inline int get(const int &row, const int &col) const {
        return a[row][col];
    }
    inline void set(const int &row, const int &col, const int &value) {
        if (row >= a.size()) a.resize(row);
        if (col >= a[0].size()) {
            for (int i = 0; i < a.size(); i++)
                a[i].resize(col);
        }
        a[row][col] = value;
    }
    inline void add(const Matrix& other) {
        if (a.empty() && other.a.empty()) return;
        a.resize(std::max(other.a.size(), a.size()));
        register int subsize = std::max(a[0].size(), other.a[0].size());
        for (int i = 0; i < a.size(); i++) {
            a[i].resize(subsize);
            for (int j = 0; j < subsize; j++)
                a[i][j] += other.a[i][j];
        }
    }
};