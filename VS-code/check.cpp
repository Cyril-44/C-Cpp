#include<bits/stdc++.h>
using namespace std;
int main(){
    for(int i=1;;i++){
        cout<<i<<endl;
        system("./rand");
        system("./Y5780");
        system("./Y5780_2");
        if(system("diff -b 1.out 2.out")){
            cout<<"NO";
            return 0;
        }
        puts("YES");
    }
    return 0;
}