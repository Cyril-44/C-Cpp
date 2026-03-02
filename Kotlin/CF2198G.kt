import kotlin.math.*
fun main() {
    fun List<Int>.lowerBound(key: Int): Int {
        val idx = this.binarySearch(key)
        val p = if (idx < 0) -idx - 1 else idx
        return p
    }

    fun readList() = readln().split(' ').map(String::toInt)
    val T = readln().toInt()
    repeat(T) {
        val (_n, _s, _x) = readln().split(' ')
        val n = _n.toInt(); val s = _s.toLong(); val x = _x.toInt()
        val a = readList()
        val sum = LongArray(n+1) {0}
        for (i in 1..n) sum[i] = sum[i-1] + a[i-1]
        var lastGtxIdx = -1; var lastEqxIdx = -1
        val f = sortedMapOf<Long, ArrayList<Int>>()
        f[0] = arrayListOf(0)
        var ans = 0
        for (i in 1..n) {
            if (a[i-1] > x) lastGtxIdx = i
            if (a[i-1] == x) lastEqxIdx = i
            f.getOrPut(sum[i]) { arrayListOf() }.add(i)
            val fx = f.get(sum[i] - s)
            if (fx != null) {
                ans += max(0,
                    fx.lowerBound(lastEqxIdx)
                    - fx.lowerBound(lastGtxIdx))
            }
        }
        println(ans)
    }
}