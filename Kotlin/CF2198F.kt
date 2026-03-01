import kotlin.math.*
fun main() {
    fun readList() = readln().split(' ').map(String::toInt)
    data class Node(var adds : Int = 0, var dels : Int = 0)
    val T = readln().toInt()
    repeat(T) {
        val (n, k) = readList()
        val a = readList(); val b = readList()
        val f = sortedMapOf<Int, Node>()
        for (ai in a) f.getOrPut(ai, {Node()}).adds++
        for (bi in b) f.getOrPut(bi, {Node()}).dels++
        var buyCnt = n.toLong(); var negCnt = 0
        var ans = 0L
        for ((i, j) in f) {
            if (negCnt <= k) ans = max(ans, i * buyCnt)
            buyCnt -= j.dels; negCnt += j.adds - j.dels
        }
        println(ans)
    }
}
