import kotlin.math.*
fun main() {
    data class Node(var lastOccIdx : Int, var cnt : Int)
    fun readList() = readln().split(' ').map(String::toInt)
    val T = readln().toInt()
    repeat(T) {
        val (_n, _s, _x) = readln().split(' ')
        val n = _n.toInt(); val s = _s.toLong(); val x = _x.toInt()
        val a = readList()
        val sum = LongArray(n+1) {0}
        sum[0] = -s
        for (i in 1..n) sum[i] = sum[i-1] + a[i-1]
        var lastGtxIdx = -1; var lastEqxIdx = -1
        val f = sortedMapOf<Long, Node>()
        var ans = 0
        for (i in 1..n) {
            if (a[i-1] > x) {
                f.remove(sum[i])
                lastGtxIdx = i
            } else {
                if (a[i-1] == x) lastEqxIdx = i
                val fx = f.getOrPut(sum[i], {Node(i, 0)})
                fx.cnt++
                if (fx.lastOccIdx <= lastGtxIdx) { fx.lastOccIdx = i; fx.cnt = 1 }
                if (fx.lastOccIdx <= lastEqxIdx) ans += fx.cnt
            }
        }
        println(ans)
    }
}