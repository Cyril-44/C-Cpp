#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
template<class T>
void in(T &x){
    char c=getchar();T f=1; x=0;
    while(c<'0'||c>'9'){
        if(c=='-') f=-1;
        c=getchar();
    }
    while(c>='0'&&c<='9'){
        x=x*10+c-'0';
        c=getchar();
    }
    x*=f;
}
const int N=200010;
int n,q,x,y,z,a[N],b[N];
set<pii> sa,sb,mx,mn;
ll sum,add;
int main(){
    in(n); in(q);
    for(int i=1;i<=n;i++){
        in(a[i]),in(b[i]); sum+=a[i];
        if(b[i]==1) sa.insert({a[i],i});
        else sb.insert({a[i],i});
        mn.insert({a[i],i});
    }
    while(mx.size()<sb.size()){
        mx.insert(*mn.rbegin()); add+=mn.rbegin()->first;
        mn.erase(*mn.rbegin());
    }
    while(q--){
        in(x); in(y); in(z);
        sum-=a[x];
        if(b[x]==1) sa.erase(sa.lower_bound({a[x],x}));
        else sb.erase(sa.lower_bound({a[x],x}));
        if(mx.count({a[x],x})) mx.erase({a[x],x}),add-=a[x];
        else mn.erase({a[x],x}); 
//        cerr<<sa.size()<<" "<<sb.size()<<" "<<mx.size()<<" "<<mn.size()<<" "<<mn.begin()->first<<" "<<mn.begin()->second<<endl;
//        cerr<<x<<endl;
        a[x]=y; b[x]=z; sum+=a[x]; 
        
        if(b[x]==1) sa.insert({a[x],x});
        else sb.insert({a[x],x});
        
        if(mx.empty()) mn.insert({a[x],x});
        else if(make_pair(a[x],x)<*mx.begin()) mn.insert({a[x],x});
        else mx.insert({a[x],x}),add+=a[x];
        
        while(mx.size()<sb.size()){
            mx.insert(*mn.rbegin()); add+=mn.rbegin()->first;
            mn.erase(*mn.rbegin());
        }
        while(mx.size()>sb.size()){
            mn.insert(*mx.begin()); add-=mx.begin()->first;
            mx.erase(*mx.begin());
        }
        
        if(sa.empty()) printf("%lld\n",sum+add-mx.begin()->first);
        else if(sb.empty()) printf("%lld\n",sum);
        else{
            if(sb.begin()->first>sa.rbegin()->first) printf("%lld\n",sum+add-sb.begin()->first+sa.rbegin()->first);
            else printf("%lld\n",sum+add); 
        }
    }
}