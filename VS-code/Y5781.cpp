#include<bits/stdc++.h>
using namespace std;
#define ll unsigned long long
ll t,C[70][70];
ll work(ll k,ll m){
    ll ans=0;
    while(k){
        ll x=k;
        while(C[x][k]<=m){
            x++;
        }
        x--;
        ans|=1llu<<x;
        m-=C[x][k];
        k--;
    }
    return ans;
}
int main(){
    C[0][0]=1;
    for(int i=1;i<=67;i++){
        C[i][0]=C[i][i]=1;
        for(int j=1;j<i;j++){
            C[i][j]=C[i-1][j]+C[i-1][j-1];
        }
    }
    cin>>t;
    while(t--){
        ll m,k;
        scanf("%llu%llu",&m,&k);
        if(k==1){
            puts("1 -1");
        }
        else{
            ll r=work(k-1,m),l=0;
            if(m) l=work(k-1,m-1);
            printf("%llu %llu\n",l+1,r-l);
        }
    }
    return 0;
}