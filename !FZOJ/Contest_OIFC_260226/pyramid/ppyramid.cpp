#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,ma[300005],ans[300005];
struct node{
        int x,y,id;
}a[300005],q[300005],lq[300005],rq[300005];
bool cmp(node x,node y){
        return x.x<y.x;
}
void Search(int l,int r,int L,int R){
        if(l>r) return;
        if(L==R){
                for(int i=l;i<=r;i++){
                        ans[q[i].id]=L;
                }
                return;
        }
        int mid=L+R>>1,i1=1,i2=1;
        mid++;
        for(int i=l;i<=r;i++){
                ma[i]=0;
        }
        set<int> s;
        for(int j1=l,j2=l;i1<=n;i1++){
                while(i2<=n&&(ll)(a[i2].x)-a[i1].x<mid){
                        s.insert(a[i2].y);
                        i2++;
                }
                while(j1<=r&&q[j1].x<=a[i1].x) j1++;
                while(j2<=r&&q[j2].x<a[i2].x) j2++;
                s.erase(a[i1].y);
                for(int k=j1;k<j2;k++){
                        auto w=s.upper_bound(q[k].y);
                        if(w==s.end()||w==s.begin()) ma[k]=1e9+7;
                        else{
                                ma[k]=max(ma[k],*w-*prev(w));
                        }
                }
        }
        for(auto it=s.begin();it!=s.end();it=s.erase(it));
        i1=n,i2=n;
        for(int j1=r,j2=r;i1>=1;i1--){
                while(i2>=1&&(ll)(a[i1].x)-a[i2].x<mid){
                        s.insert(a[i2].y);
                        i2--;
                }
                s.erase(a[i1].y);
                while(j1>=l&&q[j1].x>=a[i1].x) j1--;
                while(j2>=l&&q[j2].x>a[i2].x) j2--;
                for(int k=j1;k>j2;k--){
                        auto w=s.upper_bound(q[k].y);
                        if(w==s.end()||w==s.begin()) ma[k]=1e9+7;
                        else{
                                ma[k]=max(ma[k],*w-*prev(w));
                        }
                }
        }
        int cnt1=0,cnt2=0;
        for(int i=l;i<=r;i++){
                if(ma[i]>=mid) lq[++cnt1]=q[i];
                else rq[++cnt2]=q[i];
        }
        for(int i=l,j=1;j<=cnt1;i++,j++){
                q[i]=lq[j];
        }
        for(int i=l+cnt1,j=1;i<=r;i++,j++){
                q[i]=rq[j];
        }
        Search(l+cnt1,r,L,mid-1);
        Search(l,l+cnt1-1,mid,R);
}
int main(){
        freopen("pyramid.in","r",stdin);
        freopen("pyramid.out","w",stdout);
        cin>>n>>m;
        bool ok=1;
        for(int i=1;i<=n;i++){
                scanf("%d%d",&a[i].x,&a[i].y);
                if(a[i].x<a[i-1].x||a[i].y<a[i-1].y) ok=0;
        }
        if(ok||n>=1e5){
                for(int i=1;i<=m;i++){
                        puts("-1");
                }
                return 0;
        }
        a[++n].x=-1e9-10,a[n].y=0;
        a[++n].x=2e9+10,a[n].y=0;
        for(int i=1;i<=m;i++){
                scanf("%d%d",&q[i].x,&q[i].y);
                q[i].id=i;
        }
        sort(a+1,a+n+1,cmp);
        sort(q+1,q+m+1,cmp);
        Search(1,m,1,1e9+7);
        for(int i=1;i<=m;i++){
                if(ans[i]>1e9) puts("-1");
                else printf("%d\n",ans[i]);
        }
        return 0;
}