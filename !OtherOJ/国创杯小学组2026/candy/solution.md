首先，根据题意，我们知道总共要分出去 $m\times k$ 个糖果。而小可可总共有 $n$ 个糖果。所以如果 $n\lt m\times k$，说明糖果不够分，输出 $-1$。

否则，我们知道小可可还剩下 $n-m\times k$ 个糖果，而每个糖果的美味值为 $x$，所以总美味值为 $(n-m\times k)\times x$。注意由于数据范围有 $10^6$，所以要开 long long。

code：

```cpp
#include<bits/stdc++.h>
#define ll long long
using namespace std;
ll n,x,m,k;
int main(){
    cin>>n>>x>>m>>k;
    if(n<m*k) cout<<-1;
    else cout<<(n-m*k)*x;
    return 0;
}
```