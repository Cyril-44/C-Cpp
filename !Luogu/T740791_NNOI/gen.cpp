#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int randint(int l, int r) {
    return l + rng() % (r - l + 1);
}

vector<int> gen_perm(int n) {
    vector<int> a(n);
    iota(a.begin(), a.end(), 0);
    shuffle(a.begin(), a.end(), rng);
    return a;
}

vector<int> gen_by_mode(int n, int mode) {
    vector<int> a(n);
    if (mode == 1) {
        iota(a.begin(), a.end(), 0);
    } else if (mode == 2) {
        for (int i = 0; i < n; i++) a[i] = n - 1 - i;
    } else if (mode == 3) {
        a = gen_perm(n);
    } else if (mode == 4) {
        int k = randint(1, n - 1);
        vector<int> b(n);
        iota(b.begin(), b.end(), 0);
        shuffle(b.begin(), b.begin() + k, rng);
        shuffle(b.begin() + k, b.end(), rng);
        a = b;
    } else if (mode == 5) {
        // 奇偶交错
        vector<int> even, odd;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) even.push_back(i);
            else odd.push_back(i);
        }
        shuffle(even.begin(), even.end(), rng);
        shuffle(odd.begin(), odd.end(), rng);
        int p = 0;
        for (int x : even) a[p++] = x;
        for (int x : odd) a[p++] = x;
    } else {
        // fallback
        a = gen_perm(n);
    }
    return a;
}

vector<array<int,3>> gen_ops(int n, int m, bool allow_swap) {
    vector<array<int,3>> ops;
    ops.reserve(m);
    for (int i = 0; i < m; i++) {
        int type = allow_swap ? randint(1,2) : 2;
        int x = randint(1,n);
        int y = randint(1,n);
        ops.push_back({type, x, y});
    }
    return ops;
}

void print_case(int n, int m, vector<int> a, vector<array<int,3>> ops) {
    cout << n << " " << m << "\n";
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i+1==n];
    for (auto &op : ops) {
        cout << op[0] << " " << op[1] << " " << op[2] << "\n";
    }
}

int main(int argc, char ** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (argc != 3) return puts("Usage: ./std TestCaseID Mode"), 1;
    int tp = atoi(argv[1]), mode = atoi(argv[2]);
    // mode = -1 表示随机模式

    int n, m;
    vector<int> a;
    vector<array<int,3>> ops;

    auto pick_mode = [&](int default_cnt) {
        if (mode != -1) return mode;
        return randint(1, default_cnt);
    };

    if (tp == 1) {
        n = randint(1,50);
        m = randint(1,50);
        a = gen_by_mode(n, pick_mode(5));
        ops = gen_ops(n,m,true);
    }
    else if (tp == 2) {
        n = randint(990,1000);
        m = randint(990,1000);
        a = gen_by_mode(n, pick_mode(5));
        ops = gen_ops(n,m,true);
    }
    else if (tp == 3) {
        n = randint(990,1000);
        m = randint(990,1000);
        a = gen_by_mode(n, pick_mode(5));
        ops = gen_ops(n,m,false);  // no swap
    }
    else if (tp == 4) {
        n = randint(990,1000);
        m = randint(990,1000);
        a = gen_by_mode(n, pick_mode(5));
        // force a[0] = 0
        int pos = find(a.begin(), a.end(), 0) - a.begin();
        swap(a[0], a[pos]);
        ops = gen_ops(n,m,true);
    }
    else {
        n = randint(999990,1000000);
        m = randint(999990,1000000);
        a = gen_by_mode(n, pick_mode(5));
        ops = gen_ops(n,m,true);
    }

    print_case(n,m,a,ops);
    return 0;
}
