#include<bits/stdc++.h>
using namespace std;
// #define DEBUG 1
struct IO {
#define MAXSIZE (1 << 20)
#define isdigit(x) (x >= '0' && x <= '9')
    char buf[MAXSIZE], *p1, *p2;
    char pbuf[MAXSIZE], *pp;
#if DEBUG
#else
    IO() : p1(buf), p2(buf), pp(pbuf) {}

    ~IO() { fwrite(pbuf, 1, pp - pbuf, stdout); }
#endif
char gc() {
#if DEBUG  // 调试，可显示字符
    return getchar();
#endif
    if (p1 == p2) p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin);
    return p1 == p2 ? ' ' : *p1++;
}

bool blank(char ch) {
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}

template <class T>
void read(T &x) {
    double tmp = 1;
    bool sign = 0;
    x = 0;
    char ch = gc();
    for (; !isdigit(ch); ch = gc())
        if (ch == '-') sign = 1;
    for (; isdigit(ch); ch = gc()) x = x * 10 + (ch - '0');
    if (ch == '.')
        for (ch = gc(); isdigit(ch); ch = gc())
            tmp /= 10.0, x += tmp * (ch - '0');
    if (sign) x = -x;
}

void read(char *s) {
    char ch = gc();
    for (; blank(ch); ch = gc());
    for (; !blank(ch); ch = gc()) *s++ = ch;
    *s = 0;
}

void read(char &c) { for (c = gc(); blank(c); c = gc()); }

void push(const char &c) {
#if DEBUG  // 调试，可显示字符
    putchar(c);
#else
    if (pp - pbuf == MAXSIZE) fwrite(pbuf, 1, MAXSIZE, stdout), pp = pbuf;
    *pp++ = c;
#endif
}

template <class T>
void write(T x) {
    if (x < 0) x = -x, push('-');  // 负数输出
    static T sta[35];
    T top = 0;
    do {
      sta[top++] = x % 10, x /= 10;
    } while (x);
    while (top) push(sta[--top] + '0');
}

template <class T>
void write(T x, char lastChar) {
    write(x), push(lastChar);
}
} io;
template<int Mod=1000000007>
struct Mint{
    int x;
    Mint(): x(0){}
    Mint(const int& m): x(m<0?m+Mod:m){
    } 
    Mint(const Mint& m): x(m.x){}
    inline Mint Pow(int b) const{
        Mint res=1,a=*this;
        while(b){
            if(b&1) res*=a;
            a*=a;
            b>>=1;
        }
        return res;
    } 
    inline Mint Inv() const{return Pow(Mod-2);}
    inline Mint operator-() const{
        return Mint(x?Mod-x:0);
    }
    inline Mint& operator+=(const Mint& m){
        x+=m.x;
        x>=Mod?x-=Mod:0;
        return *this;
    }
    inline Mint& operator*=(const Mint& m){
        x=1ll*x*m.x%Mod;  
        return *this;
    }
    inline Mint& operator/=(const Mint& m){
        assert(m.x);
        return (*this)*=m.Inv();
    }
    inline Mint& operator-=(const Mint& m){
        x-=m.x;
        x<0?x+=Mod:0;
        return *this;
    }
    inline Mint operator+(const Mint& m) const{
        return Mint(*this)+=m;
    }
    inline Mint operator*(const Mint& m) const{
        return Mint(*this)*=m;
    }
    inline Mint operator/(const Mint& m) const{
        return Mint(*this)/=m;
    }
    inline Mint operator-(const Mint& m) const{
        return Mint(*this)-=m;
    }
};
typedef Mint<> Mi;


int n,a[400000];
void init();
void modify_add(int l,int r,int v);
int query_ans(int l,int r);

signed main(){
    int q,opt,x,y,z,lans = 0;
    io.read(n),io.read(q);
    for(int i = 1;i <= n;i ++)io.read(a[i]);
    int _lst = 0;
    auto get = [&_lst]() {
        return _lst = ((_lst + 212001290) ^ 323920323) % 200 + 1;
    };
    init();
    while(q --){
        io.read(opt),io.read(x),io.read(y);
        opt ^= lans,x ^= lans,y ^= lans;
        if(opt == 1){
            z = get();
            modify_add(x,y,z);
        }else{
            io.write(lans = query_ans(x,y),'\n');
        }
    }
}