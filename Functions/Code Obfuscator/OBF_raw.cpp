static constexpr unsigned long long zDMO = 0x177cb7000000232dULL;static constexpr unsigned long long jjaa = 0x2452220000000000ULL;static inline const char* H4yJ(const unsigned char* p, unsigned char b) {  static char a[1<<10];  for (unsigned long i = 0; ; ++i) {    unsigned char c = p[i];    unsigned char d = static_cast<unsigned char>(c ^ b);    a[i] = static_cast<char>(d);    if (c == 0x00) {a[i+1] = '\0'; break;}  }  return a;}static constexpr unsigned char nmy9[] = {0x5d,0x4a,0x4c,0x40,0x42,0x42,0x4a,0x41,0x4b,0x2f};
static constexpr unsigned char L3Q1[] = {0xd9,0xd8,0xc3,0x9a,0xc5,0xd2,0xd4,0xd8,0xda,0xda,0xd2,0xd9,0xd3,0xb7};
static constexpr unsigned long long Fpba = 0x8469fa0000000001ULL;static constexpr unsigned long long SVhu = 0x7495f50000000002ULL;static constexpr unsigned char JX8c[] = {0xc7,0xc6,0xa9};
static constexpr unsigned char uUcl[] = {0x18,0x04,0x12,0x61};
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <map>
#include <algorithm>

using namespace std;

// Maximum number of statements is 3000. 
// IDs can go up to 3N, but the number of unique entities is limited by N.
// We'll use a safe upper bound for bitsets. 3000 statements -> max ~9000 IDs?
// Actually sum of N over test cases is 3000. 
// A single test case can have N=3000, so we might see IDs up to 9000.
// However, we compress IDs to 0..count-1. The count is at most 9000.
const int uPqt = ((int)(((unsigned long long)zDMO) ^ 0x177cb70000000000ULL));

struct JEvA {
    int SesW, Z5Bg, Cfp0;
};

int koO9[uPqt];
bitset<uPqt> oQYG[uPqt];
bitset<uPqt> DBiC[uPqt];
int NiSD;
map<int, int> tnrx;

// DSU Find
int find_set(int kNrw) {
    if (kNrw == koO9[kNrw]) return kNrw;
    return koO9[kNrw] = find_set(koO9[kNrw]);
}

// Get compressed ID
int get_id(int P89U) {
    if (tnrx.find(P89U) == tnrx.end()) {
        tnrx[P89U] = NiSD++;
    }
    return tnrx[P89U];
}

// Merge set b into set a
void union_sets(int WgpX, int X2T5) {
    WgpX = find_set(WgpX);
    X2T5 = find_set(X2T5);
    if (WgpX != X2T5) {
        // Merge Row Constraints (What constraints apply to this group)
        oQYG[WgpX] |= oQYG[X2T5];
        DBiC[WgpX] |= DBiC[X2T5];
        
        // Merge Column Constraints (Update references in other groups)
        // Since 'b' is now 'a', any group containing 'b' now contains 'a'.
        // This effectively renames bit 'b' to 'a' in all bitsets.
        for (int MmXN = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)); MmXN < NiSD; ++MmXN) {
            // Only iterate active roots to save time, though strictly i < id_counter covers all.
            // Using parent[i]==i check might be an optimization, but loop is fast enough for N=3000 sum.
            if (koO9[MmXN] == MmXN) {
                if (oQYG[MmXN].test(X2T5)) {
                    oQYG[MmXN].reset(X2T5);
                    oQYG[MmXN].set(WgpX);
                }
                if (DBiC[MmXN].test(X2T5)) {
                    DBiC[MmXN].reset(X2T5);
                    DBiC[MmXN].set(WgpX);
                }
            }
        }
        
        koO9[X2T5] = WgpX;
    }
}

void solve() {
    int ar6s;
    if (!(cin >> ar6s)) return;

    // Reset for new test case
    tnrx.clear();
    NiSD = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL));
    
    struct gahR {
        int jGpA; // 0: rec, 1: not-rec, 2: inter
        int B9Vc, IF5z, LO5R;
    };
    vector<RawStmt> d1V8;
    d1V8.reserve(ar6s);

    for (int zEWi = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)); zEWi < ar6s; ++zEWi) {
        string oqFh;
        cin >> oqFh;
        if (oqFh == (reinterpret_cast<const char*>(H4yJ(nmy9, 0x2f)))) {
            int bZeJ, tOq3; cin >> bZeJ >> tOq3;
            d1V8.push_back({((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)), bZeJ, tOq3, ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL))});
        } else if (oqFh == (reinterpret_cast<const char*>(H4yJ(L3Q1, 0xb7)))) {
            int NNOG, Pxpq; cin >> NNOG >> Pxpq;
            d1V8.push_back({((int)(((unsigned long long)Fpba) ^ 0x8469fa0000000000ULL)), NNOG, Pxpq, ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL))});
        } else {
            int SD9q, nXqq, gaOj; cin >> SD9q >> nXqq >> gaOj;
            d1V8.push_back({((int)(((unsigned long long)SVhu) ^ 0x7495f50000000000ULL)), SD9q, nXqq, gaOj});
        }
    }

    // Assign internal IDs
    vector<Intersection> qsZz;
    for (auto& p5AE : d1V8) {
        p5AE.B9Vc = get_id(p5AE.B9Vc);
        p5AE.IF5z = get_id(p5AE.IF5z);
        if (p5AE.jGpA == ((int)(((unsigned long long)SVhu) ^ 0x7495f50000000000ULL))) {
            p5AE.LO5R = get_id(p5AE.LO5R);
            qsZz.push_back({p5AE.B9Vc, p5AE.IF5z, p5AE.LO5R});
        }
    }

    // Initialize Structures
    for (int xVX7 = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)); xVX7 < NiSD; ++xVX7) {
        koO9[xVX7] = xVX7;
        oQYG[xVX7].reset();
        DBiC[xVX7].reset();
    }

    // Apply Initial Constraints
    // Note: We use find_set(x) immediately to handle any pre-existing logic if we were dynamic,
    // but here initially everyone is their own root.
    for (auto& xnZf : d1V8) {
        int pk0B = xnZf.B9Vc; 
        int aiS4 = xnZf.IF5z;
        if (xnZf.jGpA == ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL))) { // recommend u v -> u in S_v
            oQYG[aiS4].set(pk0B);
        } else if (xnZf.jGpA == ((int)(((unsigned long long)Fpba) ^ 0x8469fa0000000000ULL))) { // not-recommend u v -> u not in S_v
            DBiC[aiS4].set(pk0B);
        }
    }

    bool h9Q2 = true;
    while (h9Q2) {
        h9Q2 = false;

        // 1. Detect aliases via duplicate intersection definitions
        // If S_u = S_v \cap S_w and S_x = S_v \cap S_w, then u == x.
        map<pair<int, int>, int> PyUJ;
        
        for (auto& ZCVM : qsZz) {
            int vEd8 = find_set(ZCVM.SesW);
            int SYmo = find_set(ZCVM.Z5Bg);
            int nWMR = find_set(ZCVM.Cfp0);

            // Normalize order for key
            if (SYmo > nWMR) swap(SYmo, nWMR);

            // Special Case: S_u = S_v \cap S_v => S_u = S_v => u == v
            if (SYmo == nWMR) {
                if (vEd8 != SYmo) {
                    union_sets(vEd8, SYmo);
                    h9Q2 = true;
                }
            } else {
                if (PyUJ.count({SYmo, nWMR})) {
                    int BEmK = PyUJ[{SYmo, nWMR}];
                    if (BEmK != vEd8) {
                        union_sets(BEmK, vEd8);
                        h9Q2 = true;
                    }
                } else {
                    PyUJ[{SYmo, nWMR}] = vEd8;
                }
            }
        }
        
        // 2. Propagate Intersection Logic
        // Re-iterate because unions might have changed roots
        for (auto& BYmp : qsZz) {
            int rlMC = find_set(BYmp.SesW);
            int YQ6d = find_set(BYmp.Z5Bg);
            int CTUL = find_set(BYmp.Cfp0);

            // S_u = S_v \cap S_w
            
            // Logic: x in Sv AND x in Sw => x in Su
            bitset<uPqt> xyNK = oQYG[YQ6d] & oQYG[CTUL];
            if ((oQYG[rlMC] | xyNK) != oQYG[rlMC]) {
                oQYG[rlMC] |= xyNK;
                h9Q2 = true;
            }

            // Logic: x in Su => x in Sv AND x in Sw
            if ((oQYG[YQ6d] | oQYG[rlMC]) != oQYG[YQ6d]) {
                oQYG[YQ6d] |= oQYG[rlMC];
                h9Q2 = true;
            }
            if ((oQYG[CTUL] | oQYG[rlMC]) != oQYG[CTUL]) {
                oQYG[CTUL] |= oQYG[rlMC];
                h9Q2 = true;
            }

            // Logic: x not in Sv => x not in Su (since Su subset Sv)
            if ((DBiC[rlMC] | DBiC[YQ6d]) != DBiC[rlMC]) {
                DBiC[rlMC] |= DBiC[YQ6d];
                h9Q2 = true;
            }
            if ((DBiC[rlMC] | DBiC[CTUL]) != DBiC[rlMC]) {
                DBiC[rlMC] |= DBiC[CTUL];
                h9Q2 = true;
            }

            // Logic: x not in Su AND x in Sv => x not in Sw
            bitset<uPqt> BNIr = DBiC[rlMC] & oQYG[YQ6d];
            if ((DBiC[CTUL] | BNIr) != DBiC[CTUL]) {
                DBiC[CTUL] |= BNIr;
                h9Q2 = true;
            }
            
            // Logic: x not in Su AND x in Sw => x not in Sv
            bitset<uPqt> zPNY = DBiC[rlMC] & oQYG[CTUL];
            if ((DBiC[YQ6d] | zPNY) != DBiC[YQ6d]) {
                DBiC[YQ6d] |= zPNY;
                h9Q2 = true;
            }
        }
    }

    // Final Consistency Checks
    int RT1O = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL));
    for (int dUhi = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)); dUhi < NiSD; ++dUhi) {
        if (koO9[dUhi] == dUhi) {
            RT1O++;
            // 1. Contradiction check
            if ((oQYG[dUhi] & DBiC[dUhi]).any()) {
                cout << (reinterpret_cast<const char*>(H4yJ(JX8c, 0xa9))) << endl;
                return;
            }
            // 2. Self-loop check (x recommends x)
            if (oQYG[dUhi].test(dUhi)) {
                cout << (reinterpret_cast<const char*>(H4yJ(JX8c, 0xa9))) << endl;
                return;
            }
        }
    }

    // 3. Cycle Detection (Topological Sort)
    // Build graph where edge U -> V exists if group V contains member U
    // Due to union_sets logic, bits in bitsets correspond to current roots.
    vector<vector<int>> OuIG(NiSD);
    vector<int> sK8C(NiSD, ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)));

    for (int F8kn = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)); F8kn < NiSD; ++F8kn) {
        if (koO9[F8kn] != F8kn) continue;
        
        // For every active root u, if u is in must_have[v], add u -> v
        // We can iterate all k set in must_have[v]. 
        // Since we migrated columns, only active roots should be set.
        for (int RGvF = oQYG[F8kn]._Find_first(); RGvF < uPqt; RGvF = oQYG[F8kn]._Find_next(RGvF)) {
             // u is a recommender of v. So u happened before v. Edge u -> v.
             // Double check u is a root (it should be if logic is correct).
             // If u is not a root, find_set(u) is the root.
             int XPJn = find_set(RGvF);
             if (XPJn != RGvF) {
                 // This theoretically shouldn't happen with full migration, 
                 // but safe to handle.
             }
             OuIG[XPJn].push_back(F8kn);
             sK8C[F8kn]++;
        }
    }

    // Kahn's Algorithm
    vector<int> tTgB;
    for (int iuoT = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL)); iuoT < NiSD; ++iuoT) {
        if (koO9[iuoT] == iuoT && sK8C[iuoT] == ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL))) {
            tTgB.push_back(iuoT);
        }
    }

    int fj3H = ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL));
    while (!tTgB.empty()) {
        int I1GC = tTgB.back(); tTgB.pop_back();
        fj3H++;
        for (int Z9TO : OuIG[I1GC]) {
            sK8C[Z9TO]--;
            if (sK8C[Z9TO] == ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL))) tTgB.push_back(Z9TO);
        }
    }

    if (fj3H < RT1O) {
        cout << (reinterpret_cast<const char*>(H4yJ(JX8c, 0xa9))) << endl;
    } else {
        cout << (reinterpret_cast<const char*>(H4yJ(uUcl, 0x61))) << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int nJpS;
    if (cin >> nJpS) {
        while (nJpS--) {
            solve();
        }
    }
    return ((int)(((unsigned long long)jjaa) ^ 0x2452220000000000ULL));
}