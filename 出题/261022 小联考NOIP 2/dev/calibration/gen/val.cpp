#include "testlib.h"
#include <algorithm>
#include <cstdio>
#include <string>
using namespace std;
int main(int argc,char**argv) {
    registerValidation(argc,argv);
    const int P=998244353,LIMIT=100000;
    int n=inf.readInt(1,LIMIT,"n");inf.readEoln();
    for(int i=1;i<=n;++i){inf.readInt(0,P-1,"a_i");if(i<n)inf.readSpace();else inf.readEoln();}
    int m=inf.readInt(1,LIMIT,"m");inf.readEoln();
    bool has_cal=false,has_general=false;int queries=0;
    for(int i=1;i<=m;++i){
        int op=inf.readInt(1,3,"op");inf.readSpace();
        int l=inf.readInt(1,n,"l");inf.readSpace();int r=inf.readInt(l,n,"r");
        if(op==1){ensuref(l<r,"calibration needs l<r, operation %d",i);inf.readSpace();int k=inf.readInt(1,r-l,"k");has_cal=true;has_general|=k>1;}
        else if(op==2){inf.readSpace();inf.readInt(0,P-1,"u");inf.readSpace();inf.readInt(0,P-1,"v");}
        else ++queries;
        inf.readEoln();
    }
    inf.readEof();ensuref(queries>0,"at least one query is required");
    int N=max(n,m),actual=N<=1000?1:!has_cal?2:!has_general?3:N<=10000?4:5;
    string group=validator.group();
    if(!group.empty()){ensuref(group.size()==1&&group[0]>='1'&&group[0]<='5',"group must be 1..5");ensuref(actual==group[0]-'0',"expected group %s, actual group %d",group.c_str(),actual);}
    fprintf(stderr,"OK group=%d n=%d m=%d queries=%d\n",actual,n,m,queries);
}
