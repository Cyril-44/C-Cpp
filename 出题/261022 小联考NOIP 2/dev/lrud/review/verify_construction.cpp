// Verify every allowed N without materializing hundreds of megabytes on disk.
#define main submitted_main
#include "../sol/std_maximum.cpp"
#undef main
int main() {
    long long cells = 0;
    for (int n = 2; n <= 1000; ++n) {
        string s = maximumPath(n);
        if (s.size() != size_t(n*n-1)) return 1;
        const string dirs = "QWEADZXC";
        const int dr[] = {-1,-1,-1,0,0,1,1,1};
        const int dc[] = {-1,0,1,-1,1,-1,0,1};
        vector<unsigned char> seen(n*n);
        seen[0] = 1;
        int r=0,c=0,d=0;
        for (char ch : s) {
            auto k = dirs.find(ch);
            if (k == string::npos) return 2;
            r += dr[k]; c += dc[k];
            if (r<0 || r>=n || c<0 || c>=n || seen[r*n+c]++) return 3;
            d += ch=='D';
        }
        if (r!=n-1 || c!=n-1 || d!=maximumK(n)) return 4;
        cells += 1LL*n*n;
    }
    cout << "N=2..1000 passed; " << cells << " cells verified\n";
}
