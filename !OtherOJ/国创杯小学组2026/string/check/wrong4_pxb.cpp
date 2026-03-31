#include<bits/stdc++.h>
using namespace std;
int t,n,top,ans;
char s[5000005],st[5000005];
int main(){
    cin>>t;
    while(t--){
        scanf("%s",s+1);
        n=strlen(s+1);
        bool ok=1,ok2=0;
        ans=top=0;
        s[n+1]='$';
        for(int i=1;i<=n+1;i++){
            if(ok&&s[i]>=st[top]){
                st[++top]=s[i];
            }
            else{
                ok=0;
                if(!ok2){
                    while(top&&st[top-1]==st[top]) top--;
                    top--;
                    ok2=1;
                }
                if(st[top]==s[i]){
                    top--;
                    if(!top){
                        ans++;
                        ok=1;
                        ok2=0;
                    }
                }
                else{
                    ans++;
                    if(top) ans++;
                    for(int j=top-1;j>=1;j--){
                        if(st[j]!=st[j+1]){
                            ans++;
                        }
                    }
                    top=0;
                    ok=1;
                    ok2=0;
                    st[top=1]=s[i];
                }
            }
        }
        if(top) ans++;
        for(int j=top-1;j>=1;j--){
            if(st[j]!=st[j+1]){
                ans++;
            }
        }
        cout<<ans-1<<endl;
    }
    return 0;
}