#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
const int MOD = 1000000007;

// 加法模
inline int addmod(int a, int b){ a+=b; if(a>=MOD) a-=MOD; return a; }
// 乘法模
inline int mulmod(int a, int b){ return int((int64)a * b % MOD); }

int main(){
    freopen("snake.in", "r", stdin);
    freopen("snake.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin>>T)) return 0;
    while(T--){
        int n,k;
        string a;
        cin>>n>>k>>a;
        vector<int> s(n);
        for(int i=0;i<n;++i){
            if(a[i]=='o') s[i]=0;
            else if(a[i]=='x') s[i]=1;
            else s[i]=-1;
        }
        // 前缀计数 o / x / ?
        vector<int> p0(n+1,0), p1(n+1,0), pq(n+1,0);
        for(int i=0;i<n;++i){
            p0[i+1]=p0[i] + (s[i]==0);
            p1[i+1]=p1[i] + (s[i]==1);
            pq[i+1]=pq[i] + (s[i]==-1);
        }
        auto cnt0 = [&](int l,int r){ return p0[r+1]-p0[l]; };
        auto cnt1 = [&](int l,int r){ return p1[r+1]-p1[l]; };
        auto cntq = [&](int l,int r){ return pq[r+1]-pq[l]; };

        int maxDel = n / k;
        // add[pos] 存放延迟合并到 pos 的若干 (del, ways) 贡献
        vector< vector<pair<int,int>> > add(n+1);
        // 当前位置 pos 的稀疏表（del -> ways）以 vector<pair> 形式存储并在需要时合并
        vector<pair<int,int>> cur; // pair(del, ways)
        // 初始状态：pos = 0, del = 0 -> 1
        cur.push_back({0,1});

        for(int pos = 0; pos <= n; ++pos){
            // 把延迟到 pos 的项合并到 cur
            if(!add[pos].empty()){
                // 合并 add[pos] into cur
                // We merge by using a small hash map keyed by del to sum ways
                unordered_map<int,int> tmp;
                tmp.reserve(add[pos].size() * 2 + 4);
                for(auto &pr : add[pos]){
                    tmp[pr.first] = ( (int64)tmp[pr.first] + pr.second ) % MOD;
                }
                // merge tmp into cur
                if(!cur.empty()){
                    for(auto &pr : cur) tmp[pr.first] = ( (int64)tmp[pr.first] + pr.second ) % MOD;
                    cur.clear();
                }
                cur.reserve(tmp.size());
                for(auto &kv : tmp) cur.push_back({kv.first, kv.second});
                // tmp destructs here
            }
            if(cur.empty()) continue;
            if(pos == n) break; // at end, we don't transition further

            // 为了遍历高效，若 cur 较大，先把 cur 转为 unordered_map 聚合（避免重复 del）
            if(cur.size() > 32){
                unordered_map<int,int> tmp;
                tmp.reserve(cur.size()*2+4);
                for(auto &pr : cur){
                    tmp[pr.first] = ( (int64)tmp[pr.first] + pr.second ) % MOD;
                }
                cur.clear();
                cur.reserve(tmp.size());
                for(auto &kv : tmp) cur.push_back({kv.first, kv.second});
            } else {
                // cur 较小，仍可能含有重复 del, 合并一次以降低后续成本
                sort(cur.begin(), cur.end(), [](const pair<int,int>&A,const pair<int,int>&B){
                    return A.first < B.first;
                });
                int w = 0;
                for(int i=0;i<(int)cur.size();++i){
                    if(w>0 && cur[i].first == cur[w-1].first){
                        cur[w-1].second = ( (int64)cur[w-1].second + cur[i].second ) % MOD;
                    } else {
                        cur[w++] = cur[i];
                    }
                }
                cur.resize(w);
            }

            // 准备下一个位置的临时容器 next_pos (pos+1) 的累积（直接放到 add[pos+1]）
            // 但是出于统一机制，我们把所有转移（保留）直接 push 到 add[pos+1]
            // 把删除转移 push 到 add[pos+k]
            for(auto &pr : cur){
                int del = pr.first;
                int ways = pr.second;
                if(ways == 0) continue;
                // 保留当前字符 -> 到 pos+1, del 不变
                int ways_keep = (s[pos]==-1) ? 2 : 1;
                int contrib_keep = mulmod(ways, ways_keep);
                // push to add[pos+1]
                add[pos+1].push_back({del, contrib_keep});

                // 尝试删除从 pos 开始的长度 k 段
                if(del < maxDel && pos + k <= n){
                    int c0 = cnt0(pos, pos+k-1);
                    int c1 = cnt1(pos, pos+k-1);
                    if(!(c0>0 && c1>0)){
                        int ways_unify = (c0==0 && c1==0) ? 2 : 1;
                        int contrib_del = mulmod(ways, ways_unify);
                        add[pos+k].push_back({del+1, contrib_del});
                    }
                }
            }
            // 清掉 cur，下一次循环从 add[pos+1] 合并
            cur.clear();

            // 为了控制内存增长，若 add[pos+1] 或 add[pos+k] 变得非常大，我们会在下次迭代把它们合并成小向量
            // 这里不强制即时合并以节约时间，合并在取用时才做
        }

        // 最终合并 add[n] 中的所有 (del,ways) 并求和（这些就是到达 pos=n 的不同删除计数下的贡献）
        int64 ans = 0;
        if(!add[n].empty()){
            unordered_map<int,int> finish;
            finish.reserve(add[n].size()*2+4);
            for(auto &pr : add[n]){
                finish[pr.first] = ( (int64)finish[pr.first] + pr.second ) % MOD;
            }
            for(auto &kv : finish) ans = (ans + kv.second) % MOD;
        }
        cout << (ans%MOD+MOD)%MOD << "\n";
    }
    return 0;
}
