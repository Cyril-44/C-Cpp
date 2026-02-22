#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
long long rndll(long long L,long long R){return uniform_int_distribution<long long>(L,R)(rng);}
int rndi(int L,int R){return uniform_int_distribution<int>(L,R)(rng);}

void gen_chain(int m, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    c[1]=rndll(1,1e18); x[1]=0;
    for(int i=2;i<=m;i++){x[i]=i-1; c[i]=rndll(c[i-1],1e18);}
}
void gen_star(int m, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    c[1]=rndll(1,1e18); x[1]=0;
    for(int i=2;i<=m;i++){x[i]=1; c[i]=rndll(c[1],1e18);}
}
void gen_random_dag(int m, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    c[1]=rndll(1,1e18); x[1]=0;
    for(int i=2;i<=m;i++){
        if(rndi(0,1)) x[i]=0;
        else x[i]=rndi(1,i-1);
        long long base=(x[i]==0?1:c[x[i]]);
        c[i]=rndll(base,1e18);
    }
}
void gen_equal_c(int m, vector<long long>&c, vector<int>&x, long long val){
    c.assign(m+1,val);
    x.resize(m+1);
    x[1]=0;
    for(int i=2;i<=m;i++){
        if(rndi(0,1)) x[i]=0;
        else x[i]=rndi(1,i-1);
    }
}
void gen_increasing_c(int m, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    for(int i=1;i<=m;i++) c[i]=i;
    x[1]=0;
    for(int i=2;i<=m;i++){
        if(rndi(0,1)) x[i]=0;
        else x[i]=rndi(1,i-1);
    }
}
void gen_decreasing_c(int m, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    for(int i=1;i<=m;i++) c[i]=m-i+1;
    x[1]=0;
    for(int i=2;i<=m;i++){
        if(rndi(0,1)) x[i]=0;
        else x[i]=rndi(1,i-1);
    }
}
void gen_layered(int m,int L, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    int idx=1;
    c[idx]=rndll(1,1e18); x[idx]=0; idx++;
    while(idx<=m){
        int parent=max(1,idx-L);
        x[idx]=parent;
        c[idx]=rndll(c[parent],1e18);
        idx++;
    }
}
void gen_forest(int m, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    for(int i=1;i<=m;i++){
        c[i]=rndll(1,1e18);
        if(i==1) x[i]=0;
        else if(rndi(0,4)==0) x[i]=0;
        else x[i]=rndi(1,i-1);
        if(x[i]!=0) c[i]=rndll(c[x[i]],1e18);
    }
}
void gen_interleave(int m, vector<long long>&c, vector<int>&x){
    c.resize(m+1); x.resize(m+1);
    c[1]=rndll(1,1e18); x[1]=0;
    for(int i=2;i<=m;i++){
        if(i%2==0) x[i]=i-1;
        else x[i]=max(1,i-3);
        c[i]=rndll(c[x[i]],1e18);
    }
}

void gen_sub1(int Y,long long&n,int&m,vector<long long>&c,vector<int>&x){
    n=1;
    if(Y==1){m=rndi(5,20); gen_random_dag(m,c,x);}
    if(Y==2){m=1000000; gen_chain(m,c,x);}
    if(Y==3){m=1000000; gen_star(m,c,x);}
    if(Y==4){m=1000000; c.resize(m+1); x.assign(m+1,0); for(int i=1;i<=m;i++) c[i]=rndll(1,1e18);}
    if(Y==5){m=1000000; gen_increasing_c(m,c,x);}
    if(Y==6){m=1000000; gen_decreasing_c(m,c,x);}
    if(Y==7){m=1000000; gen_equal_c(m,c,x,rndll(1,1e18));}
    if(Y==8){m=1000000; gen_forest(m,c,x);}
    if(Y==9){m=1000000; gen_interleave(m,c,x);}
    if(Y==10){m=1000000; gen_layered(m,1000,c,x);}
}

void gen_sub2(int Y,long long&n,int&m,vector<long long>&c,vector<int>&x){
    m=1; c.resize(2); x.resize(2); x[1]=0;
    if(Y==1){n=1e18; c[1]=1;}
    if(Y==2){n=1; c[1]=1e18;}
    if(Y==3){n=1e18; c[1]=1e18;}
    if(Y==4){n=1; c[1]=1;}
    if(Y==5){n=rndll(1,1e18); c[1]=rndll(1,1e18);}
}

void gen_sub3(int Y,long long&n,int&m,vector<long long>&c,vector<int>&x){
    n=rndll(1,1e18);
    m=1000000;
    x.assign(m+1,0);
    if(Y==1){c.resize(m+1); for(int i=1;i<=m;i++) c[i]=rndll(1,1e18);}
    if(Y==2){gen_increasing_c(m,c,x);}
    if(Y==3){gen_decreasing_c(m,c,x);}
    if(Y==4){gen_equal_c(m,c,x,rndll(1,1e18));}
    if(Y==5){gen_equal_c(m,c,x,1e18);}
    if(Y==6){gen_equal_c(m,c,x,1);}
    if(Y==7){c.resize(m+1); for(int i=1;i<=m;i++) c[i]= (rndi(0,4)==0? rndll(1,1e18): rndll(1,100));}
    if(Y==8){c.resize(m+1); for(int i=1;i<=m;i++) c[i]= (i/1000)*1000;}
    if(Y==9){c.resize(m+1); for(int i=1;i<=m;i++) c[i]=min((long long)1e18,(1LL<<min(i,60))); }
    if(Y==10){c.resize(m+1); for(int i=1;i<=m;i++) c[i]=i%100;}
}

void gen_sub4(int Y,long long&n,int&m,vector<long long>&c,vector<int>&x){
    n=rndll(1,1e18);
    m=1000000;
    c.resize(m+1); x.resize(m+1);
    c[1]=rndll(1,1e18); x[1]=0;
    for(int i=2;i<=m;i++){c[i]=c[i-1];}
    if(Y==1){for(int i=2;i<=m;i++) x[i]=0;}
    if(Y==2){for(int i=2;i<=m;i++) x[i]=i-1;}
    if(Y==3){for(int i=2;i<=m;i++) x[i]=1;}
    if(Y==4){for(int i=2;i<=m;i++){if(rndi(0,1)) x[i]=0; else x[i]=rndi(1,i-1);}}
    if(Y==5){gen_layered(m,500,c,x);}
    if(Y==6){gen_forest(m,c,x);}
    if(Y==7){gen_interleave(m,c,x);}
    if(Y==8){for(int i=2;i<=m;i++){if(rndi(0,9)==0) x[i]=rndi(1,i-1); else x[i]=0;}}
    if(Y==9){for(int i=2;i<=m;i++){if(rndi(0,9)) x[i]=rndi(1,i-1); else x[i]=0;}}
    if(Y==10){for(int i=2;i<=m;i++) x[i]=i-1;}
}

void gen_sub5(int Y,long long&n,int&m,vector<long long>&c,vector<int>&x){
    n=rndll(1,1e18);
    m=rndi(500,1000);
    if(Y==1){gen_random_dag(m,c,x);}
    if(Y==2){gen_chain(m,c,x);}
    if(Y==3){gen_star(m,c,x);}
    if(Y==4){gen_layered(m,10,c,x);}
    if(Y==5){gen_forest(m,c,x);}
    if(Y==6){for(int i=1;i<=m;i++){if(i==1)x.push_back(0); else if(rndi(0,9)==0)x.push_back(rndi(1,i-1)); else x.push_back(0);} c.resize(m+1); for(int i=1;i<=m;i++) c[i]=rndll(1,1e18);}
    if(Y==7){for(int i=1;i<=m;i++){if(i==1)x.push_back(0); else if(rndi(0,9))x.push_back(rndi(1,i-1)); else x.push_back(0);} c.resize(m+1); for(int i=1;i<=m;i++) c[i]=rndll(1,1e18);}
    if(Y==8){gen_increasing_c(m,c,x);}
    if(Y==9){gen_decreasing_c(m,c,x);}
    if(Y==10){gen_equal_c(m,c,x,rndll(1,1e18));}
}

void gen_sub6(int Y,long long&n,int&m,vector<long long>&c,vector<int>&x){
    n=rndll(1,100);
    m=200000;
    if(Y==1){gen_random_dag(m,c,x);}
    if(Y==2){gen_chain(m,c,x);}
    if(Y==3){gen_star(m,c,x);}
    if(Y==4){gen_layered(m,2000,c,x);}
    if(Y==5){gen_forest(m,c,x);}
    if(Y==6){gen_interleave(m,c,x);}
    if(Y==7){c.resize(m+1); x.resize(m+1); c[1]=rndll(1,1e18); x[1]=0; for(int i=2;i<=m;i++){if(rndi(0,9)==0)x[i]=rndi(1,i-1); else x[i]=0; long long base=(x[i]==0?1:c[x[i]]); c[i]=rndll(base,1e18);} }
    if(Y==8){c.resize(m+1); x.resize(m+1); c[1]=rndll(1,1e18); x[1]=0; for(int i=2;i<=m;i++){if(rndi(0,9))x[i]=rndi(1,i-1); else x[i]=0; long long base=(x[i]==0?1:c[x[i]]); c[i]=rndll(base,1e18);} }
    if(Y==9){gen_increasing_c(m,c,x);}
    if(Y==10){gen_equal_c(m,c,x,rndll(1,1e18));}
}

void gen_sub7(int Y,long long&n,int&m,vector<long long>&c,vector<int>&x){
    if(Y==1){n=rndll(1,1e18); m=1000000; gen_chain(m,c,x);}
    if(Y==2){n=rndll(1,1e18); m=1000000; gen_star(m,c,x);}
    if(Y==3){n=rndll(1,1e18); m=1000000; gen_random_dag(m,c,x);}
    if(Y==4){n=rndll(1,1e18); m=1000000; gen_layered(m,2000,c,x);}
    if(Y==5){n=rndll(1,1e18); m=1000000; gen_equal_c(m,c,x,1e18);}
    if(Y==6){n=rndll(1,1e18); m=1000000; gen_equal_c(m,c,x,1);}
    if(Y==7){n=rndll(1,1e18); m=1000000; gen_random_dag(m,c,x);}
    if(Y==8){n=rndll(1,1e18); m=1000000; gen_interleave(m,c,x);}
    if(Y==9){n=rndll(1,1e18); m=1000000; gen_forest(m,c,x);}
    if(Y==10){n=rndll(1,1e18); m=1000000; gen_chain(m,c,x);}
}

int main(int argc,char*argv[]){
    string s=argv[1];
    int p=s.find('.');
    int X=stoi(s.substr(0,p));
    int Y=stoi(s.substr(p+1));
    long long n; int m;
    vector<long long> c;
    vector<int> x;

    if(X==1) gen_sub1(Y,n,m,c,x);
    if(X==2) gen_sub2(Y,n,m,c,x);
    if(X==3) gen_sub3(Y,n,m,c,x);
    if(X==4) gen_sub4(Y,n,m,c,x);
    if(X==5) gen_sub5(Y,n,m,c,x);
    if(X==6) gen_sub6(Y,n,m,c,x);
    if(X==7) gen_sub7(Y,n,m,c,x);

    cout<<1<<"\n";
    cout<<n<<" "<<m<<"\n";
    for(int i=1;i<=m;i++) cout<<c[i]<<(i==m?'\n':' ');
    for(int i=1;i<=m;i++) cout<<x[i]<<(i==m?'\n':' ');
}
