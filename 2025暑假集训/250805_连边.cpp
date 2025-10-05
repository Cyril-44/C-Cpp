#include <iostream>
#include <vector>
#include <queue>
using namespace std;
typedef pair<int,int> pii;

// Check if point x lies strictly on the clockwise arc from a to b on a circle of size N
bool onClockwiseArc(int a, int b, int x, int N) {
    if (a < b) {
        return (a < x && x < b);
    } else {
        // wraps around
        return (x > a || x < b);
    }
}

// Check if chords (a,b) and (c,d) cross on the circle
bool chordsCross(int a, int b, int c, int d, int N) {
    bool f = onClockwiseArc(a, b, c, N) ^ onClockwiseArc(a, b, d, N);
    bool g = onClockwiseArc(c, d, a, N) ^ onClockwiseArc(c, d, b, N);
    return f && g;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int N, M;
        cin >> N >> M;
        
        vector<pii> edges(M);
        for (int i = 0; i < M; i++) {
            int x, y;
            cin >> x >> y;
            edges[i] = make_pair(x, y);
        }
        
        // Build intersection graph: two edges are adjacent if they cross
        vector<vector<int> > adj(M);
        for (int i = 0; i < M; i++) {
            int a = edges[i].first, b = edges[i].second;
            for (int j = i + 1; j < M; j++) {
                int c = edges[j].first, d = edges[j].second;
                if (chordsCross(a, b, c, d, N)) {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }
        
        // Try 2-coloring the intersection graph
        vector<int> color(M, -1);
        bool ok = true;
        
        for (int i = 0; i < M && ok; i++) {
            if (color[i] != -1) 
                continue;
            
            // BFS from i
            queue<int> q;
            color[i] = 0;
            q.push(i);
            while (!q.empty() && ok) {
                int u = q.front(); 
                q.pop();
                for (size_t k = 0; k < adj[u].size(); ++k) {
                    int v = adj[u][k];
                    if (color[v] == -1) {
                        color[v] = color[u] ^ 1;
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        ok = false;
                        break;
                    }
                }
            }
        }
        
        cout << (ok ? "YES\n" : "NO\n");
    }

    return 0;
}
