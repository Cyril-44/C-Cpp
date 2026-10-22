#include "testlib.h"
#include <algorithm>
#include <cstdio>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
constexpr int P=998244353, LIMIT=100000;
const vector<int> periods={1,2,3,4,5,7,8,15,16,17,31,32,33,63,64,65,127,128,129,255,256,257,1023,1024,1025};
const vector<int> edges={0,1,P-1,P-2,P/2,2};
pair<int,int> boundary(int n,int step,bool cal) {
    int mode=step%14,l,r;
    if(mode==0)l=1,r=n;
    else if(mode==1)l=1,r=max(1,n-1);
    else if(mode==2)l=min(2,n),r=n;
    else if(mode==3)l=r=1;
    else if(mode==4)l=r=n;
    else if(mode==5)l=r=(n+1)/2;
    else if(mode==6)l=1,r=min(n,2);
    else if(mode==7)l=max(1,n-1),r=n;
    else if(mode==8)l=1,r=min(n,3);
    else if(mode==9)l=max(1,n-2),r=n;
    else {int len=min(n,periods[(step/14+mode)%periods.size()]);if(mode&1)l=1,r=len;else l=n-len+1,r=n;}
    if(cal&&r-l+1<2)l=min(l,n-1),r=l+1;
    return {l,r};
}
pair<int,int> range(int n,int step,const string& profile,bool cal) {
    if(profile=="noop")return cal&&step%4==0?boundary(n,step,true):make_pair(1,n);
    if(profile=="boundaries"||step%16==0)return boundary(n,profile=="boundaries"?step:step/16,cal);
    if(profile=="point"&&!cal&&step%5){int p=1+(1LL*step*65537%n);return {p,p};}
    if(profile=="offset") {int width=max(cal?2:1,n-n/3),l=1+(1LL*step*97+1LL*(step/64)*257)%(n-width+1);return {l,l+width-1};}
    if(profile=="nested"||profile=="gc") {
        int phase=step%128,inset=min((phase/2)*max(1,n/256),max(0,n-(cal?2:1)));
        int l=phase&1?1:1+inset,r=phase&1?n-inset:n;
        if(profile=="gc"&&phase%7==0){int center=1+1LL*step*1009%n;l=max(1,center-n/8);r=min(n,center+n/8);}
        if(cal&&r-l+1<2)l=min(l,n-1),r=l+1;
        return {l,r};
    }
    int mode=step%10;
    if(mode==0)return {1,n};
    if(mode==1)return boundary(n,step/10,cal);
    if(!cal&&mode==2){int p=rnd.next(1,n);return {p,p};}
    if(cal){int l=rnd.next(1,n-1);return {l,rnd.next(l+1,n)};}
    int l=rnd.next(1,n),r=rnd.next(1,n);if(l>r)swap(l,r);return {l,r};
}
int choose_k(int len,int step,const string& profile) {
    int high=len-1,mode=step%12;
    if(mode==0)return 1;
    if(mode==1)return high;
    if(mode==2)return max(1,high-1);
    if(profile=="small_k"||profile=="offset"||profile=="gc"||mode==3||mode==4)return min(high,periods[(step/12+mode)%periods.size()]);
    if(profile=="large_k")return rnd.next(max(1,high/4),high);
    return rnd.next(1,high);
}
pair<int,int> affine(int step) {
    switch(step%8){
        case 0:return {0,edges[(step/8)%edges.size()]};case 1:return {1,0};
        case 2:return {P-1,P-1};case 3:return {1,P-1};case 4:return {0,0};case 5:return {P-1,1};
        default:return {rnd.next(P),rnd.next(P)};
    }
}
int main(int argc,char**argv) {
    registerGen(argc,argv,1);
    // All generation uses testlib's seeded rnd. The final seed argument is part
    // of registerGen's command-line hash, so each full command is reproducible.
    ensuref(argc==7,"usage: gen n m group array profile seed");
    int n=atoi(argv[1]),m=atoi(argv[2]),g=atoi(argv[3]);string ar=argv[4],profile=argv[5];
    ensuref(1<=n&&n<=LIMIT&&1<=m&&m<=LIMIT,"n,m must be in [1,100000]");
    ensuref(1<=g&&g<=5,"group must be 1..5");
    ensuref(ar=="random"||ar=="edges"||ar=="periodic"||ar=="arithmetic","unknown initial array");
    ensuref(profile=="mixed"||profile=="affine"||profile=="queries"||profile=="point"||profile=="boundaries"||profile=="offset"||profile=="nested"||profile=="gc"||profile=="noop"||profile=="sweep"||profile=="small_k"||profile=="large_k"||profile=="preserved_seed","unknown profile");
    ensuref(g!=1||max(n,m)<=1000,"group 1 size");
    ensuref(g==1||max(n,m)>1000,"groups 2..5 lower size bound");
    ensuref(g!=3||(n>=2&&m>=2),"group 3 requires calibration and query");
    ensuref((g!=4&&g!=5)||(n>=3&&m>=2),"general groups require k>1");
    ensuref(g!=4||max(n,m)<=10000,"group 4 upper size bound");
    ensuref(g!=5||max(n,m)>10000,"group 5 lower size bound");
    ensuref(profile!="sweep"||(n<=16&&g==1),"sweep requires n<=16 and group 1");
    ensuref(profile!="preserved_seed"||(n>=8&&(g==1||g==4||g==5)),"preserved_seed requires n>=8 and a general calibration group");
    setvbuf(stdout,nullptr,_IOFBF,1<<20);
    printf("%d\n",n);
    int offset=rnd.next((int)edges.size()),period=vector<int>{3,7,16,31}[rnd.next(4)],d=rnd.next(P),v=rnd.next(P);
    vector<int> base(period);for(int&x:base)x=rnd.next(P);
    for(int i=0;i<n;++i){int x;if(ar=="random")x=rnd.next(P);else if(ar=="edges")x=edges[(i+offset)%edges.size()];else if(ar=="periodic")x=base[i%period];else x=v,v=(v+1LL*d)%P;printf("%d%c",x,i+1==n?'\n':' ');}
    printf("%d\n",m);
    vector<tuple<int,int,int>> sweep;
    if(profile=="sweep")for(int l=1;l<n;++l)for(int r=l+1;r<=n;++r)for(int k=1;k<=r-l;++k)sweep.emplace_back(l,r,k);
    size_t si=0;int ac=0;
    for(int step=0;step<m;++step) {
        if(step==0&&g>=3){if(g==3)puts("1 1 2 1");else puts("1 1 3 2");continue;}
        if(step==m-1){printf("3 1 %d\n",n);continue;}
        if(profile=="preserved_seed") {
            int h=n/2,phase=step%16;
            if(phase<12){int k=rnd.next(min(max(2,n/3),h-1),h-1),l=h-k;
                int r=step%4==0?n:step%4==1?n-1:rnd.next(h+max(1,(n-h)/2),n);printf("1 %d %d %d\n",l,r,k);}
            else if(phase<15){int l=phase==12?h+1:rnd.next(h+1,n),r=phase==12?n:phase==13?l:rnd.next(l,n);auto [u,v]=affine(ac++);printf("2 %d %d %d %d\n",l,r,u,v);}
            else printf("3 %d %d\n",rnd.next(1,h),rnd.next(h+1,n));
            continue;
        }
        int pick=rnd.next(100),op;
        if(g==2||profile=="affine"||profile=="point"||profile=="queries"||n<2)op=profile=="queries"||pick>=65?3:2;
        else if(profile=="gc")op=step%16<12?1:step%16<15?2:3;
        else if(profile=="noop")op=pick<75?1:pick<85?2:3;
        else if(profile=="sweep")op=step%4<2?1:step%4==2?2:3;
        else op=pick<58?1:pick<82?2:3;
        if(op==1&&!sweep.empty()){auto [l,r,k]=sweep[si++%sweep.size()];printf("1 %d %d %d\n",l,r,k);continue;}
        auto [l,r]=range(n,step,profile,op==1);
        if(op==1){int k=g==3?1:choose_k(r-l+1,step,profile);printf("1 %d %d %d\n",l,r,k);}
        else if(op==2){auto [u,v]=affine(ac++);printf("2 %d %d %d %d\n",l,r,u,v);}
        else printf("3 %d %d\n",l,r);
    }
}
