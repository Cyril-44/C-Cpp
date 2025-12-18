#include<bits/stdc++.h>
using namespace std;
int n,x;
int main(){
	freopen("1.in","w",stdout);
	srand(time(NULL));
	n=500000;
	cout<<n<<endl;
	for(int i=1;i<=n;i++){
		x=rand()%200000-100000;
		printf("%d ",x);
	}
	cout<<endl;
	for(int i=1;i<=n;i++){
		x=rand()%200000-100000;
		printf("%d ",x);
	}
    return 0;
}