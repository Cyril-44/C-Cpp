#include <bits/stdc++.h>
using namespace std;
#if defined(ONLINE_JUDGE)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
#endif

constexpr int N = 5000, Q = 30000, K = 1000;
template<typename bb>constexpr bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>||requires(typename kb::value_type mb,typename kb::value_type nb){{mb+nb}->std::same_as<typename kb::value_type>;{mb-nb}->std::same_as<typename kb::value_type>;{mb*nb}->std::same_as<typename kb::value_type>;{mb%nb}->std::same_as<typename kb::value_type>;{mb+=nb}->std::same_as<typename kb::value_type&>;{mb-=nb}->std::same_as<typename kb::value_type&>;{mb*=nb}->std::same_as<typename kb::value_type&>;{mb%=nb}->std::same_as<typename kb::value_type&>;requires std::is_default_constructible_v<typename kb::value_type>;requires std::is_copy_constructible_v<typename kb::value_type>;requires std::is_move_constructible_v<typename kb::value_type>;requires std::is_assignable_v<typename kb::value_type&,int>;requires std::equality_comparable<typename kb::value_type>;}class MI{using ob=kb::value_type;inline static constexpr ob pb(){return kb::value;}ob qb;template<typename sb>inline constexpr ob rb(sb tb){if constexpr(std::is_unsigned_v<sb>)return static_cast<ob>(tb<pb()?tb:tb%pb());else{ob ub=static_cast<ob>(-pb()<tb&&tb<pb()?tb:tb%pb());return(ub<0?ub+pb():ub);}}public:constexpr inline MI():qb(lb){}template<typename vb>inline constexpr MI(const vb&wb){qb=rb(wb);}template<typename xb>inline explicit constexpr operator xb()const{return static_cast<xb>(qb);}inline constexpr ob operator()()const{return qb;}inline constexpr MI&operator+=(const MI&yb){if((qb+=yb.qb)>=pb())qb-=pb();return*this;}inline constexpr MI&operator-=(const MI&zb){if((qb-=zb.qb)<0)qb+=pb();return*this;}inline constexpr MI&operator*=(const MI&Ab){if constexpr(std::is_same_v<ob,int>)qb=rb((uint64_t)qb*Ab.qb);else if constexpr(std::is_same_v<ob,int64_t>)qb=rb((unsigned __int128)qb*Ab.qb);else qb=rb(qb*Ab.qb);return*this;}inline constexpr MI&operator/=(const MI&Bb){return*this*=MI(modInv(Bb.qb,pb()));}template<std::integral Cb>inline constexpr MI&operator^=(Cb Db){if(Db<0)return*this=MI(modInv(static_cast<ob>(1),(*this^(-Db))()));MI Eb=*this;for(*this=static_cast<ob>(1);Db;Db>>=1){if(Db&1)*this*=Eb;Eb*=Eb;}return*this;}inline constexpr MI operator-()const{return MI(-qb);}inline constexpr MI&operator++(){return*this+=1;}inline constexpr MI&operator--(){return*this-=1;}inline constexpr MI operator++(int){MI Fb=*this;++*this;return Fb;}inline constexpr MI operator--(int){MI Gb=*this;--*this;return Gb;}inline constexpr MI operator+(const MI&Hb)const{return MI(*this)+=Hb;}inline constexpr MI operator-(const MI&Ib)const{return MI(*this)-=Ib;}inline constexpr MI operator*(const MI&Jb)const{return MI(*this)*=Jb;}inline constexpr MI operator/(const MI&Kb)const{return MI(*this)/=Kb;}template<std::integral Lb>inline constexpr MI operator^(const Lb Mb)const{return MI(*this)^=Mb;}inline constexpr bool operator==(const MI&Nb)const{return qb==Nb.qb;}inline constexpr bool operator!=(const MI&Ob)const{return qb!=Ob.qb;}inline constexpr bool operator!()const{return!qb;}template<typename Pb>inline constexpr friend MI operator+(const Pb&Qb,const MI&Rb){return MI(Qb)+Rb;}template<typename Sb>inline constexpr friend MI operator-(const Sb&Tb,const MI&Ub){return MI(Tb)-Ub;}template<typename Vb>inline constexpr friend MI operator*(const Vb&Wb,const MI&Xb){return MI(Wb)*Xb;}template<typename Yb>inline constexpr friend MI operator/(const Yb&Zb,const MI&_b){return MI(Zb)/_b;}template<typename ac>inline constexpr friend MI operator==(const ac&bc,const MI&cc){return MI(bc)==cc;}template<typename dc>inline constexpr friend MI operator!=(const dc&ec,const MI&fc){return MI(ec)!=fc;}template<typename gc>inline constexpr friend MI operator<=(const gc&hc,const MI&ic){return MI(hc)<=ic;}template<typename jc>inline constexpr friend MI operator>=(const jc&kc,const MI&lc){return MI(kc)>=lc;}template<typename mc>inline constexpr friend MI operator<(const mc&nc,const MI&oc){return MI(nc)<oc;}template<typename pc>inline constexpr friend MI operator>(const pc&qc,const MI&rc){return MI(qc)>rc;}template<typename sc>inline friend sc&operator>>(sc&tc,MI&uc){tc>>uc.qb;uc.qb=uc.normalize(uc.qb);return tc;}template<typename vc>friend inline vc&operator<<(vc&wc,const MI&xc){return wc<<xc.qb;}};
constexpr auto MOD = (int)1e9 + 7;
using Mint = MI<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
constexpr Mint P = (int)1e7 + 19;

using Dp = array<uint32_t, K+1>;
using Obj = pair<uint32_t, uint16_t>;
Obj a[N+Q+1];
uint16_t avalrg[N+Q+1][2];
uint16_t tot;
int n, k;
Mint pwP[K+1];
inline void mult(Dp &f, Obj x) {
    for (uint16_t i = k; i >= x.second; i--)
        f[i] = std::max(f[i], f[i - x.second] + x.first);
}
inline Mint calc(const Dp &f) {
    Mint res = 0;
    for (int i = 1; i <= k; i++)
        res += f[i] * pwP[i - 1];
    return res;
}
namespace SegDivide {
    vector<Obj> mods[Q+1 << 2];
    Obj X;
    uint16_t L, R;
    void _modify(uint16_t u, uint16_t l, uint16_t r) {
        if (L <= l && r <= R) mods[u].push_back(X);
        else {
            uint16_t mid = l + r >> 1;
            if (L <= mid) _modify(u<<1, l, mid);
            if (mid < R) _modify(u<<1|1, mid+1, r);
        }
    }
    inline void modify(uint16_t l, uint16_t r, Obj o) {
        L = l, R = r, X = o;
        assert(X.second > 0);
        _modify(1, 1, tot);
    }
    void solve(uint16_t u = 1, uint16_t l = 1, uint16_t r = tot, Dp f = {}) {
        for (auto obj : mods[u]) mult(f, obj);
        if (l == r) {
            auto res = calc(f);
            cout << res << '\n';
        } else {
            auto mid = l + r >> 1;
            solve(u<<1, l, mid, f);
            solve(u<<1|1, mid+1, r, f);
        }
    }
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> a[i].first >> a[i].second;
        avalrg[i][1] = -1;
    }
    pwP[0] = 1;
    for (int i = 1; i <= k; i++) pwP[i] = pwP[i-1] * P;
    int q;
    cin >> q;
    for (int op, x, _q = q; _q--; ) {
        cin >> op;
        switch (op) {
            case 1:
                ++n;
                cin >> a[n].first >> a[n].second;
                avalrg[n][0] = tot;
                avalrg[n][1] = -1;
                break;
            case 2:
                cin >> x;
                avalrg[x][1] = tot;
                break;
            case 3:
                ++tot;
        }
    }
    for (int i = 1; i <= n; i++)
        if (avalrg[i][1] == -1)
            avalrg[i][1] = tot;
    for (int i = 1; i <= n; i++)
        if (avalrg[i][0] ^ avalrg[i][1])
            SegDivide::modify(avalrg[i][0] + 1, avalrg[i][1], a[i]);
    SegDivide::solve();
    return 0;
}