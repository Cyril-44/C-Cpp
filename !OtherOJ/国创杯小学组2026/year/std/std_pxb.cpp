#include<bits/stdc++.h>
#define ll long long
using namespace std;
int tid,t;
ll m,a,b,c,n;
int main(){
    cin>>tid>>t;
    while(t--){
        scanf("%lld%lld%lld%lld%lld",&m,&a,&b,&c,&n);
        m--;
        n+=m/a-m/b+m/c;
        m=0;
        ll g=c/a-c/b+1;
        if(g){
            m+=n/g*c;
            n%=g;
        }
        if(!n){
            printf("%lld\n",m);
            continue;
        }
        g=b/a-1;
        if(g){
            m+=n/g*b;
            n%=g;
        }
        if(!n) m-=a;
        else m+=n*a;
        printf("%lld\n",m);
    }
    return 0;
}