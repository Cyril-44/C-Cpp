import kotlin.math.*
fun main() {
    fun readList() = readln().split(' ').map(String::toInt)
    val T = readln().toInt()
    repeat (T) {
        val k = readln().toInt()
        val (a1, b1) = readList()
        val (a2, b2) = readList()
        val a = a1 + a2; val b = b1 + b2 + k
        if (b > a || b == a && (b1 > a1 || b2 > a2)) println("YES")
        else println("NO")
    }
}