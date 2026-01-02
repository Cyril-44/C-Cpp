#include <bits/extc++.h>
using namespace std;
const int N = 2000005;
using lint = long long;
using pi = pair<int, int>;
#define sz(v) ((int)(v).size())
#define all(v) (v).begin(), (v).end()

namespace hu_shing{
	struct Arc{
		int first, second;
		lint A, B;
		Arc(): first(0), second(0), A(0), B(0) {}
		Arc(int x, int y): first(x), second(y), A(0), B(0) {}
		bool operator<(const Arc &a)const{
			return (__int128)A * a.B < (__int128)B * a.A;
		}
		bool operator<=(const Arc &a)const{
			return (__int128)A * a.B <= (__int128)B * a.A;
		}
	};
    using pq_t = __gnu_pbds::priority_queue<Arc>;

	lint solve(vector<lint> w){
		int n = sz(w);
		if(n <= 2) return 0;
		// One sweep.
		vector<Arc> arcs;
		{
			vector<int> stk;
			rotate(w.begin(), min_element(all(w)), w.end());
			for(int i = 0; i < n; i++){
				while(sz(stk) >= 2 && w[stk.back()] > w[i]){
					arcs.emplace_back(stk[sz(stk) - 2], i);
					stk.pop_back();
				}
				stk.push_back(i);
			}
			for(int i = 2; i < sz(stk) - 1; i++){
				arcs.emplace_back(0, stk[i]);
			}
			if(sz(arcs) > n - 3) arcs.resize(n - 3);
		}
		// Build the tree.
		vector<vector<int>> gph(n - 2);
		{
			w.push_back(w[0]);
			arcs.emplace_back(0, n);
			sort(all(arcs), [&](const Arc &a, const Arc &b){
				return pi(a.first, -a.second) < pi(b.first, -b.second);
			});
			auto in = [&](Arc a, Arc b){
				return a.first <= b.first && b.second <= a.second;
			};
			vector<int> stk;
			for(int i = 0; i < sz(arcs); i++){
				while(sz(stk) && !in(arcs[stk.back()], arcs[i])){
					stk.pop_back();
				}
				if(sz(stk)){
					gph[stk.back()].push_back(i);
				}
				stk.push_back(i);
			}
		}
        
		// Calculate.
		lint ans = 0;
		{
			// Helpers.
			vector<lint> sum(n + 1);
			vector<vector<int>> left_idx(n + 1), right_idx(n + 1);
			vector<pq_t> pq(n - 2);
			for(int i = 1; i <= n; i++){
				sum[i] = sum[i-1] + w[i] * w[i-1];
			}
			auto consec = [&](Arc a){
				int x = a.first, y = a.second;
				return sum[y] - sum[x] - w[x] * w[y];
			};
			auto push = [&](int idx, Arc a){
				left_idx[a.first].push_back(a.second);
				right_idx[a.second].push_back(a.first);
                pq[idx].push(a);
			};
			auto pop = [&](int idx){
				Arc a = pq[idx].top();
                pq[idx].pop();
				left_idx[a.first].pop_back();
				right_idx[a.second].pop_back();
			};
			for(int i = n - 3; i >= 0; i--){
				if(sz(gph[i]) == 0){
					int pos = arcs[i].first;
					arcs[i].A = w[pos] * w[pos+1] * w[pos+2];
					arcs[i].B = consec(arcs[i]);
					ans += arcs[i].A;
					push(i, arcs[i]);
					continue;
				}
				arcs[i].B = consec(arcs[i]);
				for(auto &j : gph[i]){
					arcs[i].B -= consec(arcs[j]);
                    pq[i].join(pq[j]);
				}
				int x = arcs[i].first, y = arcs[i].second;
				int low = w[x] <= w[y] ? x : y;
				int lowp = (low == 0 ? n : low);
				while(!pq[i].empty() && pq[i].top().A >= w[low] * pq[i].top().B){
					ans -= pq[i].top().A;
					arcs[i].B += pq[i].top().B;
					pop(i);
				}
				arcs[i].A = arcs[i].B + w[x] * w[y];
				if(x == low) arcs[i].A -= w[x] * w[x + 1];
				if(y == lowp) arcs[i].A -= w[y] * w[y - 1];
				{
					int rmin = (sz(left_idx[low]) ? left_idx[low].back() : -1);
					int lmax = (sz(right_idx[lowp]) ? right_idx[lowp].back() : -1);
					if(~rmin && rmin <= y){
						arcs[i].A += w[low] * w[low + 1] - w[low] * w[rmin]; 
					}
					if(~lmax && lmax >= x){
						arcs[i].A += w[lowp] * w[lowp - 1] - w[low] * w[lmax];
					}
				}
				arcs[i].A *= w[low];
				ans += arcs[i].A;				
				while (!pq[i].empty() && arcs[i] <= pq[i].top()){
					arcs[i].A += pq[i].top().A;
					arcs[i].B += pq[i].top().B;;
					pop(i);
				}
				push(i, arcs[i]);
			}
		}
		return ans;
	}
}

int main(){
	int n;
	scanf("%d",&n);
	vector<lint> v(n+1);
	for(int i=0; i<=n; i++){
		scanf("%lld",&v[i]);
	}
	cout << hu_shing::solve(v) << endl;
}