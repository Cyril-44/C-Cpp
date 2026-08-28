
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
constexpr LL MAXN = 2005;
LL TaskId, T;
LL N;
struct Node{
    LL Num;
    LL Id;
    LL A;
}F[MAXN];
LL A[MAXN][MAXN], A2[MAXN][MAXN];
LL Id[MAXN];
LL F2[MAXN];
LL B[MAXN];
bool Flag[MAXN];
bool Compare1(Node A, Node B) {
    return A.Num < B.Num;
}
void Input() {
    cin >> N;
    for(int i = 0; i <= N; i++) {
        Flag[i] = false;
    }
    for(int i = 1; i <= N; i++) {
        cin >> F[i].Num;
        F[i].Id = i;
        B[i] = -1;
    }
}
void Solve() {
    sort(F + 1, F + N + 1, Compare1);
    for(int i = 1; i <= N; i++) {
        if(F[i].Num != F[i - 1].Num) {
            B[i] = F[i - 1].Num;
        }
        Flag[F[i].Num] = true;
    }
    LL Cur = 1;
    for(int i = 1; i <= N; i++) {
        if(B[i] != -1) continue;
        while(Flag[Cur]) Cur++;
        B[i] = Cur;
        Flag[Cur] = true;
    }
    // for(int i = 1; i <= N; i++) {
    //     cerr << F[i].A << ' ';
    // }
    // cerr << '\n';
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= N; j++) {
            A[i][j] = B[min(i, j)];
        }
    }
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= N; j++) {
            A2[i][j] = A[F[i].Id][F[j].Id];
        }
    }
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= N; j++) {
            cout << A2[i][j] << ' ';
        }
        cout << '\n';
    }
}
int main() {
    cin >> TaskId >> T;
    for(int i = 1; i <= T; i++) {
        Input();
        Solve();
    }
    return 0;
}