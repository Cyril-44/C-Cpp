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
        g=b/a-1;
        if(g){
            m+=n/g*b;
            n%=g;
        }
        m+=n*a;
        printf("%lld\n",m);
    }
    return 0;
}
/*
0 4
2026 4 100 400 2
2026 4 100 400 19
2026 1 6 12 7
2024 4 4 8 5
*/