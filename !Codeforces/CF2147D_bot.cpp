#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    for (int test = 0; test < t; ++test) {
        int n;
        cin >> n;
        
        map<ll, ll> freq;
        for (int i = 0; i < n; ++i) {
            ll a;
            cin >> a;
            freq[a]++;
        }
        
        vector<pair<ll, ll>> vec;
        for (auto& p : freq) {
            vec.emplace_back(p.second, p.first); // f, v
        }
        
        sort(vec.rbegin(), vec.rend());
        
        ll alice = 0, bob = 0;
        ll cur_pos = 0;
        
        for (auto& pr : vec) {
            ll val = pr.first;
            ll num = pr.second;
            
            ll start = cur_pos + 1;
            ll num_odd;
            if (start % 2 == 1) {
                num_odd = (num + 1) / 2;
            } else {
                num_odd = num / 2;
            }
            ll num_even = num - num_odd;
            
            alice += num_odd * val;
            bob += num_even * val;
            
            cur_pos += num;
        }
        
        cout << alice << " " << bob << "\n";
    }
    
    return 0;
}