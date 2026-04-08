#include<bits/stdc++.h>
#define ll long long
using namespace std;
ll n,x,m,k;
int main(){
    freopen("candy.in","r",stdin);
    freopen("candy.out","w",stdout);
    cin>>n>>x>>m>>k;
    if(n<m*k) cout<<-1;
    else cout<<(n-m*k)*x;
    return 0;
}