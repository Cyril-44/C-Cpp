#include <cstdio>
#include <list>
#include <algorithm>
const int N = 1000005;
std::list<int> a;
int main() {
    std::list<int>::iterator it;
    static int sta[N];
    static int stap[N];
    int n;
    scanf("%d", &n);
    char c; int x, p(0), top(0), sum(0);
    const auto push = [&] (const int &x, const int &p) {
        if (!top || sta[top] <= x) {
            ++top;
            sta[top] = x;
            stap[top] = p;
        }
    };
    while (n--) {
        scanf(" %c", &c);
        switch (c) {
        case 'I':
            scanf("%d", &x);
            if (a.empty()) {
                a.emplace_back(x);
                it = a.end();
            }
            else a.insert(it, x);
            sum += x, ++p;
            push(sum, p);
            break;
        case 'L':
            --p, sum -= *(--it);
            if (stap[top] > p) --top;
            break;
        case 'R':
            ++p, sum += *(it++);
            push(sum, p);
            break;
        case 'D':
            --p, sum -= *(--it);
            it = a.erase(it);
            if (stap[top] > p) --top;
            break;
        case 'Q':
            scanf("%d", &x);
            printf("%d\n", sta[std::upper_bound(stap+1, stap+1 + top, x) - stap - 1]);
            break;
        }
    }
    return 0;
}