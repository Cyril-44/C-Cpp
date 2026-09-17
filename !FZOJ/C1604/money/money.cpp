#pragma GCC optimize("O3,unroll-loops,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 2, LM = 500000, INF = 0x3f3f3f3f;
#else
constexpr int N = 250004, LM = 500000, INF = 0x3f3f3f3f;
#endif
inline void umin(int &x, int y) { if (x > y) x = y; }
namespace fasti{using namespace std;const int bufl=1<<20;const double base1[16]={1,1e-1,1e-2,1e-3,1e-4,1e-5,1e-6,1e-7,1e-8,1e-9,1e-10,1e-11,1e-12,1e-13,1e-14,1e-15};const double base2[16]={1,1e1,1e2,1e3,1e4,1e5,1e6,1e7,1e8,1e9,1e10,1e11,1e12,1e13,1e14,1e15};struct IN{FILE*IT;char ibuf[bufl],*is=ibuf,*it=ibuf;IN(){IT=stdin;}IN(char*a){IT=fopen(a,"r");}inline char getChar(){if(is==it){it=(is=ibuf)+fread(ibuf,1,bufl,IT);if(is==it)return EOF;}return*is++;}template<typename Tp>inline void getInt(Tp&a){a=0;int b=0,c=getChar();while(c<48||c>57)b^=(c==45),c=getChar();while(c>=48&&c<=57)a=(a<<1)+(a<<3)+c-48,c=getChar();if(b)a=-a;}template<typename Tp>inline void getDouble(Tp&a){a=0;int b=0,c=getChar(),d=0;__int128 e=0,f=0;while(c<48||c>57)b^=(c==45),c=getChar();while(c>=48&&c<=57)e=(e<<1)+(e<<3)+c-48,c=getChar();if(c==46){c=getChar();while(c>=48&&c<=57)d++,f=(f<<1)+(f<<3)+c-48,c=getChar();}a=e+base1[d]*f;if(b)a=-a;}IN&operator>>(char&a){a=getChar();return*this;}IN&operator>>(char*a){do{*a=getChar();}while(*a<=32);while(*a>32)*++a=getChar();*a=0;return*this;}IN&operator>>(string&a){a="0";char b=getChar();while(b<=32)b=getChar();while(b>32)a+=b,b=getChar();return*this;}IN&operator>>(int&a){getInt(a);return*this;}IN&operator>>(long long&a){getInt(a);return*this;}IN&operator>>(unsigned long long&a){getInt(a);return*this;}IN&operator>>(__int128&a){getInt(a);return*this;}IN&operator>>(float&a){getDouble(a);return*this;}IN&operator>>(double&a){getDouble(a);return*this;}IN&operator>>(long double&a){getDouble(a);return*this;}};}fasti::IN fin;
namespace fasto{char buf[1<<20],*p3=buf;const char*p4=buf+sizeof(buf);inline void pc(char c){if(p3==p4)fwrite(buf,sizeof buf,1,stdout),p3=buf;*p3++=c;}inline void println(int v){if(v==-1){pc('-'),pc('1'),pc('\n');return;}int8_t sta[20];sta[sta[0]=1]=v%10|'0';while(v/=10)sta[++sta[0]]=v%10|'0';while(sta[0])pc(sta[sta[0]--]);pc('\n');}inline void flush(){fwrite(buf,p3-buf,1,stdout);p3=buf;}}using fasto::println;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = (int)1e9 + 7;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;

struct Fenwick2D {
    int n;
    std::vector<int> xs;
    std::vector<std::vector<int>> ys, t;
    std::vector<std::pair<int*, int>> hist;
    std::vector<size_t> stk;
    // precomputed update paths: all_up[k] = {node_i, y_rank}, up_st[id] .. up_st[id+1]
    std::vector<std::pair<int, int>> all_up;
    std::vector<int> up_st;
    std::vector<int> vals; // precomputed signed value per point id

    void init(const std::vector<std::pair<int, int>>& tpts, const std::vector<int>& v) {
        n = 0;
        xs.clear(); ys.clear(); t.clear();
        hist.clear(); stk.clear();
        all_up.clear(); up_st.clear(); vals = v;
        if (tpts.empty()) {
            up_st.assign(1, 0);
            return;
        }
        const int m = (int)tpts.size();
        xs.reserve(m);
        for (auto [x, y] : tpts) xs.push_back(x);
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        n = (int)xs.size();
        ys.resize(n + 1);
        for (int p = 0; p < m; p++) {
            int x = tpts[p].first, y = tpts[p].second;
            int rx = (int)(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
            for (int i = rx; i <= n; i += i & -i) ys[i].push_back(y);
        }
        t.resize(n + 1);
        for (int i = 1; i <= n; i++) {
            auto& yv = ys[i];
            std::sort(yv.begin(), yv.end());
            yv.erase(std::unique(yv.begin(), yv.end()), yv.end());
            t[i].assign(yv.size() + 1, INF);
        }
        // precompute paths
        up_st.resize(m + 1);
        all_up.reserve(m * 20);
        for (int p = 0; p < m; p++) {
            up_st[p] = (int)all_up.size();
            int x = tpts[p].first, y = tpts[p].second;
            int rx = (int)(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
            for (int i = rx; i <= n; i += i & -i) {
                int ry = (int)(std::lower_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin()) + 1;
                all_up.emplace_back(i, ry);
            }
        }
        up_st[m] = (int)all_up.size();
    }

    void add(int id) {
        stk.push_back(hist.size());
        if (n == 0) return;
        int v = vals[id];
        int ed = up_st[id + 1];
        for (int k = up_st[id]; k < ed; k++) {
            int i = all_up[k].first, j = all_up[k].second;
            for (; j < (int)t[i].size(); j += j & -j) {
                hist.emplace_back(&t[i][j], t[i][j]);
                umin(t[i][j], v);
            }
        }
    }

    int qry(int x, int y) {
        if (n == 0) return INF;
        int res = INF;
        int rx = (int)(std::upper_bound(xs.begin(), xs.end(), x) - xs.begin());
        for (int i = rx; i; i -= i & -i) {
            int ry = (int)(std::upper_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin());
            for (int j = ry; j; j -= j & -j) umin(res, t[i][j]);
        }
        return res;
    }

    void rollback(size_t sz) {
        while (hist.size() > sz) {
            auto [p, old] = hist.back();
            hist.pop_back();
            *p = old;
        }
        // also pop stk entries that belong to this group (may be multiple adds)
        while (!stk.empty() && stk.back() >= sz) stk.pop_back();
    }

    size_t hist_size() const { return hist.size(); }
};

class MyTr {
    Fenwick2D ds[2][2][2];
public:
    void init(const std::vector<std::pair<int, int>> pts[2]) {
        for (int o = 0; o < 2; o++) {
            const int m = (int)pts[o].size();
            for (int sx = 0; sx < 2; sx++) {
                for (int sy = 0; sy < 2; sy++) {
                    std::vector<std::pair<int, int>> tpts(m);
                    std::vector<int> vals(m);
                    for (int p = 0; p < m; p++) {
                        int x = pts[o][p].first, y = pts[o][p].second;
                        tpts[p] = {sx ? -x : x, sy ? -y : y};
                        vals[p] = x * (sx ? 1 : -1) + y * (sy ? 1 : -1);
                    }
                    ds[o][sx][sy].init(tpts, vals);
                }
            }
        }
    }
    // bulk helpers
    void save(size_t sav[2][2][2]) {
        for (int o = 0; o < 2; o++)
            for (int sx = 0; sx < 2; sx++)
                for (int sy = 0; sy < 2; sy++)
                    sav[o][sx][sy] = ds[o][sx][sy].hist_size();
    }
    void restore(const size_t sav[2][2][2]) {
        for (int o = 0; o < 2; o++)
            for (int sx = 0; sx < 2; sx++)
                for (int sy = 0; sy < 2; sy++)
                    ds[o][sx][sy].rollback(sav[o][sx][sy]);
    }
    void add(int o, int id) {
        for (int sx = 0; sx < 2; sx++)
            for (int sy = 0; sy < 2; sy++)
                ds[o][sx][sy].add(id);
    }
    int inquire(int xl, int xr, int yl, int yr, int o, int sx, int sy) {
        int qx = sx ? -xl : xr;
        int qy = sy ? -yl : yr;
        return ds[o][sx][sy].qry(qx, qy);
    }
} f;

int n;
std::vector<std::pair<int, int>> pts[2];

struct SegTr {
    using Point = std::pair<int, int>; // o, id
    std::vector<Point> tr[N << 2];
    int L, R;
    Point X;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) {
            tr[u].push_back(X);
            return;
        }
        int mid = l + r >> 1;
        if (L <= mid) upd(u << 1, l, mid);
        if (mid < R) upd(u << 1 | 1, mid + 1, r);
    }
    void insert(int l, int r, int o, int id) {
        L = l; R = r; X = {o, id};
        upd(1, 1, n);
    }
    void run(int u = 1, int l = 1, int r = n, int ans = INF) {
        size_t sav[2][2][2];
        f.save(sav);
        for (const auto& [o, id] : tr[u]) {
            int x = pts[o][id].first, y = pts[o][id].second;
            umin(ans, x + y + f.inquire(-LM, x, -LM, y, !o, 0, 0));
            umin(ans, x - y + f.inquire(-LM, x, y, LM, !o, 0, 1));
            umin(ans, -x + y + f.inquire(x, LM, -LM, y, !o, 1, 0));
            umin(ans, -x - y + f.inquire(x, LM, y, LM, !o, 1, 1));
            f.add(o, id);
        }
        if (l == r) {
            println(ans > INF - N * 2 ? -1 : ans / 2);
        } else {
            int mid = l + r >> 1;
            run(u << 1, l, mid, ans);
            run(u << 1 | 1, mid + 1, r, ans);
        }
        f.restore(sav);
    }
} fd;

int main() {
    fin >> n;
    std::map<std::pair<int, int>, std::pair<int, int>> mp[2];
    for (int op, k, x, y, i = 1; i <= n; i++) {
        fin >> op >> k >> x >> y;
        --k;
        int px = (x + y) * (k ? 1 : -1), py = (x - y) * (k ? 1 : -1);
        if (op == 1) {
            if (!mp[k][{px, py}].second++) mp[k][{px, py}].first = i;
        } else {
            if (!--mp[k][{px, py}].second) {
                int id = (int)pts[k].size();
                pts[k].emplace_back(px, py);
                fd.insert(mp[k][{px, py}].first, i - 1, k, id);
                mp[k].erase({px, py});
            }
        }
    }
    for (const auto& [c, x] : mp[0]) {
        int id = (int)pts[0].size();
        pts[0].emplace_back(c.first, c.second);
        fd.insert(x.first, n, 0, id);
    }
    for (const auto& [c, x] : mp[1]) {
        int id = (int)pts[1].size();
        pts[1].emplace_back(c.first, c.second);
        fd.insert(x.first, n, 1, id);
    }
    f.init(pts);
    fd.run();
    fasto::flush();
    return 0;
}