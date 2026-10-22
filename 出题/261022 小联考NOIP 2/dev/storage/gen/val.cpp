#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
using ull=unsigned long long;
int main(int argc,char**argv) {
    registerValidation(argc,argv);
    prepareOpts(argc,argv);
    int group=has_opt("group")?opt<int>("group"):10;
    ensuref(1<=group&&group<=10,"group must lie in [1,10]");
    int n=inf.readInt(1,100000,"n");inf.readSpace();
    int q=inf.readInt(1,100000,"q");inf.readEoln();int N=2*n-1;
    ensuref(group!=1||(n<=7&&q<=100),"group 1: n<=7 and q<=100");
    ensuref(group!=2||1LL*n*q<=2000000,"group 2: n*q<=2000000");
    vector<int> l(N+1),r(N+1),deg(N+1),depth(N+1),lo(N+1),hi(N+1);
    int leaves=0;
    for(int v=1;v<=N;v++) {
        l[v]=inf.readInt(0,N,"left");inf.readSpace();r[v]=inf.readInt(0,N,"right");inf.readSpace();
        inf.readInt(1,1000000000,"weight");inf.readEoln();
        ensuref((l[v]==0)==(r[v]==0),"node %d must have zero or two children",v);
        if(!l[v])++leaves;
        else {
            ensuref(l[v]!=r[v]&&l[v]!=v&&r[v]!=v,"node %d: repeated child or self-loop",v);
            ensuref(++deg[l[v]]==1&&++deg[r[v]]==1,"node %d: multiple parents",v);
        }
    }
    ensuref(leaves==n&&deg[1]==0,"wrong leaf count or root indegree");
    for(int v=2;v<=N;v++)ensuref(deg[v]==1,"node %d must have one parent",v);
    vector<int> order,stack{1};vector<bool> seen(N+1);
    int leaf=0,maxdepth=0;
    while(!stack.empty()) {
        int v=stack.back();stack.pop_back();ensuref(!seen[v],"cycle/repeated node %d",v);
        seen[v]=true;order.push_back(v);
        if(!l[v])lo[v]=hi[v]=++leaf,maxdepth=max(maxdepth,depth[v]);
        else {
            depth[l[v]]=depth[r[v]]=depth[v]+1;stack.push_back(r[v]);stack.push_back(l[v]);
        }
    }
    ensuref(int(order.size())==N,"tree is disconnected (possibly contains a cycle)");
    ensuref(group!=4||maxdepth<=30,"group 4: leaf depth<=30");
    unordered_set<ull> nodes,active;
    auto key=[](int a,int b){return (ull(a)<<32)|unsigned(b);};
    for(auto it=order.rbegin();it!=order.rend();++it) {
        int v=*it;
        if(l[v]) {
            lo[v]=lo[l[v]];hi[v]=hi[r[v]];
            int a=hi[l[v]]-lo[l[v]]+1,b=hi[r[v]]-lo[r[v]]+1;
            ensuref(group!=3||a==(a+b+1)/2,"group 3: node %d is not a midpoint split",v);
            ensuref(group!=6||l[l[v]]==0,"group 6: node %d has a nonleaf left child",v);
            ensuref(group!=7||b>=a,"group 7: node %d is not right-heavy",v);
        }
        if(group==5)nodes.insert(key(lo[v],hi[v]));
    }
    active.reserve(size_t(q)*2+1);
    for(int step=1;step<=q;step++) {
        string op=inf.readToken("[+-]","op");inf.readSpace();
        int a=inf.readInt(1,n,"l");inf.readSpace();int b=inf.readInt(a,n,"r");inf.readEoln();
        ensuref(group!=5||nodes.count(key(a,b)),"group 5: operation %d is not a node interval",step);
        ensuref(group!=8||op=="+","group 8: operation %d is a deletion",step);
        ensuref(group!=9||b-a+1<=2,"group 9: operation %d is longer than 2",step);
        if(op=="+")ensuref(active.insert(key(a,b)).second,"operation %d: duplicate insertion",step);
        else ensuref(active.erase(key(a,b))==1,"operation %d: inactive deletion",step);
    }
    inf.readEof();
}
