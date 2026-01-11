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
const int N=510;
int n,a[N][N],x[N],y[N],tmp,cnt;
int diva[4]={0,2,3,1},divb[8]={0,7,1,6,5,2,4,3};
void solve(int xx,int yy,int l){
    if(l==tmp){
        int tx,ty;
        for(int i=0;i<l;i++){
            tx=xx+i; ty=yy+(i<<1);
            if(ty>=yy+l) ty-=l;
            x[tx]=ty; y[tx]=a[tx][ty];
        }
        return;
    }
    int ll;
    if(l==(tmp<<3)){
        ll=l>>3;
        for(int i=0;i<8;i++)
            solve(xx+i*ll,yy+divb[i]*ll,ll);
    }else{
        ll=l>>2;
        for(int i=0;i<4;i++)
            solve(xx+i*ll,yy+diva[i]*ll,ll);
    }
}
bool used[N],used2[N];
void work(){
    in(n);
    if(n%4==2){
        printf("NO\n");
        return;
    }
    tmp=n,cnt=1;
    while(!(tmp&1)){
        cnt<<=1;
        tmp>>=1;
    }
    printf("YES\n");
    for(int i=0;i<cnt;i++)
        for(int j=0;j<cnt;j++)
            for(int k=0;k<tmp;k++)
                for(int l=0;l<tmp;l++)
                    a[i*tmp+k][j*tmp+l]=(i^j)*tmp+(k+l)%tmp;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%d ",a[i][j]);
        putchar('\n');
    }
    for(int i=0;i<n;i++) printf("%d ",i); putchar('\n');
    solve(0,0,n);
    for(int i=0;i<n;i++) printf("%d ",x[i]); putchar('\n');
    for(int i=0;i<n;i++) printf("%d ",y[i]); putchar('\n');
}
int main(){
    int t; in(t);
    while(t--) work();
} 
