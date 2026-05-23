#include <cstdio>
#include <string>
#include <concepts>
#include <type_traits>
int a,b,c,d,e,f,g,h,i,j,k,l,m,n;
int o,p,q,r,s,t,u,v,w,x,y,z,A,B;
int C,D,E,F,G,H,I,J,K,L,M,N,O,P;
int Q,R,S,T,U,V,W,X,Y,Z,_;
// 定义一个概念来匹配所有整数类型，包括 __int128
template <typename bb>
concept IntegerWithI128 = std::integral<bb> || std::same_as<bb, __int128_t> || std::same_as<bb, __uint128_t>;
class FastInputStream {
    static constexpr size_t db = 1 << 20;
    char eb[db];
    char *fb = nullptr, *gb = nullptr;
    FILE *hb;
public:
    FastInputStream(FILE *ib = stdin) : hb(ib) {}
    [[gnu::always_inline]] char get() {
        if (fb == gb) {
            fb = eb;
            gb = eb + fread(eb, 1, db, hb);
            if (fb == gb) return EOF;
        }
        return *fb++;
    }
    FastInputStream& operator>>(bool& kb) {
        int lb = get();
        while (~lb && lb != '0' && lb != '1') lb = get();
        kb = (lb == '1');
        return *this;
    }
    FastInputStream& operator>>(char& mb) {
        mb = get();
        while (mb == ' ' || mb == '\r' || mb == '\n' || mb == '\t') mb = get();
        return *this;
    }
    template <IntegerWithI128 nb>
    FastInputStream& operator>>(nb& ob) {
        int pb = get();
        bool qb = false;
        while (~pb && (pb < '0' || pb > '9') && pb != '-') pb = get();
        if constexpr (std::is_signed_v<nb> || std::same_as<nb, __int128_t>) {
            if (pb == '-') { qb = true; pb = get(); }
        }
        ob = 0;
        while (pb >= '0' && pb <= '9') {
            ob = (ob << 3) + (ob << 1) + (pb ^ '0');
            pb = get();
        }
        if (qb) ob = -ob;
        return *this;
    }
    template <std::floating_point rb>
    FastInputStream& operator>>(rb& sb) {
        int tb = get();
        bool ub = false;
        while (~tb && (tb < '0' || tb > '9') && tb != '-') tb = get();
        if (tb == '-') { ub = true; tb = get(); }
        
        __uint128_t vb = 0;
        while (tb >= '0' && tb <= '9') {
            vb = (vb << 3) + (vb << 1) + (tb ^ '0');
            tb = get();
        }
        sb = static_cast<rb>(vb);
        if (tb == '.') {
            rb wb = 1.0;
            for (tb = get(); tb >= '0' && tb <= '9'; tb = get()) {
                sb += (tb ^ '0') * (wb /= 10.0);
            }
        }
        if (ub) sb = -sb;
        return *this;
    }
    FastInputStream& operator>>(char *xb) {
        int yb = get();
        while (~yb && (yb <= ' ')) yb = get();
        while (~yb && (yb > ' ')) *xb++ = yb, yb = get();
        *xb = '\0';
        return *this;
    }
} fin;

class FastOutputStream {
    static constexpr size_t Bb = 1 << 20;
    char Cb[Bb], *Db = Cb;
    FILE *Eb; long double Fb = 5e-6;
    unsigned char Gb = 6;
public:
    FastOutputStream(FILE *Hb = stdout) : Eb(Hb) { setvbuf(Eb, nullptr, _IONBF, 0); }
    ~FastOutputStream() { flush(); }
    void flush() {
        fwrite(Cb, 1, Db - Cb, Eb);
        Db = Cb;
    }
    [[gnu::always_inline]] void put(char Kb) {
        if (Db == Cb + Bb) flush();
        *Db++ = Kb;
    }
    FastOutputStream& precision(int Mb) { 
        Gb = Mb;
        Fb = 0.5;
        while (Mb--) Fb *= 0.1;
        return *this;
    }
    FastOutputStream& operator<<(char Nb) { put(Nb); return *this; }
    FastOutputStream& operator<<(const char *Ob) {
        while (*Ob) put(*Ob++);
        return *this;
    }
    FastOutputStream& operator<<(const std::string &Pb) {
        for (char c : Pb) put(c);
        return *this;
    }
    template <IntegerWithI128 Qb>
    FastOutputStream& operator<<(Qb Rb) {
        if (Rb == 0) { put('0'); return *this; }
        Qb Sb = Rb;
        if constexpr (std::is_signed_v<Qb> || std::same_as<Qb, __int128_t>) {
            if (Sb < 0) { put('-'); Sb = -Sb; }
        }
        static char Tb[64];
        int Ub = 0;
        while (Sb) {
            Tb[Ub++] = static_cast<char>(Sb % 10) ^ '0';
            Sb /= 10;
        }
        while (Ub) put(Tb[--Ub]);
        return *this;
    }
    template <std::floating_point Vb>
    FastOutputStream& operator<<(Vb Wb) {
        Wb += Fb;
        if (Wb < 0) { put('-'); Wb = -Wb; }
        __uint128_t Xb = static_cast<__uint128_t>(Wb);
        *this << Xb;
        Vb Yb = Wb - static_cast<Vb>(Xb);
        if (Gb > 0) {
            put('.');
            for (unsigned Zb = Gb; Zb; --Zb) {
                Yb *= 10;
                int _b = static_cast<int>(Yb);
                put(_b ^ '0');
                Yb -= _b;
            }
        }
        return *this;
    }
} fout;