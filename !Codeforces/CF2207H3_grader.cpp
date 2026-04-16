#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int type; // 0 leaf, 1 min, 2 max
    int var = -1;
    vector<Node*> ch;
};

vector<string> tok;
int ptr;

vector<string> split(const string& s) {
    vector<string> res;
    string cur;
    stringstream ss(s);
    while (ss >> cur) res.push_back(cur);
    return res;
}

Node* parse() {
    if (ptr >= (int)tok.size())
        quitf(_fail, "parser out of bounds");

    string s = tok[ptr++];

    if (isdigit(s[0])) {
        return new Node{0, stoi(s) - 1, {}};
    }

    Node* u = new Node;
    u->type = (s == "min" ? 1 : 2);

    if (ptr >= (int)tok.size() || tok[ptr] != "(")
        quitf(_fail, "expected (");
    ptr++;

    while (true) {
        u->ch.push_back(parse());

        if (ptr >= (int)tok.size())
            quitf(_fail, "unexpected EOF");

        if (tok[ptr] == ")") {
            ptr++;
            break;
        }

        if (tok[ptr] != ",")
            quitf(_fail, "expected comma");

        ptr++;
    }

    return u;
}

int eval(Node* u, const vector<int>& x) {
    if (u->type == 0) return x[u->var];

    if (u->type == 1) {
        int res = INT_MAX;
        for (auto v : u->ch)
            res = min(res, eval(v, x));
        return res;
    }

    int res = INT_MIN;
    for (auto v : u->ch)
        res = max(res, eval(v, x));
    return res;
}

int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    int t = inf.readInt();
    inf.readEoln();
    cout << t << endl;

    int totalQ = 0;

    for (int tc = 0; tc < t; tc++) {
        int n = inf.readInt();
        inf.readEoln();
        cout << n << endl;

        string expr = inf.readLine();
        tok = split(expr);

        ptr = 0;
        Node* root = parse();

        while (true) {
            string op = ouf.readToken();

            if (op == "?") {
                totalQ++;
                if (totalQ > 1500)
                    quitf(_wa, "too many queries");

                vector<int> x(n);
                for (int i = 0; i < n; i++)
                    x[i] = ouf.readInt(-1000000000, 1000000000);

                cout << eval(root, x) << endl;
                cout.flush();
            }
            else if (op == "!") {
                // 模拟若干组在线询问
                int q = 10;

                for (int i = 0; i < q; i++) {
                    vector<int> x(n);
                    for (int j = 0; j < n; j++)
                        x[j] = rnd.next(1, 20);

                    for (int j = 0; j < n; j++) {
                        if (j) cout << ' ';
                        cout << x[j];
                    }
                    cout << endl;
                    cout.flush();

                    int ans = ouf.readInt();
                    int cor = eval(root, x);

                    if (ans != cor)
                        quitf(_wa,
                              "wrong answer on query %d: expected %d got %d",
                              i + 1, cor, ans);
                }

                cout << 0 << endl;
                cout.flush();
                break;
            }
            else {
                quitf(_wa, "expected ? or !");
            }
        }
    }

    quitf(_ok, "accepted");
}