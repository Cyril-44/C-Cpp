#include <bits/stdc++.h>
#define int long long
#define pii pair<int,int>
#define fi first
#define se second 
using namespace std;
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

vector<array<int,3>> q; 

int query(vector<int> &v) {
    cout << "? ";
    for (int r:v) cout << r << " "; cout << endl;
    int x; cin >> x;
    return x; 
}

int f(int l, int r, vector<int> &v, vector<int> pl, vector<int> pr) {
    if (l==r) return l;
    for (int i = l; i <= r; i++) v[i]=i+1; 
    int a=(pl.size()>0?pl[0]:query(v)-1); 
    for (int i = l; i <= r; i++) v[i]=l+r+1-i;
    int b=(pr.size()>0?pr[0]:l+r-query(v)+1); 
    int t=1;
    int j=1; 
    vector<int> nl, nr; 
    if (a<b) {
        nl.push_back(a); 
        t=0; 
        for (int i = l; i <= r; i++) v[i]=i+1; 
        while (1) {
            if (b-a==1) break; 
            v[a]=22;
            // for (int i = a+1; i < b; i++) v[i]--;
			// assert(pl[j] < b);
			// if (j < pl.size()) assert(pl[j] < b);
            int x=(j<pl.size()?v[pl[j++]]:query(v));
            // if (v[a]==x) break; 
			bool flag = 0;
            for (int i = a+1; i < b; i++) if (v[i]==x) a=i, flag = 1;
			if (!flag) break;
            nl.push_back(a); 
        }
    } 
    else {
        swap(a,b); 
        nr.push_back(a); 
        while (1) {
            if (b-a==1) break; 
            v[a]=-22;
            // for (int i = a+1; i < b; i++) v[i]++; 
            int x=(j<pr.size()&&pr[j]<b?v[pr[j++]]:query(v));
            // if (v[a]==x) break; 
			bool flag = 0;
            for (int i = a+1; i < b; i++) if (v[i]==x) a=i, flag = 1;
			if (!flag) break;
            nr.push_back(a); 
        }
    }
    for (int i = a+1; i <= r; i++) v[i]=(t?1:1e9); 
    if (t==1) {
        for (int z:pl) {
            if (z<=a) nl.push_back(z); 
        }
    }
    else {
        for (int z:pr) {
            if (z<=a) nr.push_back(z); 
        }
    }
    int x=f(l,a,v,nl,nr); 
    for (int i = l; i <= a; i++) v[i]=(t?1:1e9); 
    nl.clear(); nr.clear(); 
    for (int z:pl) {
        if (z>a) nl.push_back(z); 
    }
    for (int z:pr) {
        if (z>a) nr.push_back(z); 
    }
    int y=f(a+1,r,v,nl,nr); 
    q.push_back({x,y,t}); 
    return x; 
}

void solve() {
    int n; cin >> n; 
    q.clear(); 
    vector<int> v(n),w; 
    int a=f(0,n-1,v,w,w); 
    cout << "!" << endl; 
    vector<int> b(n); 
    while (1) {
        cin >> b[0]; 
        if (!b[0]) return; 
        for (int i = 1; i < n; i++) cin >> b[i]; 
        for (auto r:q) {
            b[r[0]]=(r[2]?max(b[r[0]],b[r[1]]):min(b[r[0]],b[r[1]])); 
        }
        cout << b[a] << endl; 
    }
}

int32_t main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int t=1; cin >> t;
    while (t--) solve(); 
}