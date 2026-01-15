#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define gcd __gcd
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if(!(cin >> n >> m)) return 0;
    vector<int> h(m+1);
    for(int i=1;i<=m;i++) cin >> h[i];
    vector<int> a(n);
    for(int i=0;i<n;i++) cin >> a[i];

    // map value -> index (1-based index required in output)
    vector<int> idx(m+1, 0);
    for(int i=0;i<n;i++) idx[a[i]] = i+1;

    // present array
    vector<char> present(m+1, 0);
    for(int v: a) present[v]=1;

    // linear sieve for mu and spf up to m
    vector<int> primes;
    vector<int> mu(m+1,0), spf(m+1,0);
    vector<char> is_comp(m+1, 0);
    mu[1] = 1;
    for(int i=2;i<=m;i++){
        if(!is_comp[i]){
            primes.push_back(i);
            mu[i] = -1;
            spf[i] = i;
        }
        for(int p: primes){
            ll v = 1LL*p*i;
            if(v>m) break;
            is_comp[v] = 1;
            spf[v] = p;
            if(i % p == 0){
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    spf[1] = 1;

    // Build list: for each g, store reduced values s = val/g for present val
    vector<vector<int>> mults(m+1);
    for(int val : a){
        // enumerate divisors by iterating g | val via multiples loop is cheaper globally,
        // but building by for g=1..m: for (x=g; x<=m; x+=g) if present[x] ... is also OK.
        // we'll use multiples scan to avoid factoring each val for all divisors.
    }
    // Build using multiples loop:
    for(int g=1; g<=m; ++g){
        for(int x=g; x<=m; x+=g){
            if(present[x]) mults[g].push_back(x / g);
        }
    }

    // Arrays used during per-g processing (global sized m+1), we will touch only small subset and reset via touched vectors
    vector<int> cntTotal(m+1, 0), cntPrev(m+1, 0);
    vector<int> touchedTotal, touchedPrev;

    // helper: get divisors of x via spf factorization
    auto get_divisors = [&](int x){
        vector<pair<int,int>> fac;
        while(x>1){
            int p = spf[x] ? spf[x] : x;
            int c = 0;
            while(x % p == 0){ x/=p; ++c; }
            fac.emplace_back(p,c);
        }
        vector<int> divs = {1};
        for(auto &pc : fac){
            int p = pc.first, c = pc.second;
            int sz = divs.size();
            int mul = 1;
            for(int e=1;e<=c;e++){
                mul *= p;
                for(int i=0;i<sz;i++) divs.push_back(divs[i]*mul);
            }
        }
        return divs;
    };

    // outdeg for value (use 64-bit)
    vector<ll> outdeg(m+1, 0);

    // Process each g
    for(int g=1; g<=m; ++g){
        auto &S = mults[g];
        int sz = (int)S.size();
        if(sz < 2) continue;
        sort(S.begin(), S.end()); // ascending reduced values

        // build cntTotal: for each s in S, for each divisor d of s: cntTotal[d]++
        touchedTotal.clear();
        for(int s : S){
            // get divisors
            auto divs = get_divisors(s);
            for(int d : divs){
                if(cntTotal[d]==0) touchedTotal.push_back(d);
                cntTotal[d] += 1;
            }
        }

        touchedPrev.clear();
        // iterate S in ascending
        int pos = 0;
        for(int s : S){
            // compute coprime_prev = sum_{d|s} mu[d] * cntPrev[d]
            ll coprime_prev = 0;
            auto divs = get_divisors(s);
            for(int d : divs){
                if(cntPrev[d]) coprime_prev += (ll)mu[d] * cntPrev[d];
            }
            // tot_coprime = sum_{d|s} mu[d] * cntTotal[d]
            ll tot_coprime = 0;
            for(int d : divs){
                if(cntTotal[d]) tot_coprime += (ll)mu[d] * cntTotal[d];
            }
            ll later = tot_coprime - coprime_prev;
            int val = g * s;
            if(h[g]==1){
                // larger (current val) wins against those previous with gcd exactly g
                outdeg[val] += coprime_prev;
            } else {
                // smaller (current val) wins against later ones
                outdeg[val] += later;
            }
            // mark current into cntPrev: for each divisor of s
            for(int d : divs){
                if(cntPrev[d]==0) touchedPrev.push_back(d);
                cntPrev[d] += 1;
            }
            pos++;
        }

        // reset cntTotal and cntPrev
        for(int d: touchedTotal) cntTotal[d]=0;
        for(int d: touchedPrev) cntPrev[d]=0;
    }

    // Now outdeg computed for each present value.
    // Compute sum C(outdeg,2)
    __int128 sumC2_128 = 0;
    for(int val : a){
        ll k = outdeg[val];
        if(k >= 2) sumC2_128 += (__int128)k * (k-1) / 2;
    }
    // total triples C(n,3)
    __int128 total_triples = (__int128)n * (n-1) * (n-2) / 6;
    __int128 cycles = total_triples - sumC2_128;
    long long cycles_ll = (long long)cycles; // fits in 64-bit as argued (n<=1e6)

    // Output count and any cyclic triple (we must produce indices x y z such that x->y,y->z,z->x)
    // We'll try to find one triangle.
    auto orientation = [&](int va, int vb)->bool{
        // return true if va -> vb
        if(va == vb) return false;
        int g = std::gcd(va, vb);
        if(h[g]==1) return va > vb;
        else return va < vb;
    };

    // Build list of present values for convenience
    vector<int> vals = a;
    sort(vals.begin(), vals.end()); // ascending values

    // Strategy: try some deterministic candidates first (first K nodes), else random search.
    int Kdet = min(n, 1000);
    int found = 0;
    int rx= -1, ry=-1, rz=-1;
    auto try_node_find = [&](int val)->bool{
        // choose node 'val', collect small lists of outneighbors and inneighbors (limit to L)
        const int L = 2000;
        vector<int> outn, inn;
        for(int v : vals){
            if(v==val) continue;
            if(orientation(val, v)){
                outn.push_back(v);
                if((int)outn.size() > L) break;
            }
        }
        for(int v : vals){
            if(v==val) continue;
            if(orientation(v, val)){
                inn.push_back(v);
                if((int)inn.size() > L) break;
            }
        }
        if(outn.empty() || inn.empty()) return false;
        // test pairs
        for(int v: outn){
            for(int w: inn){
                if(orientation(v,w)){
                    // val -> v -> w -> val
                    rx = idx[val]; ry = idx[v]; rz = idx[w];
                    return true;
                }
            }
        }
        return false;
    };

    for(int i=0;i<Kdet && !found;i++){
        if(try_node_find(vals[i])){
            found=1; break;
        }
    }
    // try some nodes near middle
    for(int i=0;i<Kdet && !found;i++){
        if(try_node_find(vals[n/2 + i < n ? n/2 + i : n-1])){ found=1; break; }
    }
    // random attempts
    std::mt19937_64 rng(1234567);
    for(int t=0; t<2000 && !found; ++t){
        int r = rng() % n;
        if(try_node_find(vals[r])){ found=1; break; }
    }

    // As guaranteed by problem, there exists at least one triangle.
    if(!found){
        // fallback brute force on small subset (should not happen on judge tests)
        for(int i=0;i<min(n,2000) && !found;i++){
            for(int j=i+1;j<min(n,2000) && !found;j++){
                for(int k=j+1;k<min(n,2000) && !found;k++){
                    int va=vals[i], vb=vals[j], vc=vals[k];
                    // check the 3-cycle in either orientation
                    if(orientation(va,vb) && orientation(vb,vc) && orientation(vc,va)){
                        rx=idx[va]; ry=idx[vb]; rz=idx[vc]; found=1; break;
                    }
                    if(orientation(va,vc) && orientation(vc,vb) && orientation(vb,va)){
                        rx=idx[va]; ry=idx[vc]; rz=idx[vb]; found=1; break;
                    }
                }
            }
        }
    }

    // print
    cout << cycles_ll << '\n';
    if(found){
        cout << rx << ' ' << ry << ' ' << rz << '\n';
    } else {
        // should not happen per statement, but to be safe output first three indices
        cout << idx[vals[0]] << ' ' << idx[vals[1]] << ' ' << idx[vals[2]] << '\n';
    }
    return 0;
}
