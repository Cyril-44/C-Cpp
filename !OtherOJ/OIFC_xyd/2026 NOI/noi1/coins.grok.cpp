#include<bits/stdc++.h>
using namespace std;template<typename T>struct is_i128:std::false_type{};template<>struct is_i128<__int128_t>:std::true_type{};template<>struct is_i128<__uint128_t>:std::true_type{};template<typename T>struct is_integer_with_i128{static constexpr bool value=std::is_integral<T>::value||is_i128<T>::value;};class FastInputStream{static constexpr size_t db=1<<20;char eb[db];char*fb=nullptr,*gb=nullptr;FILE*hb;public:FastInputStream(FILE*ib=stdin):hb(ib){}inline char get(){if(fb==gb){fb=eb;gb=eb+fread(eb,1,db,hb);if(fb==gb)return EOF;}return*fb++;}FastInputStream&operator>>(bool&kb){int lb=get();while(~lb&&lb!='0'&&lb!='1')lb=get();kb=(lb=='1');return*this;}FastInputStream&operator>>(char&mb){mb=get();while(mb==' '||mb=='\r'||mb=='\n'||mb=='\t')mb=get();return*this;}template<typename nb>typename std::enable_if<is_integer_with_i128<nb>::value,FastInputStream&>::type operator>>(nb&ob){int pb=get();bool qb=false;while(~pb&&(pb<'0'||pb>'9')&&pb!='-')pb=get();if(std::is_signed<nb>::value||is_i128<nb>::value){if(pb=='-'){qb=true;pb=get();}}ob=0;while(pb>='0'&&pb<='9'){ob=(ob<<3)+(ob<<1)+(pb^'0');pb=get();}if(qb)ob=-ob;return*this;}template<typename rb>typename std::enable_if<std::is_floating_point<rb>::value,FastInputStream&>::type operator>>(rb&sb){int tb=get();bool ub=false;while(~tb&&(tb<'0'||tb>'9')&&tb!='-')tb=get();if(tb=='-'){ub=true;tb=get();}__uint128_t vb=0;while(tb>='0'&&tb<='9'){vb=(vb<<3)+(vb<<1)+(tb^'0');tb=get();}sb=static_cast<rb>(vb);if(tb=='.'){rb wb=1.0;for(tb=get();tb>='0'&&tb<='9';tb=get()){sb+=(tb^'0')*(wb/=10.0);}}if(ub)sb=-sb;return*this;}FastInputStream&operator>>(char*xb){int yb=get();while(~yb&&yb<=' ')yb=get();while(~yb&&yb>' ')*xb++=yb,yb=get();*xb='\0';return*this;}};class FastOutputStream{static constexpr size_t Bb=1<<20;char Cb[Bb],*Db=Cb;FILE*Eb;long double Fb=5e-6;unsigned char Gb=6;public:FastOutputStream(FILE*Hb=stdout):Eb(Hb){setvbuf(Eb,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Cb,1,Db-Cb,Eb);Db=Cb;}inline void put(char Kb){if(Db==Cb+Bb)flush();*Db++=Kb;}unsigned char precision()const{return Gb;}FastOutputStream&precision(int Mb){Gb=Mb;Fb=0.5;while(Mb--)Fb*=0.1;return*this;}FastOutputStream&operator<<(char Nb){put(Nb);return*this;}FastOutputStream&operator<<(const char*Ob){while(*Ob)put(*Ob++);return*this;}FastOutputStream&operator<<(const std::string&Pb){for(char c:Pb)put(c);return*this;}template<typename Qb>typename std::enable_if<is_integer_with_i128<Qb>::value,FastOutputStream&>::type operator<<(Qb Rb){if(Rb==0){put('0');return*this;}Qb Sb=Rb;if(std::is_signed<Qb>::value||is_i128<Qb>::value){if(Sb<0){put('-');Sb=-Sb;}}static char Tb[64];int Ub=0;while(Sb){Tb[Ub++]=char((Sb%10)^'0');Sb/=10;}while(Ub)put(Tb[--Ub]);return*this;}template<typename Vb>typename std::enable_if<std::is_floating_point<Vb>::value,FastOutputStream&>::type operator<<(Vb Wb){Wb+=Fb;if(Wb<0){put('-');Wb=-Wb;}__uint128_t Xb=static_cast<__uint128_t>(Wb);*this<<Xb;Vb Yb=Wb-static_cast<Vb>(Xb);if(Gb>0){put('.');for(unsigned Zb=Gb;Zb;--Zb){Yb*=10;int _b=static_cast<int>(Yb);put(_b^'0');Yb-=_b;}}return*this;}};

const int INF = 2000000000;
const int LOG = 20;
const int MAXN = 1 << 20;
int n, m;
vector<int> px;
int X_cur;

vector<int> mh, mt;
vector<char> lz;

int get_node_id(int node, int b) {
    return node * LOG + b;
}

void build(int node, int start, int end) {
    lz[node] = 0;
    if (start == end) {
        if (start > n) {
            for (int b = 0; b < LOG; b++) {
                int id = get_node_id(node, b);
                mh[id] = mt[id] = INF;
            }
            return;
        }
        for (int b = 0; b < LOG; b++) {
            int id = get_node_id(node, b);
            if ((start & (1 << b)) != 0) {
                mh[id] = start;
                mt[id] = INF;
            } else {
                mh[id] = mt[id] = INF;
            }
        }
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    for (int b = 0; b < LOG; b++) {
        int id = get_node_id(node, b);
        int idl = get_node_id(2 * node, b);
        int idr = get_node_id(2 * node + 1, b);
        mh[id] = min(mh[idl], mh[idr]);
        mt[id] = min(mt[idl], mt[idr]);
    }
}

void push(int node, int start, int end) {
    if (lz[node]) {
        for (int b = 0; b < LOG; b++) {
            int id = get_node_id(node, b);
            swap(mh[id], mt[id]);
        }
        if (start != end) {
            lz[2 * node] ^= 1;
            lz[2 * node + 1] ^= 1;
        }
        lz[node] = 0;
    }
}

void merge_node(int node) {
    for (int b = 0; b < LOG; b++) {
        int id = get_node_id(node, b);
        int idl = get_node_id(2 * node, b);
        int idr = get_node_id(2 * node + 1, b);
        mh[id] = min(mh[idl], mh[idr]);
        mt[id] = min(mt[idl], mt[idr]);
    }
}

void update(int node, int start, int end, int l, int r) {
    push(node, start, end);
    if (end < l || start > r) return;
    if (l <= start && end <= r) {
        for (int b = 0; b < LOG; b++) {
            int id = get_node_id(node, b);
            swap(mh[id], mt[id]);
        }
        if (start != end) {
            lz[2 * node] ^= 1;
            lz[2 * node + 1] ^= 1;
        }
        return;
    }
    int mid = (start + end) / 2;
    update(2 * node, start, mid, l, r);
    update(2 * node + 1, mid + 1, end, l, r);
    merge_node(node);
}

int main() {
    FastInputStream fin(fopen("coins.in", "r"));
    FastOutputStream fout(fopen("coins.out", "w"));
    fin >> n >> m;
    px.resize(n + 1, 0);
    for (int i = 1; i <= n; i++) { px[i] = px[i - 1] ^ i; }
    X_cur = px[n];

    int TSZ = 2 * MAXN;
    mh.assign(TSZ * LOG, INF);
    lz.assign(TSZ, 0);
    mt.assign(TSZ * LOG, INF);
    build(1, 1, MAXN);
    for (int qi = 0; qi < m; qi++) {
        int l, r;
        fin >> l >> r;
        // if (l != r) return 23;
        X_cur ^= (px[r] ^ px[l - 1]);
        update(1, 1, MAXN, l, r);
        if (X_cur == 0) {
            fout << "-1\n";
        } else {
            int b = 31 - __builtin_clz(X_cur);
            push(1, 1, MAXN); // ensure
            int ans = mh[get_node_id(1, b)];
            fout << ans << '\n';
        }
    }
    return 0;
}