#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; 
    cin >> T;
    while(T--){
        int n;
        cin >> n;
        vector<ll> x(n), y(n), u(n), v(n);
        for(int i = 0; i < n; i++){
            cin >> x[i] >> y[i];
            u[i] = x[i] + y[i];
            v[i] = x[i] - y[i];
        }

        set<pair<ll,int>> set_u, set_v;
        for(int i = 0; i < n; i++){
            set_u.insert({u[i], i});
            set_v.insert({v[i], i});
        }

        vector<pair<int,int>> ans;
        ans.reserve(n/2);

        while(!set_u.empty()){
            // extremal in u
            auto it_u_min = set_u.begin();
            auto it_u_max = prev(set_u.end());
            ll minu = it_u_min->first, idx_u_min = it_u_min->second;
            ll maxu = it_u_max->first, idx_u_max = it_u_max->second;

            // extremal in v
            auto it_v_min = set_v.begin();
            auto it_v_max = prev(set_v.end());
            ll minv = it_v_min->first, idx_v_min = it_v_min->second;
            ll maxv = it_v_max->first, idx_v_max = it_v_max->second;

            ll du = maxu - minu;
            ll dv = maxv - minv;

            int a, b;
            if(du >= dv){
                a = idx_u_min;
                b = idx_u_max;
            } else {
                a = idx_v_min;
                b = idx_v_max;
            }

            // record (1-based)
            ans.emplace_back(a+1, b+1);

            // erase from both sets
            set_u.erase({u[a], a});
            set_v.erase({v[a], a});
            set_u.erase({u[b], b});
            set_v.erase({v[b], b});
        }

        // output pairs
        for(auto &p : ans){
            cout << p.first << " " << p.second << "\n";
        }
    }

    return 0;
}
