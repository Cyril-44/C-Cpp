#include <cstdio>
#include <cstring>
#include <algorithm>
int main() {
    int l1lll1l11l111l1l11111llll1l11l1l1;
    long long llllll1lll1llll1lll11l1l1l11l1ll1, l11l1lll1111ll1l111l11111l11l11l1, llll11llll111l1l1l11111lll11llll1;
    scanf("\u0025\u0064", &l1lll1l11l111l1l11111llll1l11l1l1);
    while (l1lll1l11l111l1l11111llll1l11l1l1--) {
        scanf("\u0025\u006C\u006C\u0064\u0025\u006C\u006C\u0064\u0025\u006C\u006C\u0064", &llllll1lll1llll1lll11l1l1l11l1ll1, &l11l1lll1111ll1l111l11111l11l11l1, &llll11llll111l1l1l11111lll11llll1);
        long long l11lll111lllll1l11l11ll1l1lllll1l = std::__gcd(llllll1lll1llll1lll11l1l1l11l1ll1, l11l1lll1111ll1l111l11111l11l11l1);
        long long ll1l1llll1ll11l1111l1lll111l1lll1 = std::max(llllll1lll1llll1lll11l1l1l11l1ll1/l11lll111lllll1l11l11ll1l1lllll1l, l11l1lll1111ll1l111l11111l11l11l1/l11lll111lllll1l11l11ll1l1lllll1l);
        if (llll11llll111l1l1l11111lll11llll1 >= ll1l1llll1ll11l1111l1lll111l1lll1) puts("\u0031");
        else puts("\u0032");
    }
    return 0;
}