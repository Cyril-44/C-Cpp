#include<bits/stdc++.h>
using namespace std;
int t,n,top,ans;
char s[5000005],st[5000005];
int main(){
    int C;
    cin>>t;
    while(t--){
        scanf("%s",s+1);
        n=strlen(s+1);
        bool ok=1,ok2=0;
        ans=top=0;
        s[n+1]='$';
        int bad=0,fix=0;
        // 旧结构1：ch1 ch1 ch2 ch1 ch2 ch1（aababa 类）
        for(int i=2;i<=n-4;i++){
            if(s[i]==s[i-1]){
                char ch1=s[i];
                char ch2=s[i+1];
                if(ch2!=ch1&&s[i+2]==ch1&&s[i+3]==ch2&&s[i+4]==ch1){
                    bool continues=(i+5<=n&&s[i+5]==ch2&&i+6<=n&&s[i+6]==ch1);
                    if(!continues){
                        bad++;
                    }
                }
            }
        }
        // 旧结构2：ch2 ch1 ch1 ch2 ch1（baaba 类）
        for(int i=1;i<=n-4;i++){
            char ch2=s[i];
            char ch1=s[i+1];
            if(ch2!=ch1&&s[i+2]==ch1&&s[i+3]==ch2&&s[i+4]==ch1){
                bool continues=(i+5<=n&&s[i+5]==ch2&&i+6<=n&&s[i+6]==ch1);
                if(!continues){
                    bad++;
                }
            }
        }
        // 新结构3：ch1 ch2 ch2 ch1 ch2（abbab 类，如 aaaaaaaaaaaabbab 中的 abbab 部分）
        for(int i=1;i<=n-4;i++){
            char ch1=s[i];
            char ch2=s[i+1];
            if(ch2!=ch1&&s[i+2]==ch2&&s[i+3]==ch1&&s[i+4]==ch2){
                bool continues=(i+5<=n&&s[i+5]==ch1&&i+6<=n&&s[i+6]==ch2);
                if(!continues){
                    fix++;
                }
            }
        }
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
        cout<<ans-1-bad+fix<<endl;
    }
    return 0;
}