import kotlin.math.*
fun main() {
    fun readList() = readln().split(' ').map(String::toInt)
    fun readLongList() = readln().split(' ').map(String::toLong)
    val T = 1
    repeat(T) {
        val (n, q) = readList()
        val a = readList(); val x = readLongList()
        val que = sortedMapOf<Long, ArrayList<Int>>()
        val ans = IntArray(q){-1}
        for (i in 0..<q)
            que.getOrPut(x[i],{arrayListOf()}).add(i)
        var pos = 1L
        val qx = que.toList()
        var idx = 0
        for (i in 1..<n) {
            val prePos = pos
            pos += abs(a[i] - a[i-1])
            val dir = if (a[i] > a[i-1]) 1 else if (a[i] < a[i-1]) -1 else 0
            if (a[i] == a[i-1]) pos += 2
            while (idx < qx.size && qx[idx].first <= pos) {
                for (id in qx[idx].second) ans[id] = if (dir == 0 && qx[idx].first == pos - 1) 0 else (a[i-1] + dir * (qx[idx].first - prePos).toInt())
                idx++
            }
        }
        for (i in 0..<q) print("${ans[i]} ")
        println()
    }
}