#include <bits/stdc++.h>
using namespace std;

const long long N = 1000000000LL;

// 随机生成敌方基地
struct Base {
    long long x1, y1, x2, y2;
};

Base generate_base() {
    // 为了测试方便，这里生成一个小矩形
    long long x1 = 2 + rand() % 1000000;
    long long y1 = 2 + rand() % 1000000;
    long long x2 = x1 + 1000 + rand() % 1000;
    long long y2 = y1 + 1000 + rand() % 1000;
    // return {x1, y1, x2, y2};
    return {2, 3, 4, 5};
}

bool inside_base(long long x, long long y, const Base &b) {
    return (b.x1 <= x && x <= b.x2 && b.y1 <= y && y <= b.y2);
}

long long manhattan_distance(long long x, long long y, const Base &b) {
    long long dx = 0, dy = 0;
    if (x < b.x1) dx = b.x1 - x;
    else if (x > b.x2) dx = x - b.x2;
    if (y < b.y1) dy = b.y1 - y;
    else if (y > b.y2) dy = y - b.y2;
    return dx + dy;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    srand(time(nullptr));
    Base base = generate_base();

    cerr << "[DEBUG] Enemy base: "
         << base.x1 << " " << base.y1 << " "
         << base.x2 << " " << base.y2 << "\n";

    string cmd;
    int cnt = 0;
    while (cin >> cmd) {
        if (cmd == "?") {
            ++cnt;
            long long x, y;
            cin >> x >> y;
            if (x < 1 || x > N || y < 1 || y > N || inside_base(x, y, base)) {
                cout << -1 << "\n";
                cout.flush();
                cerr << "Fuck you!";
                return 0; // 游戏结束
            } else {
                cout << manhattan_distance(x, y, base) << "\n";
                cout.flush();
            }
        } else if (cmd == "!") {
            long long x, y, p, q;
            cin >> x >> y >> p >> q;
            if (x == base.x1 && y == base.y1 && p == base.x2 && q == base.y2) {
                cerr << "Used: " << cnt << "\n";
                if (cnt <= 40) cerr << "Correct!\n";
                else cerr << "Out of limit!\n";
            } else {
                cerr << "Wrong!\n" << x << ' ' << y << ' ' << p << ' ' << q << '\n';
            }
            cout.flush();
            return 0;
        }
    }
    return 0;
}
