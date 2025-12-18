#include<bits/stdc++.h>
using namespace std;
int t,n,m,top1,top2;
double sl1[3005],sl2[3005];
long long ans;
struct node{
    int x,y,c;
}a[3005],b[3005],st1[3005],st2[3005];
bool cmp(node x,node y){
    if(x.x==y.x) return x.y<y.y;
    return x.x<y.x;
}
double slope(int x,int y,int xx,int yy){
    if(x==xx) return 1e18;
    return ((double)(yy-y))/(xx-x);
}
int main(){
    cin>>t;
    while(t--){
        cin>>n;
        ans=m=0;
        int cnt1=0,cnt2=0,cnt3=0;
        for(int i=1;i<=n;i++){
            scanf("%d%d%d",&a[i].x,&a[i].y,&a[i].c);
            if(a[i].c==0) cnt1++;
            else if(a[i].c==1) cnt2++;
            else cnt3++;
        }
        int mx=max(cnt1,max(cnt2,cnt3)),id1,id2;
        if(cnt1==mx){
            id1=1,id2=2;
            for(int i=1;i<=n;i++){
                if(a[i].c==0){
                    b[++m]=a[i];
                }
            }
        }
        else if(cnt2==mx){
            id1=0,id2=2;
            for(int i=1;i<=n;i++){
                if(a[i].c==1){
                    b[++m]=a[i];
                }
            }
        }
        else{
            id1=0,id2=1;
            for(int i=1;i<=n;i++){
                if(a[i].c==2){
                    b[++m]=a[i];
                }
            }
        }
        sort(b+1,b+m+1,cmp);
        st1[top1=1]=b[1];
        sl1[1]=-1e18;
        for(int i=2;i<=m;i++){
            while(top1>1&&slope(st1[top1-1].x,st1[top1-1].y,st1[top1].x,st1[top1].y)<=slope(st1[top1].x,st1[top1].y,b[i].x,b[i].y)){
                top1--;
            }
            st1[++top1]=b[i];
            sl1[top1]=-slope(st1[top1-1].x,st1[top1-1].y,st1[top1].x,st1[top1].y);
        }
        st2[top2=1]=b[1];
        sl2[1]=-1e18;
        for(int i=2;i<=m;i++){
            while(top2>1&&slope(st2[top2-1].x,st2[top2-1].y,st2[top2].x,st2[top2].y)>=slope(st2[top2].x,st2[top2].y,b[i].x,b[i].y)){
                top2--;
            }
            st2[++top2]=b[i];
            sl2[top2]=slope(st2[top2-1].x,st2[top2-1].y,st2[top2].x,st2[top2].y);
        }
        for(int i=1;i<=n;i++){
            if(a[i].c==id1){
                for(int j=1;j<=n;j++){
                    if(a[j].c==id2){
                        int w1=upper_bound(sl1+1,sl1+top1+1,-slope(a[i].x,a[i].y,a[j].x,a[j].y))-sl1-1;
                        int w2=upper_bound(sl2+1,sl2+top2+1,slope(a[i].x,a[i].y,a[j].x,a[j].y))-sl2-1;
                        long long cl1=1ll*st1[w1].x*(a[i].y-a[j].y)-1ll*st1[w1].y*(a[i].x-a[j].x)+1ll*a[i].x*a[j].y-1ll*a[j].x*a[i].y;
                        long long cl2=1ll*st2[w2].x*(a[i].y-a[j].y)-1ll*st2[w2].y*(a[i].x-a[j].x)+1ll*a[i].x*a[j].y-1ll*a[j].x*a[i].y;
                        ans=max(ans,abs(cl1));
                        ans=max(ans,abs(cl2));
                    }
                }
            }
        }
        if(ans%2){
        	printf("%lld.5\n",ans/2);
		}
		else{
			printf("%lld.0\n",ans/2);
		}
    }
    return 0;
}