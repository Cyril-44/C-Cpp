#include<bits/stdc++.h>
using namespace std;
const int MX = 1e5 + 7,Max_sz = 1e6;
int col[MX],a[MX],hard,testcase = 0,N;
bool mark[MX];
void gen(){
	cin >> N;hard = 1;
	for(int i = 1;i <= N;i++)cin >> col[i];
	for(int i = 1;i <= N;i++)a[i] = col[i];
}
int size(){return N;}
extern int helper(int l, int r) {
    int cnt = 0;
    for (int i = l; i <= r; i++) cnt += a[i] == 1;
    return cnt;
}
int test(vector<int > d){
	if(d.size() > Max_sz){
		cerr << "Query is too big\n";
		exit(0);
	}
	set<int > s;
	int cnt = 0;
	for(int i = 0;i < d.size();i++){
		int x = d[i];
		if(!(x >= 1 && x <= N)){
			cerr << x << " out of range\n";
			exit(0);
		}
		if(s.find(x) == s.end() || !hard)cnt += a[x];
		else{
			cerr << "Used querys!\n";
			exit(0);
		}
		s.insert(x);
	}
	testcase++;
	if(cnt > 0)return 1;
	else if(cnt == 0)return 0;
	else return -1;
}
void Reverse(int l,int r){
	for(int i = l;i <= r;i++){
		if(mark[i]){
			cerr << i << " already reversed\n";
			exit(0);
		}
		mark[i] = true;
		a[i] = -col[i];
	}
}
void answer(int x){
	int cnt = 0;
	for(int i = 1;i <= N;i++)
		if(col[i] == 1)cnt++;
	if(cnt == x){
		cerr << "Correct,testcase = " << testcase << '\n';
		exit(0);
	}
	else{
		cerr << "Incorrect,your answer = " << x << " real answer = " << cnt << '\n';
		exit(0);
	}
}
void solve();
int main(){
	gen();
	solve();
}
