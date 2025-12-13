#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const unsigned long long MOD = 1e18 + 7; // 更大的素数
const unsigned long long BASE = 31;
const unsigned long long PRIME = 37;

vector<vector<int>> tree;
vector<int> subtree_size;

unsigned long long qpow(unsigned long long base, int exp, unsigned long long mod) {
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int compute_subtree_size(int node, int parent) {
    int size = 1;
    for (int neighbor : tree[node]) {
        if (neighbor != parent) {
            size += compute_subtree_size(neighbor, node);
        }
    }
    return subtree_size[node] = size;
}

unsigned long long compute_hash(int node, int parent, int depth) {
    vector<unsigned long long> child_hashes;
    for (int neighbor : tree[node]) {
        if (neighbor != parent) {
            child_hashes.push_back(compute_hash(neighbor, node, depth + 1));
        }
    }
    sort(child_hashes.begin(), child_hashes.end());
    unsigned long long hash_value = qpow(PRIME, depth, MOD);
    for (unsigned long long h : child_hashes) {
        hash_value = (hash_value * BASE + h) % MOD;
    }
    return hash_value;
}

bool is_symmetric_tree(int n, const vector<pair<int, int>>& edges) {
    tree.assign(n + 1, vector<int>());
    subtree_size.assign(n + 1, 0);
    for (const auto& edge : edges) {
        int u = edge.first, v = edge.second;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    compute_subtree_size(1, -1);

    for (const auto& edge : edges) {
        int u = edge.first, v = edge.second;
        if (subtree_size[u] == n / 2 || subtree_size[v] == n / 2) {
            unsigned long long hash_u = compute_hash(u, v, 1);
            unsigned long long hash_v = compute_hash(v, u, 1);
            if (hash_u == hash_v) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    int T;
    cin >> T;
    vector<string> results;
    for (int t = 0; t < T; ++t) {
        int n;
        cin >> n;
        vector<pair<int, int>> edges(n - 1);
        for (int i = 0; i < n - 1; ++i) {
            cin >> edges[i].first >> edges[i].second;
        }
        results.push_back(is_symmetric_tree(n, edges) ? "Yes" : "No");
    }

    for (const string& result : results) {
        cout << result << endl;
    }

    return 0;
}
