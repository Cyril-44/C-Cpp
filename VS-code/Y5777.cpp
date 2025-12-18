#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> p;
int t,n,m;
set<p> s;
set<p>::iterator it,it2,it3;
struct node{
    int l,r;
}a[10005];
int main(){
    // cin>>t;
    t=1;
    while(t--){
        scanf("%d%d",&n,&m);
        s.clear();
        bool ok=1;
        for(int i=1;i<=n;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            if(y-x<m) ok=0;
            s.insert(make_pair(x,y));
        }
        if(!ok){
            puts("no");
            continue;
        }
        int l=0;
        for(int i=1;i<=n;i++){
            l=max(l,s.begin()->first);
            it=s.upper_bound(make_pair(l,2e9));
            int minr=1e9;
            for(it2=s.begin();it2!=it;it2++){
                if(minr>it2->second){
                    minr=it2->second;
                    it3=it2;
                }
            }
            if(l+m<=minr){
                a[i]=(node){l,minr};
                l+=m;
                s.erase(it3);
            }
            else{
                for(int j=1;j<i;j++){
                    if(a[j].l<it3->first&&a[j].l+m>it3->first){
                        if(it3->first+m+m<=a[j].r){
                            a[i]=(node){it3->first,minr};
                            a[j]=(node){it3->first+m,a[j].r};
                            l=it3->first+m+m;
                        }
                        else{
                            ok=0;
                        }
                        break;
                    }
                }
                if(!ok) break;
                else s.erase(it3);
            }
        }
        if(ok) puts("yes");
        else puts("no");
    }
	return 0;
}