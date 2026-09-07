#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, d;
    if (!(cin >> n >> d)) return 0;

    struct Message {
        int a, b, c;
    };
    vector<Message> msgs(n);
    for (int i = 0; i < n; ++i) {
        cin >> msgs[i].a >> msgs[i].b >> msgs[i].c;
    }

    vector<int> w(d + 1);
    for (int i = 1; i <= d; ++i) {
        cin >> w[i];
    }

    int success_count = 0;
    for (int i = 0; i < n; ++i) {
        bool caught = false;
        int start = msgs[i].a;
        int end = msgs[i].a + msgs[i].b - 1;
        
        // 检查在消息持续期间，老师是否出现在该排
        for (int t = start; t <= end; ++t) {
            if (w[t] == msgs[i].c) {
                caught = true;
                break;
            }
        }
        if (!caught) {
            success_count++;
        }
    }

    cout << success_count << endl;

    return 0;
}