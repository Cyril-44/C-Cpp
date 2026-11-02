#include "testlib.h"
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
ll n,c;
string s1,s2;
void nxt(ll &x,ll &y,char ch){
    if(ch=='U')y++;
    else if(ch=='D')y--;
    else if(ch=='R')x++;
    else if(ch=='L')x--;
}
bool chkpointvalid(ll x,ll y){
    if(c==1){
        return (x>=0 && y>=0);
    }else if(c==2){
        return (x<=0 && y>=0);
    }else if(c==3){
        return (x<=0 && y<=0);
    }else{
        return (x>=0 && y<=0);
    }
}
ll solve(ll Tid){
    n=inf.readLong();
    c=inf.readLong();
    s1=inf.readToken();
    string a1=ans.readToken(),a2=ouf.readToken();
    if(a1=="YE5"){
        if(a2!="YE5"){
            quitf(_wa,"Wrong answer [1], testcase No.%lld \n",Tid);
        }
    }else{
        if(a2!="N0"){
            quitf(_wa,"Wrong answer [1], testcase No.%lld \n",Tid);
        }
        return 0;
    }
    ll optimalmove=ans.readLong();
    ll outmove=ouf.readLong();
    if(optimalmove!=outmove){
        quitf(_wa,"Wrong answer [2], testcase No.%lld \n",Tid);
    }
    s2=ouf.readToken();
    if(s2.length()!=n){
        quitf(_wa,"Wrong answer [3], testcase No.%lld \n",Tid);
    }
    for(int i=0;i<n;i++){
        if(s2[i]!='U' && s2[i]!='L' && s2[i]!='D' && s2[i]!='R'){
            quitf(_wa,"Wrong answer [3], testcase No.%lld \n",Tid);
        }
    }
    ll cnt_dif=0;
    for(int i=0;i<n;i++){
        cnt_dif+=(s1[i]!=s2[i]);
    }
    if(cnt_dif!=optimalmove)return 1;
    ll x=0,y=0;
    for(int i=0;i<n;i++){
        nxt(x,y,s2[i]);
        if(!chkpointvalid(x,y)){
            return 2;
        }
    }
    if(x==0 && y==0)return 0;
    return 3;
}
int main(int argc,char* argv[]){
    registerTestlibCmd(argc, argv);
    ll Test=inf.readLong(),ttid=inf.readLong(),pid=0,wrid=0;
    bool isalltrue=true;
    for(int i=1;i<=Test;i++){
        ll t=solve(i);
        if(t){
            if(!pid){
                pid=i;
                wrid=t;
            }
            isalltrue=false;
        }
    }
    if(isalltrue){
        quitf(_ok,"Accepted in all %lld Testcases! Congratulations!\n",Test);
    }else{
        quitf(_pc(50),"Partially correct [%lld], testcase No.%lld \n",wrid,pid);
    }
}
