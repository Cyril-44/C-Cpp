fun main() {
    class DSU(n : Int) {
        private val fa = IntArray(n+1){ it }
        fun find(node : Int) : Int {
            var u = node; var v = node
            while (u != fa[u]) u = fa[u]
            while (v != u) { val tp = fa[v]; fa[v] = u; v = tp }
            return u
        }
        fun unite(u : Int, v : Int) : Boolean {
            val fu = find(u); val fv = find(v)
            if (fu != fv) { fa[fu] = fv; return true }
            return false;
        }
        fun isSame(u : Int, v : Int) : Boolean = find(u) == find(v)
        fun countRoot(start : Int = 1, end : Int = fa.lastIndex) : Int {
            var cnt = 0
            for (i in start..end) if (fa[i] == i) ++cnt
            return cnt
        }
    }
    val T = readln().toInt()
    repeat (T) {
        val (n, m1, m2) = readln().split(" ").map(String::toInt)
        val g1 = Array<Pair<Int,Int>>(m1){Pair(0,0)}
        val dsu1 = DSU(n+1); val dsu2 = DSU(n+1)
        for (i in 0..<m1) {
            val (u, v) = readln().split(' ').map(String::toInt)
            g1[i] = Pair(u, v)
        }
        repeat (m2) {
            val (u, v) = readln().split(' ').map(String::toInt)
            dsu2.unite(u, v)
        }
        var ans = 0
        for ((u, v) in g1) {
            if (!dsu2.isSame(u, v)) ++ans
            else dsu1.unite(u, v)
        }
        val cr1 = dsu1.countRoot(); val cr2 = dsu2.countRoot()
        println(cr1 - cr2 + ans)
    }
}