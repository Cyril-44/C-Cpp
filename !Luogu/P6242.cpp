#include <stdio.h>
#include <algorithm>
#define umax(x, y) ((x) = std::max((x), (y)))
constexpr int N = 500005;
namespace SegTr {
struct Node {
	int size; // 这个节点代表的区间的长度
	int sum, mx, mx2, mxcnt, hmx;
// 懒标记:
	struct Tag {
		int mx, hmx; // 对于 mx 的懒标记，从该节点 pushdown 以来的最大 mx 懒标记
		int x, hx; // 对于不是 mx 的懒标记，从该节点 pushdown 以来的最大 非mx 懒标记
	} add;
	inline void pull(const Tag &t) {
	// 更新线段树
		umax(hmx, mx + t.hmx);
		sum += mxcnt * t.mx + (size - mxcnt) * t.x;
		mx += t.mx, mx2 += t.mx2;
		
	// 更新树上懒标记
	}
} tr[N << 2];
}
int main() {

	return 0;
}