// Independent structural stress for the reference-counted rope itself.
// g++ -std=c++17 -O2 -DAUDIT tools/wblt_stress.cpp -o wblt_stress
#define main reference_solution_main
#include "../solutions/std.cpp"
#undef main
#include <random>

int main(int argc,char**argv) {
    unsigned seed=argc>1?unsigned(std::strtoul(argv[1],nullptr,10)):20261021;
    int rounds=argc>2?std::atoi(argv[2]):100,steps=argc>3?std::atoi(argv[3]):1000;
    std::mt19937 rng(seed);u64 operations=0;
    for(int round=0;round<rounds;++round) {
        u32 n=1+rng()%257;std::vector<u32>a(n+1);for(u32 i=1;i<=n;++i)a[i]=rng()%P;
        Rope tree(n);u32 root=tree.build(a,1,n);
        for(int step=0;step<steps;++step) {
            u32 l=1+rng()%n,r=1+rng()%n;if(l>r)std::swap(l,r);
            if(rng()%3==0) {
                u32 k=1+rng()%(r-l+1),start=1+rng()%(n-k+1);
                std::vector<u32>b(a.begin()+start,a.begin()+start+k);
                u32 block=tree.slice(root,start,start+k-1),rep=tree.repeat(block,r-l+1);
                u32 next=tree.replace(root,l,r,rep);
                tree.release(root);tree.release(block);tree.release(rep);root=next;
                for(u32 i=l;i<=r;++i)a[i]=b[(i-l)%k];
            } else {
                u32 factor=rng()%P;if(step%17==0)factor=0;
                std::vector<Rope::Change> changes;
                for(int j=0;j<4;++j)changes.push_back({u32(1+rng()%n),u32(rng()%P)});
                u32 next=tree.modify(root,l,r,factor,changes);tree.release(root);root=next;
                for(u32 i=l;i<=r;++i)a[i]=mul(a[i],factor);
                for(auto c:changes)a[c.pos]=add(a[c.pos],c.val);
            }
#ifdef AUDIT
            tree.audit(root);
#endif
            Agg expected;for(u32 i=l;i<=r;++i)expected=cat(expected,{1,a[i],0,0});
            Agg got=tree.query(root,l,r);
            assert(got.n==expected.n&&got.s0==expected.s0&&got.s1==expected.s1&&got.s2==expected.s2);
            if(step%31==0)for(u32 i=1;i<=n;++i)assert(tree.query(root,i,i).s0==a[i]);
            ++operations;
        }
        tree.release(root);assert(tree.live==0);
    }
    printf("{\"seed\":%u,\"rounds\":%d,\"steps\":%d,\"operations\":%llu,\"result\":\"PASS\"}\n",seed,rounds,steps,(unsigned long long)operations);
}
