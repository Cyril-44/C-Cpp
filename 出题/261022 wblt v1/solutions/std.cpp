// Window Calibration -- C++17 reference solution.
// Leaf-oriented weight-balanced rope, immutable nodes, reference counting.
// Compile: g++ -std=c++17 -O2 -DNDEBUG std.cpp -o std
// -DAUDIT enables exhaustive DAG/reference/balance/moment checks after each op.
// -DPROFILE prints allocation statistics to stderr. -DNO_GC is a test mutant.
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <vector>
using u32 = uint32_t;
using u64 = uint64_t;
constexpr u32 P = 998244353;
inline u32 add(u32 a,u32 b) { u32 c=a+b; return c>=P?c-P:c; }
inline u32 sub(u32 a,u32 b) { return a>=b?a-b:a+P-b; }
inline u32 mul(u32 a,u32 b) { return u64(a)*b%P; }
inline u32 c2(u32 n) { return u64(n)*(n-1)/2%P; }

struct FastIO {
    static constexpr int B=1<<20;
    char in[B],out[B]; int ip=0,il=0,op=0;
    int get() { if(ip==il) { il=int(fread(in,1,B,stdin));ip=0; if(!il)return EOF; } return in[ip++]; }
    u32 read() { int c; do {c=get();}while(c<=32&&c!=EOF); if(c==EOF)std::exit(2); u32 x=0;do{x=x*10+c-'0';c=get();}while(c>32&&c!=EOF);return x; }
    void write(u32 x) { char s[16];int k=0; do{s[k++]=char('0'+x%10);x/=10;}while(x); if(op+20>B)flush(); while(k)out[op++]=s[--k];out[op++]='\n'; }
    void flush(){fwrite(out,1,op,stdout);op=0;}
    ~FastIO(){flush();}
};
struct Agg { u32 n=0,s0=0,s1=0,s2=0; };
Agg cat(Agg a,Agg b) {
    return {a.n+b.n,add(a.s0,b.s0),add(add(a.s1,b.s1),mul(a.n,b.s0)),
        add(add(a.s2,b.s2),add(mul(a.n,b.s1),mul(c2(a.n),b.s0)))};
}
Agg scaled(Agg a,u32 k) { a.s0=mul(a.s0,k);a.s1=mul(a.s1,k);a.s2=mul(a.s2,k);return a; }

struct Rope {
    struct Node { u32 l,r,n,ref,s0,s1,s2,tag; };
    struct Change { u32 pos,val; };
    std::vector<Node> t;
    u32 free_head=0,live=0,peak=0;
    u64 allocations=0,recycled=0;
    explicit Rope(u32 n) { t.reserve(size_t(n)*2+8192);t.push_back({}); }
    u32 size(u32 x)const{return x?t[x].n:0;}
    Agg agg(u32 x)const {return x?Agg{t[x].n,t[x].s0,t[x].s1,t[x].s2}:Agg{};}
    u32 keep(u32 x){if(x)++t[x].ref;return x;}
    u32 alloc(Node a) {
        u32 x;
        if(free_head){x=free_head;free_head=t[x].l;t[x]=a;++recycled;}
        else{x=u32(t.size());t.push_back(a);}
        ++allocations;peak=std::max(peak,++live);return x;
    }
    void release(u32 x) {
        if(!x||--t[x].ref)return;
#ifdef NO_GC
        return;
#else
        u32 l=t[x].l,r=t[x].r;
        release(l);release(r);
        t[x].l=free_head;t[x].r=0;t[x].n=0;free_head=x;--live;
#endif
    }
    u32 leaf(u32 v){return alloc({0,0,1,1,v,0,0,1});}
    u32 node(u32 a,u32 b) {
        assert(a&&b);
        Agg s=cat(agg(a),agg(b));keep(a);keep(b);
        return alloc({a,b,s.n,1,s.s0,s.s1,s.s2,1});
    }
    u32 scale(u32 x,u32 k) {
        if(!x||k==1)return keep(x);
        Node a=t[x];a.ref=1;a.s0=mul(a.s0,k);a.s1=mul(a.s1,k);a.s2=mul(a.s2,k);a.tag=mul(a.tag,k);
        keep(a.l);keep(a.r);return alloc(a);
    }
    // Inputs are borrowed; returned roots each own one reference.
    std::pair<u32,u32> children(u32 x) {
        Node a=t[x];assert(a.n>1);
        u32 l=scale(a.l,a.tag),r=scale(a.r,a.tag);return {l,r};
    }
    u32 balance(u32 a,u32 b) {
        if(size(a)>3*size(b)) {
            auto [l,r]=children(a);u32 ans;
            if(size(r)<2*size(l)) {
                u32 z=node(r,b);ans=node(l,z);release(z);
            } else {
                auto [rl,rr]=children(r);
                u32 x=node(l,rl),y=node(rr,b);ans=node(x,y);
                release(x);release(y);release(rl);release(rr);
            }
            release(l);release(r);return ans;
        }
        if(size(b)>3*size(a)) {
            auto [l,r]=children(b);u32 ans;
            if(size(l)<2*size(r)) {
                u32 z=node(a,l);ans=node(z,r);release(z);
            } else {
                auto [ll,lr]=children(l);
                u32 x=node(a,ll),y=node(lr,r);ans=node(x,y);
                release(x);release(y);release(ll);release(lr);
            }
            release(l);release(r);return ans;
        }
        return node(a,b);
    }
    u32 join(u32 a,u32 b) {
        if(!a)return keep(b);
        if(!b)return keep(a);
        if(size(a)>3*size(b)) {
            auto [l,r]=children(a);u32 z=join(r,b),ans=balance(l,z);
            release(l);release(r);release(z);return ans;
        }
        if(size(b)>3*size(a)) {
            auto [l,r]=children(b);u32 z=join(a,l),ans=balance(z,r);
            release(l);release(r);release(z);return ans;
        }
        return node(a,b);
    }
    std::pair<u32,u32> split(u32 x,u32 k) {
        if(!k)return {0,keep(x)};
        if(k==size(x))return {keep(x),0};
        auto [l,r]=children(x);u32 h=size(l),a,b;
        if(k<h) {
            auto [p,q]=split(l,k);a=p;b=join(q,r);release(q);
        } else if(k==h) { return {l,r}; }
        else {
            auto [p,q]=split(r,k-h);a=join(l,p);b=q;release(p);
        }
        release(l);release(r);return {a,b};
    }
    u32 slice(u32 x,u32 l,u32 r) {
        if(l==1&&r==size(x))return keep(x);
        auto [a,b]=children(x);u32 h=size(a),ans;
        if(r<=h)ans=slice(a,l,r);
        else if(l>h)ans=slice(b,l-h,r-h);
        else {
            u32 p=slice(a,l,h),q=slice(b,1,r-h);ans=join(p,q);release(p);release(q);
        }
        release(a);release(b);return ans;
    }
    u32 replace(u32 x,u32 l,u32 r,u32 y) {
        auto [ab,c]=split(x,r);auto [a,b]=split(ab,l-1);
        u32 ay=join(a,y),ans=join(ay,c);
        release(ab);release(a);release(b);release(c);release(ay);return ans;
    }
    u32 repeat(u32 block,u32 len) {
        u32 x=keep(block);
        while(size(x)<len){u32 y=node(x,x);release(x);x=y;}
        if(size(x)==len)return x;
        auto [a,b]=split(x,len);release(x);release(b);return a;
    }
    Agg query(u32 x,u32 l,u32 r)const {
        if(l>r)return {};
        if(l==1&&r==size(x))return agg(x);
        Node a=t[x];u32 h=size(a.l);Agg z;
        if(r<=h)z=query(a.l,l,r);
        else if(l>h)z=query(a.r,l-h,r-h);
        else z=cat(query(a.l,l,h),query(a.r,1,r-h));
        return a.tag==1?z:scaled(z,a.tag);
    }
    // Apply one range multiplication plus <=4 sorted point additions in one descent.
    u32 edit(u32 x,u32 base,u32 ql,u32 qr,u32 factor,const Change* c,int lo,int hi) {
        u32 end=base+size(x)-1;
        if(lo==hi) {
            if(qr<base||end<ql||factor==1)return keep(x);
            if(ql<=base&&end<=qr)return scale(x,factor);
        }
        if(size(x)==1) {
            u32 v=t[x].s0;if(ql<=base&&base<=qr)v=mul(v,factor);
            for(int i=lo;i<hi;++i)v=add(v,c[i].val);
            return leaf(v);
        }
        auto [a,b]=children(x);u32 cut=base+size(a);int mid=lo;
        while(mid<hi&&c[mid].pos<cut)++mid;
        u32 p=edit(a,base,ql,qr,factor,c,lo,mid),q=edit(b,cut,ql,qr,factor,c,mid,hi);
        u32 ans=node(p,q);release(a);release(b);release(p);release(q);return ans;
    }
    u32 modify(u32 root,u32 l,u32 r,u32 factor,std::vector<Change> c) {
        u32 n=size(root);
        c.erase(std::remove_if(c.begin(),c.end(),[&](Change x){return x.pos>n||!x.val;}),c.end());
        std::sort(c.begin(),c.end(),[](Change a,Change b){return a.pos<b.pos;});
        return edit(root,1,l,r,factor,c.data(),0,int(c.size()));
    }
    u32 build(const std::vector<u32>& a,u32 l,u32 r) {
        if(l==r)return leaf(a[l]);
        u32 m=(l+r)/2;
        u32 x=build(a,l,m),y=build(a,m+1,r),z=node(x,y);release(x);release(y);return z;
    }
    u32 value(u32 root,int pos)const {
        if(pos<=0)return 0;
        Agg a=query(root,1,u32(pos));return sub(mul(pos,a.s0),a.s1);
    }
    u32 prefix(u32 root,u32 len)const {
        if(!len)return 0;
        Agg a=query(root,1,len);
        return add(sub(mul(c2(len+1),a.s0),mul(len,a.s1)),a.s2);
    }
#ifdef AUDIT
    Agg visit(u32 x,std::vector<u32>& expected,std::vector<unsigned char>& seen)const {
        if(seen[x])return agg(x);
        seen[x]=1;Node a=t[x];assert(a.n&&a.ref);
        if(a.n==1){assert(!a.l&&!a.r&&!a.s1&&!a.s2);return agg(x);}
        assert(a.l&&a.r);++expected[a.l];++expected[a.r];
        Agg l=visit(a.l,expected,seen),r=visit(a.r,expected,seen);
        assert(l.n<=3*r.n&&r.n<=3*l.n);
        Agg z=scaled(cat(l,r),a.tag);
        assert(z.n==a.n&&z.s0==a.s0&&z.s1==a.s1&&z.s2==a.s2);return z;
    }
    void audit(u32 root)const {
        std::vector<u32> ex(t.size());std::vector<unsigned char> seen(t.size());ex[root]=1;visit(root,ex,seen);
        u32 count=0;for(u32 x=1;x<t.size();++x){if(seen[x]){assert(t[x].ref==ex[x]);++count;}else assert(!t[x].n&&!t[x].ref);}
        assert(count==live);assert(live<=2*size(root)-1);
        u32 freecount=0;for(u32 x=free_head;x;x=t[x].l){assert(!seen[x]&&!t[x].ref&&!t[x].n);seen[x]=2;++freecount;}
        assert(size_t(count)+freecount+1==t.size());
    }
#endif
};

int main() {
    static FastIO io;u32 n=io.read();std::vector<u32> e(n+1);u32 prev=0,prev2=0;
    for(u32 i=1;i<=n;++i){u32 a=io.read();e[i]=add(sub(a,add(prev,prev)),prev2);prev2=prev;prev=a;}
    Rope tree(n);u32 root=tree.build(e,1,n);std::vector<u32>().swap(e);
    u32 m=io.read();
    for(u32 step=0;step<m;++step) {
        u32 op=io.read(),l=io.read(),r=io.read();
        if(op==1) {
            u32 k=io.read(),len=r-l-1;
            if(k!=len) {
                u32 block=tree.slice(root,l+2,l+k+1),rep=tree.repeat(block,len);
                Agg old=tree.query(root,l+2,r),now=tree.agg(rep);
                u32 ds=sub(now.s0,old.s0),dt=sub(now.s1,old.s1),da=sub(mul(len,ds),dt);
                u32 replaced=tree.replace(root,l+2,r,rep);
                std::vector<Rope::Change> c={{r+1,sub(0,add(da,ds))},{r+2,da}};
                u32 next=tree.modify(replaced,1,0,1,c);
                tree.release(root);tree.release(block);tree.release(rep);tree.release(replaced);root=next;
            }
        } else if(op==2) {
            u32 u=io.read(),v=io.read();
            u32 am=tree.value(root,int(l)-1),amm=tree.value(root,int(l)-2);
            u32 ar=tree.value(root,r),arm=tree.value(root,int(r)-1),w=sub(u,1);
            std::vector<Rope::Change> c={
                {l,add(mul(w,sub(add(am,am),amm)),v)},
                {l+1,sub(0,add(mul(w,am),v))},
                {r+1,sub(0,add(mul(w,sub(add(ar,ar),arm)),v))},
                {r+2,add(mul(w,ar),v)}};
            u32 next=tree.modify(root,l,r,u,c);tree.release(root);root=next;
        } else {
            io.write(sub(tree.prefix(root,r),tree.prefix(root,l-1)));
        }
#ifdef AUDIT
        tree.audit(root);
#endif
    }
    io.flush();
#ifdef PROFILE
    fprintf(stderr,"{\"node_bytes\":%zu,\"slots\":%zu,\"peak_live\":%u,\"live\":%u,\"allocations\":%llu,\"recycled\":%llu,\"pool_capacity_bytes\":%zu}\n",
        sizeof(Rope::Node),tree.t.size()-1,tree.peak,tree.live,(unsigned long long)tree.allocations,(unsigned long long)tree.recycled,tree.t.capacity()*sizeof(Rope::Node));
#endif
    tree.release(root);return 0;
}
