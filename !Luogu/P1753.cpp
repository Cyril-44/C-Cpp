#include <bits/extc++.h>
using namespace std;
const int N = 2000005;
using LL = long long;
using Pii = pair<int, int>;
#define sz(v) ((int)(v).size())
#define all(v) (v).begin(), (v).end()
LL w[N], sum[N];
vector<int> g[N], lp[N], rp[N];
int n;
namespace Hu_Shing {
	struct Arc {
		int x, y;
		LL s, h;
		Arc(): x(0), y(0), s(0), h(0) {}
		Arc(int x, int y): x(x), y(y), s(0), h(0) {}
		bool operator<(const Arc &a)const{
			return (__int128)s * a.h < (__int128)h * a.s;
		}
		bool operator<=(const Arc &a)const{
			return (__int128)s * a.h <= (__int128)h * a.s;
		}
	};
    using pq_t = __gnu_pbds::priority_queue<Arc>;

	LL solve() {
        ++n;
		vector<Arc> arcs;
        vector<int> sta;
        rotate(w, min_element(w, w+n), w+n);
        for(int i = 0; i < n; i++) {
            while(sz(sta) >= 2 && w[sta.back()] > w[i]){
                arcs.emplace_back(sta[sz(sta) - 2], i);
                sta.pop_back();
            }
            sta.push_back(i);
        }
        for(int i = 2; i < sz(sta) - 1; i++){
            arcs.emplace_back(0, sta[i]);
        }
        if(sz(arcs) > n - 3) arcs.resize(n - 3); // 无论如何先贪心选 n-3 条弧。

        w[n] = w[0];
        arcs.emplace_back(0, n);
        sort(all(arcs), [&](const Arc &a, const Arc &b){
            return Pii(a.x, -a.y) < Pii(b.x, -b.y);
        });
        auto in = [&](Arc a, Arc b){ return a.x <= b.x && b.y <= a.y; };
        stack<int, vector<int>> stk;
        for(int i = 0; i < sz(arcs); i++){
            while (sz(stk) && !in(arcs[stk.top()], arcs[i])) stk.pop();
            if (sz(stk)) g[stk.top()].push_back(i);
            stk.push(i);
        }
        
		// Calculate.
		LL ans = 0;
        vector<pq_t> pq(n - 2);
        for(int i = 1; i <= n; i++)
            sum[i] = sum[i-1] + w[i] * w[i-1];
        auto consec = [](const Arc &a){return sum[a.y] - sum[a.x] - w[a.x] * w[a.y];};
        auto push = [&](int id, const Arc &a){
            lp[a.x].push_back(a.y);
            rp[a.y].push_back(a.x);
            pq[id].push(a);
        };
        auto pop = [&](int id){
            Arc a = pq[id].top();
            pq[id].pop();
            lp[a.x].pop_back();
            rp[a.y].pop_back();
        };
        for(int i = n - 3; i >= 0; i--){
            if(sz(g[i]) == 0){
                int pos = arcs[i].x;
                arcs[i].s = w[pos] * w[pos+1] * w[pos+2];
                arcs[i].h = consec(arcs[i]);
                ans += arcs[i].s;
                push(i, arcs[i]);
                continue;
            }
            arcs[i].h = consec(arcs[i]);
            for(auto &j : g[i]){
                arcs[i].h -= consec(arcs[j]);
                pq[i].join(pq[j]);
            }
            int x = arcs[i].x, y = arcs[i].y;
            int low = w[x] <= w[y] ? x : y;
            int lowp = (low == 0 ? n : low);
            while(!pq[i].empty() && pq[i].top().s >= w[low] * pq[i].top().h){
                ans -= pq[i].top().s;
                arcs[i].h += pq[i].top().h;
                pop(i);
            }
            arcs[i].s = arcs[i].h + w[x] * w[y];
            if(x == low) arcs[i].s -= w[x] * w[x + 1];
            if(y == lowp) arcs[i].s -= w[y] * w[y - 1];
            int rmin = (sz(lp[low]) ? lp[low].back() : -1);
            int lmax = (sz(rp[lowp]) ? rp[lowp].back() : -1);
            if(~rmin && rmin <= y)
                arcs[i].s += w[low] * w[low + 1] - w[low] * w[rmin]; 
            if(~lmax && lmax >= x)
                arcs[i].s += w[lowp] * w[lowp - 1] - w[low] * w[lmax];
            arcs[i].s *= w[low];
            ans += arcs[i].s;				
            while (!pq[i].empty() && arcs[i] <= pq[i].top()){
                arcs[i].s += pq[i].top().s;
                arcs[i].h += pq[i].top().h;
                pop(i);
            }
            push(i, arcs[i]);
        }
		return ans;
	}
}

int main(){
	scanf("%d",&n);
	for(int i=0; i<=n; i++)
		scanf("%lld",&w[i]);
	cout << Hu_Shing::solve() << endl;
}