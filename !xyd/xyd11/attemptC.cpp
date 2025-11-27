#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
using i128 = __int128_t;

// ---------------- Fast IO (adapted) ----------------
namespace FastIO {
    const int SZ = 1<<20;
    static char ibuf[SZ], obuf[SZ];
    static int ipos = SZ, ilen = SZ;
    static int opos = 0;
    inline char nextChar() {
        if (ipos == ilen) {
            ilen = (int)fread(ibuf, 1, SZ, stdin);
            ipos = 0;
            if (ilen == 0) return EOF;
        }
        return ibuf[ipos++];
    }
    template<typename T>
    bool readInt(T &out) {
        char c; T sign = 1; T num = 0;
        c = nextChar();
        if (c==EOF) return false;
        while (c!='-' && (c<'0' || c>'9')) c = nextChar();
        if (c=='-') { sign = -1; c = nextChar(); }
        while (c>='0' && c<='9') {
            num = num*10 + (c - '0');
            c = nextChar();
        }
        out = num * sign;
        return true;
    }
    void writeChar(char c) {
        if (opos == SZ) { fwrite(obuf,1,SZ,stdout); opos = 0; }
        obuf[opos++] = c;
    }
    void writeStr(const char*s) {
        while (*s) writeChar(*s++);
    }
    void writeI128(i128 x) {
        if (x==0) { writeChar('0'); return; }
        if (x<0) { writeChar('-'); x = -x; }
        string s;
        while (x>0) {
            int d = (int)(x % 10);
            s.push_back('0' + d);
            x /= 10;
        }
        for (int i = (int)s.size()-1; i>=0; --i) writeChar(s[i]);
    }
    void flush() { if (opos) { fwrite(obuf,1,opos,stdout); opos = 0; } }
}
using FastIO::readInt;
using FastIO::writeI128;
using FastIO::writeChar;
using FastIO::flush;

// ---------------- Segment / NodeFuncSeg ----------------
struct Segment {
    int64 L, R;   // inclusive
    i128 A, B, C; // coefficients
    Segment() {}
    Segment(int64 _L, int64 _R, i128 _A, i128 _B, i128 _C)
        : L(_L), R(_R), A(_A), B(_B), C(_C) {}
};

struct NodeFuncSeg {
    vector<Segment> segs; // sorted by L, non-overlapping
    NodeFuncSeg() { segs.clear(); }
    void clear() { segs.clear(); }
    void add_segment(int64 L, int64 R, i128 A, i128 B, i128 C) {
        if (L > R) return;
        segs.emplace_back(L, R, A, B, C);
    }
    void finalize() {
        if (segs.empty()) return;
        sort(segs.begin(), segs.end(), [](const Segment &s1, const Segment &s2){
            if (s1.L != s2.L) return s1.L < s2.L;
            return s1.R < s2.R;
        });
        vector<Segment> out;
        out.reserve(segs.size());
        out.push_back(segs[0]);
        for (size_t i = 1; i < segs.size(); ++i) {
            Segment &cur = segs[i];
            Segment &last = out.back();
            if (cur.L <= last.R + 1 && cur.A == last.A && cur.B == last.B && cur.C == last.C) {
                if (cur.R > last.R) last.R = cur.R;
            } else {
                out.push_back(cur);
            }
        }
        segs.swap(out);
    }
    size_t size() const { return segs.size(); }
    // evaluate at integer l; return very large sentinel if not found
    i128 evaluate(int64 l) const {
        if (segs.empty()) return ( (i128)1 << 120 );
        int lo = 0, hi = (int)segs.size()-1;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            const Segment &s = segs[mid];
            if (l < s.L) hi = mid - 1;
            else if (l > s.R) lo = mid + 1;
            else {
                i128 li = (i128)l;
                return s.A*li*li + s.B*li + s.C;
            }
        }
        return ( (i128)1 << 120 );
    }
};

// ---------------- integer sqrt and root helpers ----------------
i128 i128_isqrt(i128 x) {
    if (x <= 0) return 0;
    i128 lo = 0, hi = (i128)1;
    while (hi*hi <= x) hi <<= 1;
    while (lo < hi) {
        i128 mid = (lo + hi + 1) >> 1;
        if (mid*mid <= x) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}
vector<int64> real_roots_floor(i128 A, i128 B, i128 C) {
    vector<int64> res;
    if (A == 0) {
        if (B == 0) return res;
        // root x = -C / B
        i128 num = -C, den = B;
        bool neg = false;
        if (den < 0) { den = -den; num = -num; }
        i128 q = num / den;
        if (num < 0 && (num % den != 0)) q -= 1;
        res.push_back((int64)q);
        return res;
    }
    i128 D = B*B - (i128)4*A*C;
    if (D < 0) return res;
    i128 sd = i128_isqrt(D);
    auto floor_div = [&](i128 num, i128 den)->int64{
        if (den < 0) { den = -den; num = -num; }
        i128 q = num / den;
        if (num < 0 && (num % den != 0)) q -= 1;
        return (int64)q;
    };
    i128 twoA = (i128)2 * A;
    i128 n1 = -B - sd;
    i128 n2 = -B + sd;
    int64 r1 = floor_div(n1, twoA);
    int64 r2 = floor_div(n2, twoA);
    if (r1 > r2) swap(r1, r2);
    res.push_back(r1); res.push_back(r2);
    return res;
}
inline i128 eval_poly_at(i128 A, i128 B, i128 C, int64 l) {
    i128 li = (i128)l;
    return A*li*li + B*li + C;
}

// ---------------- merge with pointwise min (as implemented earlier) ----------------
NodeFuncSeg merge_children_to_parent_pointwise_min(const NodeFuncSeg &Lfunc,
                                                   const NodeFuncSeg &Rfunc,
                                                   int64 a_u, int Lcnt, int Rcnt) {
    NodeFuncSeg result;
    if (Lfunc.size() == 0 || Rfunc.size() == 0) return result;
    vector<int64> events;
    events.reserve(Lfunc.size()*2 + Rfunc.size()*2 + 4);
    for (const Segment &s : Lfunc.segs) {
        events.push_back(s.L);
        events.push_back(s.R + 1);
    }
    for (const Segment &s : Rfunc.segs) {
        events.push_back(s.L - Lcnt);
        events.push_back(s.R - Lcnt + 1);
    }
    sort(events.begin(), events.end());
    events.erase(unique(events.begin(), events.end()), events.end());
    if (events.empty()) return result;

    auto find_Lseg = [&](int64 l)->const Segment&{
        int lo = 0, hi = (int)Lfunc.segs.size()-1;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            const Segment &s = Lfunc.segs[mid];
            if (l < s.L) hi = mid - 1;
            else if (l > s.R) lo = mid + 1;
            else return s;
        }
        return Lfunc.segs.back();
    };
    auto find_Rseg_shifted = [&](int64 l)->const Segment&{
        int64 rr = l + Lcnt;
        int lo = 0, hi = (int)Rfunc.segs.size()-1;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            const Segment &s = Rfunc.segs[mid];
            if (rr < s.L) hi = mid - 1;
            else if (rr > s.R) lo = mid + 1;
            else return s;
        }
        return Rfunc.segs.back();
    };

    auto sqcost = [&](int64 z)->i128 { i128 d = (i128)a_u - (i128)z; return d*d; };

    for (size_t ei = 0; ei + 1 < events.size(); ++ei) {
        int64 segL = events[ei];
        int64 segR = events[ei+1] - 1;
        if (segL > segR) continue;
        int64 sample = segL;
        const Segment &sL = find_Lseg(sample);
        const Segment &sR = find_Rseg_shifted(sample);
        // convert right child to l
        i128 Ar = sR.A, Br = sR.B, Cr = sR.C;
        i128 Ar_l = Ar;
        i128 Br_l = (i128)2 * Ar * (i128)Lcnt + Br;
        i128 Cr_l = Ar * (i128)Lcnt * (i128)Lcnt + Br * (i128)Lcnt + Cr;
        i128 A_base = sL.A + Ar_l;
        i128 B_base = sL.B + Br_l;
        i128 C_base = sL.C + Cr_l;
        int64 z0 = sample + (int64)Lcnt - 1;
        int64 z1 = sample + (int64)Lcnt;
        i128 cost0 = sqcost(z0);
        i128 cost1 = sqcost(z1);
        i128 add_cost_mode0 = cost0 < cost1 ? cost0 : cost1;
        i128 A0 = A_base, B0 = B_base, C0 = C_base + add_cost_mode0;

        // mode1
        int64 sample_r_for_left = sample + (int64)Rcnt;
        const Segment *sL2ptr = nullptr;
        {
            int lo = 0, hi = (int)Rfunc.segs.size()-1;
            while (lo <= hi) {
                int mid = (lo + hi) >> 1;
                const Segment &s = Rfunc.segs[mid];
                if (sample_r_for_left < s.L) hi = mid - 1;
                else if (sample_r_for_left > s.R) lo = mid + 1;
                else { sL2ptr = &s; break; }
            }
            if (!sL2ptr) sL2ptr = &Rfunc.segs.back();
        }
        const Segment &sL2 = *sL2ptr;
        const Segment *sR2ptr = nullptr;
        {
            int64 x = sample + (int64)Rcnt;
            int lo = 0, hi = (int)Lfunc.segs.size()-1;
            while (lo <= hi) {
                int mid = (lo + hi) >> 1;
                const Segment &s = Lfunc.segs[mid];
                if (x < s.L) hi = mid - 1;
                else if (x > s.R) lo = mid + 1;
                else { sR2ptr = &s; break; }
            }
            if (!sR2ptr) sR2ptr = &Lfunc.segs.back();
        }
        const Segment &sR2 = *sR2ptr;
        i128 AL = sL2.A, BL = sL2.B, CL = sL2.C;
        i128 AL_l = AL;
        i128 BL_l = (i128)2 * AL * (i128)Rcnt + BL;
        i128 CL_l = AL * (i128)Rcnt * (i128)Rcnt + BL * (i128)Rcnt + CL;
        i128 AR2 = sR2.A, BR2 = sR2.B, CR2 = sR2.C;
        i128 AR_l2 = AR2;
        i128 BR_l2 = (i128)2 * AR2 * (i128)Rcnt + BR2;
        i128 CR_l2 = AR2 * (i128)Rcnt * (i128)Rcnt + BR2 * (i128)Rcnt + CR2;
        i128 A1 = AL_l + AR_l2;
        i128 B1 = BL_l + BR_l2;
        i128 C1 = CL_l + CR_l2;
        int64 z0_m1 = sample + (int64)Rcnt - 1;
        int64 z1_m1 = sample + (int64)Rcnt;
        i128 costm0 = sqcost(z0_m1);
        i128 costm1 = sqcost(z1_m1);
        i128 add_cost_mode1 = costm0 < costm1 ? costm0 : costm1;
        i128 C1_tot = C1 + add_cost_mode1;

        i128 Ah = A0 - A1;
        i128 Bh = B0 - B1;
        i128 Ch = C0 - C1_tot;

        if (Ah == 0 && Bh == 0) {
            if (Ch <= 0) result.add_segment(segL, segR, A0, B0, C0);
            else result.add_segment(segL, segR, A1, B1, C1_tot);
            continue;
        }
        vector<int64> roots = real_roots_floor(Ah, Bh, Ch);
        if (roots.empty()) {
            i128 hs = eval_poly_at(Ah, Bh, Ch, sample);
            if (hs <= 0) result.add_segment(segL, segR, A0, B0, C0);
            else result.add_segment(segL, segR, A1, B1, C1_tot);
            continue;
        }
        if (roots.size() == 1) {
            int64 r = roots[0];
            int64 m1 = min(segR, r);
            int64 m2 = max(segL, r+1);
            if (segL <= m1) {
                i128 hs = eval_poly_at(Ah, Bh, Ch, segL);
                if (hs <= 0) result.add_segment(segL, m1, A0, B0, C0);
                else result.add_segment(segL, m1, A1, B1, C1_tot);
            }
            if (m2 <= segR) {
                i128 hs = eval_poly_at(Ah, Bh, Ch, m2);
                if (hs <= 0) result.add_segment(m2, segR, A0, B0, C0);
                else result.add_segment(m2, segR, A1, B1, C1_tot);
            }
            continue;
        }
        int64 r1 = roots[0], r2 = roots[1];
        int64 a1 = segL, b1 = min(segR, r1);
        int64 a2 = max(segL, r1+1), b2 = min(segR, r2);
        int64 a3 = max(segL, r2+1), b3 = segR;
        if (a1 <= b1) {
            i128 hs = eval_poly_at(Ah, Bh, Ch, a1);
            if (hs <= 0) result.add_segment(a1, b1, A0, B0, C0);
            else result.add_segment(a1, b1, A1, B1, C1_tot);
        }
        if (a2 <= b2) {
            i128 hs = eval_poly_at(Ah, Bh, Ch, a2);
            if (hs <= 0) result.add_segment(a2, b2, A0, B0, C0);
            else result.add_segment(a2, b2, A1, B1, C1_tot);
        }
        if (a3 <= b3) {
            i128 hs = eval_poly_at(Ah, Bh, Ch, a3);
            if (hs <= 0) result.add_segment(a3, b3, A0, B0, C0);
            else result.add_segment(a3, b3, A1, B1, C1_tot);
        }
    }

    result.finalize();
    return result;
}

// ---------------- Tree and main driver ----------------
int main() {
    freopen("tree.in", "r", stdin);
    freopen("tree.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,q;
    if (!readInt(n)) return 0;
    readInt(q);
    int total = 2*n - 1;
    vector<int> ls(total+1), rs(total+1);
    for (int i = 1; i <= total; ++i) {
        int L,R; readInt(L); readInt(R);
        ls[i] = L; rs[i] = R;
    }
    // build parent, children, leafcnt
    vector<int> parent(total+1, 0), isLeaf(total+1,0), leafcnt(total+1,0);
    for (int i = 1; i <= total; ++i) {
        if (ls[i]) parent[ls[i]] = i;
        if (rs[i]) parent[rs[i]] = i;
    }
    int root = 1;
    for (int i = 1; i <= total; ++i) if (parent[i]==0) { root = i; break; }
    // compute leafcnt via postorder (iterative)
    vector<int> order; order.reserve(total);
    vector<int> stk; stk.push_back(root);
    while (!stk.empty()) {
        int u = stk.back(); stk.pop_back();
        order.push_back(u);
        if (ls[u]) stk.push_back(ls[u]);
        if (rs[u]) stk.push_back(rs[u]);
    }
    for (int i = (int)order.size()-1; i>=0; --i) {
        int u = order[i];
        if (ls[u]==0 && rs[u]==0) { isLeaf[u]=1; leafcnt[u]=1; }
        else {
            isLeaf[u]=0;
            int L = ls[u] ? leafcnt[ls[u]] : 0;
            int R = rs[u] ? leafcnt[rs[u]] : 0;
            leafcnt[u] = L + R;
        }
    }

    // a_u initial zeros for non-leaf
    vector<int64> a_val(total+1, 0);
    // NodeFuncSeg per node
    vector<NodeFuncSeg*> nf(total+1, nullptr);
    for (int i = 1; i <= total; ++i) nf[i] = new NodeFuncSeg();

    // build initial node functions bottom-up using small-to-large merging
    // For leaves: set a sentinel segment at l=0 with cost 0 (domain will be adjusted when used)
    for (int i = (int)order.size()-1; i>=0; --i) {
        int u = order[i];
        if (isLeaf[u]) {
            nf[u]->add_segment(0, 0, (i128)0, (i128)0, (i128)0);
            nf[u]->finalize();
        } else {
            NodeFuncSeg *Lnf = nf[ls[u]];
            NodeFuncSeg *Rnf = nf[rs[u]];
            // ensure smaller merges into larger: but merge function returns fresh NodeFuncSeg
            NodeFuncSeg merged = merge_children_to_parent_pointwise_min(*Lnf, *Rnf, a_val[u], leafcnt[ls[u]], leafcnt[rs[u]]);
            // replace nf[u] with merged
            delete nf[u];
            nf[u] = new NodeFuncSeg();
            nf[u]->segs.swap(merged.segs);
        }
    }

    // process q operations
    for (int qi = 0; qi < q; ++qi) {
        int ui; int64 yi; int vi; int64 l1,r1,l2,r2;
        readInt(ui); readInt(yi); readInt(vi);
        readInt(l1); readInt(r1); readInt(l2); readInt(r2);
        // update a_ui
        a_val[ui] = yi;
        // rebuild from ui up to root: rebuild each ancestor by recomputing from children
        int cur = ui;
        // collect the path to root (inclusive)
        vector<int> path;
        while (cur) { path.push_back(cur); cur = parent[cur]; }
        // rebuild bottom-up along path (from deepest to root)
        for (int i = (int)path.size()-1; i>=0; --i) {
            int u = path[i];
            if (isLeaf[u]) continue;
            // rebuild u
            NodeFuncSeg *Lnf = nf[ls[u]];
            NodeFuncSeg *Rnf = nf[rs[u]];
            NodeFuncSeg merged = merge_children_to_parent_pointwise_min(*Lnf, *Rnf, a_val[u], leafcnt[ls[u]], leafcnt[rs[u]]);
            // replace nf[u]
            delete nf[u];
            nf[u] = new NodeFuncSeg();
            nf[u]->segs.swap(merged.segs);
        }
        // evaluate f(vi, l1) and f(vi, l2) using nf[vi].evaluate
        i128 ans1 = nf[vi]->evaluate(l1);
        i128 ans2 = nf[vi]->evaluate(l2);
        i128 diff = ans1 - ans2;
        writeI128(diff);
        writeChar('\n');
    }

    // flush output
    flush();
    return 0;
}
