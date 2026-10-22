#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
using Interval = pair<int,int>;
using ull = unsigned long long;

// gen n q shape mode weights seed numbering
// Testlib's RNG is explicitly seeded, so all seven arguments reproduce a case.
struct Node { int left=-1,right=-1,lo=0,hi=0,depth=0; };
struct Active {
    vector<Interval> items;
    unordered_map<ull,int> pos;
    static ull key(Interval x) { return (ull(x.first)<<32)|unsigned(x.second); }
    bool contains(Interval x) const { return pos.count(key(x)); }
    char toggle(Interval x) {
        ull k=key(x); auto it=pos.find(k);
        if(it==pos.end()) { pos[k]=int(items.size()); items.push_back(x); return '+'; }
        int i=it->second; Interval last=items.back(); items.pop_back(); pos.erase(it);
        if(i<int(items.size())) { items[i]=last; pos[key(last)]=i; }
        return '-';
    }
};
int main(int argc,char**argv) {
    registerGen(argc,argv,1);
    ensuref(argc==8,"usage: gen n q shape mode weights seed numbering");
    int n=opt<int>(1),q=opt<int>(2);
    string shape=opt<string>(3),mode=opt<string>(4),weights=opt<string>(5);
    long long seed=opt<long long>(6); string numbering=opt<string>(7);
    ensuref(1<=n&&n<=100000&&1<=q&&q<=100000,"n,q out of bounds");
    ensuref(set<string>{"mid","random","left_comb","right_comb","zigzag_comb","heavy_right","bounded30"}.count(shape),"unknown shape");
    ensuref(set<string>{"random","mixed","node_only","insert_only","root_singleton","alternating","adversarial","short"}.count(mode),"unknown mode");
    ensuref(set<string>{"unit","random","large","tie"}.count(weights),"unknown weight profile");
    ensuref(set<string>{"random","preorder","reverse"}.count(numbering),"unknown numbering");
    ensuref(mode!="insert_only"||q<=1LL*n*(n+1)/2,"not enough distinct intervals");
    rnd.setSeed(seed);
    int N=2*n-1,next=1;
    vector<Node> t(N); t[0].lo=1; t[0].hi=n;
    vector<int> stack{0},internal,cherries;
    while(!stack.empty()) {
        int v=stack.back();stack.pop_back();
        int len=t[v].hi-t[v].lo+1;
        if(len==1)continue;
        internal.push_back(v);if(len==2)cherries.push_back(v);
        int a=0;
        if(shape=="mid")a=(len+1)/2;
        else if(shape=="left_comb")a=len-1;
        else if(shape=="right_comb")a=1;
        else if(shape=="zigzag_comb")a=t[v].depth%2?1:len-1;
        else if(shape=="heavy_right") {
            int kind=rnd.next(3);a=kind==0?1:kind==1?len/2:rnd.next(1,len/2);
        } else if(shape=="bounded30") {
            int remain=30-t[v].depth; ensuref(remain>0,"bounded30 capacity exceeded");
            int cap=1<<(remain-1);a=rnd.next(max(1,len-cap),min(len-1,cap));
        } else a=rnd.next(1,len-1);
        int l=next++,r=next++; t[v].left=l;t[v].right=r;
        t[l].lo=t[v].lo;t[l].hi=t[v].lo+a-1;
        t[r].lo=t[l].hi+1;t[r].hi=t[v].hi;
        t[l].depth=t[r].depth=t[v].depth+1;
        stack.push_back(r);stack.push_back(l);
    }
    ensuref(next==N,"wrong tree size");
    vector<int> labels(N);iota(labels.begin(),labels.end(),1);
    if(numbering=="random")shuffle(labels.begin()+1,labels.end());
    else if(numbering=="reverse")reverse(labels.begin()+1,labels.end());
    else {
        stack={0};int id=1;
        while(!stack.empty()) {
            int v=stack.back();stack.pop_back();labels[v]=id++;
            if(t[v].left!=-1) {stack.push_back(t[v].right);stack.push_back(t[v].left);}
        }
    }
    vector<array<int,3>> rows(N);
    for(int v=0;v<N;v++) {
        int w=1;
        if(weights=="tie")w=t[v].hi-t[v].lo+1;
        else if(weights=="large")w=1000000000-rnd.next(1000001);
        else if(weights=="random") {
            int k=rnd.next(4);w=k==0?rnd.next(1,9):k==1?rnd.next(1,10000):rnd.next(100000000,1000000000);
        }
        rows[labels[v]-1]={t[v].left<0?0:labels[t[v].left],t[v].right<0?0:labels[t[v].right],w};
    }
    auto interval=[&](int v)->Interval{return {t[v].lo,t[v].hi};};
    auto random_interval=[&]()->Interval {
        int k=rnd.next(6),l=rnd.next(1,n);
        if(k==0)return {l,min(n,l+rnd.next(4))};
        if(k==1)return rnd.next(2)?Interval{1,l}:Interval{l,n};
        int r=rnd.next(1,n);return {min(l,r),max(l,r)};
    };
    vector<pair<Interval,Interval>> pairs;
    int limit=min(192,int(internal.size()));
    for(int i=0;i<limit;i++) {
        int v=internal[1LL*i*(internal.size()-1)/max(1,limit-1)];
        int a=t[v].left,b=t[v].right;
        if(t[b].hi>t[b].lo)pairs.push_back({interval(a),{t[a].lo,t[a].hi+1}});
        if(t[a].hi>t[a].lo)pairs.push_back({interval(b),{t[b].lo-1,t[b].hi}});
    }
    if(pairs.empty())pairs.push_back({{1,1},{1,n}});
    vector<Interval> prefix;
    if(mode=="adversarial"&&n>1) {
        prefix.push_back({1,n});
        for(auto [a,b]:pairs)for(Interval x:vector<Interval>{a,b,a,b,b,a,b,a})prefix.push_back(x);
        for(int v:cherries) {
            prefix.push_back(interval(v));if(weights!="tie")prefix.push_back({t[v].lo,t[v].lo});
        }
        for(auto it=cherries.rbegin();it!=cherries.rend();++it) {
            int v=*it;prefix.push_back({t[v].hi,t[v].hi});prefix.push_back({t[v].lo,t[v].lo});
        }
    } else if(mode=="short") {
        for(int i=0;i<min(16,int(pairs.size()));i++) {
            auto [a,b]=pairs[i];
            if(a.second-a.first<=1&&b.second-b.first<=1)
                for(Interval x:vector<Interval>{a,b,a,b})prefix.push_back(x);
        }
        for(int v:cherries) {
            prefix.push_back(interval(v));if(weights!="tie")prefix.push_back({t[v].lo,t[v].lo});
        }
    }
    Active active;active.pos.reserve(size_t(q)*2+1);
    int fallback_l=1,fallback_r=1;
    cout<<n<<' '<<q<<'\n';
    for(auto row:rows)cout<<row[0]<<' '<<row[1]<<' '<<row[2]<<'\n';
    for(int step=0;step<q;step++) {
        Interval x{1,1};
        if((mode=="adversarial"||mode=="short")&&step<int(prefix.size()))x=prefix[step];
        else if(mode=="root_singleton") {
            int at=step%(2*n+2);
            if(n==1)x={1,1};
            else if(at==0||at==2*n+1)x={1,n};
            else {int p=at<=n?at:2*n+1-at;x={p,p};}
        } else if(mode=="alternating") {
            auto [a,b]=pairs[(step/8)%pairs.size()];
            x=vector<Interval>{a,b,a,b,b,a,b,a}[step%8];
        } else if(mode=="adversarial") {
            if(n==1)x={1,1};
            else {
                int at=step-int(prefix.size());
                if(at%12<8) {
                    auto [a,b]=pairs[(at/12)%pairs.size()];
                    x=vector<Interval>{a,b,a,b,b,a,b,a}[at%12];
                } else if(at%12==8)x=interval(internal[rnd.next(int(internal.size()))]);
                else if(at%12==9) {
                    int v=internal[rnd.next(int(internal.size()))];x={t[v].lo,max(t[v].lo,t[v].hi-1)};
                } else x=random_interval();
            }
        } else if(mode=="insert_only") {
            bool found=false;
            for(int attempt=0;attempt<24&&!found;attempt++) {
                x=attempt%4==0?interval(rnd.next(N)):random_interval();found=!active.contains(x);
            }
            if(!found) {
                while(active.contains({fallback_l,fallback_r})) {
                    if(++fallback_r>n)fallback_r=++fallback_l;
                }
                ensuref(fallback_l<=n,"interval universe exhausted");x={fallback_l,fallback_r};
            }
        } else if(!active.items.empty()&&((mode=="mixed"&&active.items.size()>=256)||
                  (mode=="random"&&active.items.size()>=512)||rnd.next(100)<43)) {
            x=active.items[rnd.next(int(active.items.size()))];
        } else if(mode=="short") {
            int l=rnd.next(1,n);x={l,l==n||rnd.next(2)==0?l:l+1};
        } else if(mode=="node_only")x=interval(rnd.next(N));
        else if(mode=="mixed"&&step%5<2)x=interval(rnd.next(N));
        else if(mode=="mixed"&&step%5==2)x={1,n};
        else x=random_interval();
        char op=active.toggle(x);ensuref(mode!="insert_only"||op=='+',"duplicate insertion");
        cout<<op<<' '<<x.first<<' '<<x.second<<'\n';
    }
}
