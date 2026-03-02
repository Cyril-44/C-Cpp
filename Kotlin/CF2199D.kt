import kotlin.math.*
fun main() {
    fun readList() = readln().split(' ').map(String::toInt)
    val T = readln().toInt()
    repeat(T) {
        val (n, m) = readList()
        val a = readList().toMutableList(); val b = readList().toMutableList()
        if (n > 1) { a.removeFirst(); a.removeLast() }
        if (m > 1) { b.removeFirst(); b.removeLast() }
        var flg = false
        for (ai in a)
            if (b.binarySearch(ai) >= 0) { flg = true; break }
        if (flg) println("YES") else println("NO")
    }
}