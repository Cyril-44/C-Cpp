#include<bits/stdc++.h>
#define ll long long
using namespace std;
int t,n,m,a[100005],mm,b[100005];
ll cnt,ans;
set<int> s;
struct node{
    int l,r,k;
}q[100005],qq[100005];
bool cmp(node x,node y){
    return x.r<y.r;
}
int main(){
    cin>>t;
    while(t--){
        cin>>n>>m;
        cnt=0;
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
            cnt+=a[i];
        }
        for(int i=1;i<=m;i++){
            scanf("%d%d%d",&q[i].l,&q[i].r,&q[i].k);
        }
        sort(q+1,q+m+1,cmp);
        for(int i=1;i<=n;i++){
            mm=0;
            s.clear();
            for(int i=1;i<=n;i++){
                s.insert(i);
                b[i]=a[i];
            }
            for(int j=1;j<=m;j++){
                if(q[j].l<=i&&q[j].r>=i){
                    qq[++mm]=q[j];
                }
            }
            ans=cnt;
            for(int j=1;j<=mm;j++){
                auto it=s.lower_bound(qq[j].l);
                while(it!=s.end()&&*it<=qq[j].r){
                    if(b[*it]>qq[j].k){
                        ans-=qq[j].k;
                        b[*it]-=qq[j].k;
                        break;
                    }
                    else{
                        ans-=b[*it];
                        qq[j].k-=b[*it];
                        s.erase(it++);
                    }
                }
            }
            printf("%lld ",ans);
        }
        putchar('\n');
    }
	return 0;
}