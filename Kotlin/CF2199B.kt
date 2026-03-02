import kotlin.math.*
fun main() {
    fun readList() = readln().split(' ').map(String::toInt)
    val T = readln().toInt()
    repeat (T) {
        var (a, b, c, d) = readList()
        var ans = c - a + d - b
        ans -= (min(c,d) - max(a, b)).let {if (it < 0) 0 else it}
        println(ans)
    }
}