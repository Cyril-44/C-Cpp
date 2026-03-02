import kotlin.math.*
fun main() {
    val T = readln().toInt()
    repeat(T) {
        val k = readln().toInt()
        when (k % 5) {
            0 -> {
                val cols = k / 5 * 3
                println("YES"); println(cols)
                repeat(cols / 3) { print(".*.") }; println()
                repeat(cols / 3) { print("...") }; println()
            }
            1 -> {
                val cols = k / 5 * 3 + 1
                println("YES"); println(cols)
                repeat(cols / 3) { print("*..") }; println("*")
                repeat(cols / 3) { print("...") }; println(".")
            }
            2 -> { println("NO") }
            3 -> {
                val cols = k / 5 * 3 + 2
                println("YES"); println(cols)
                repeat(cols / 3) { print(".*.") }; println(".*")
                repeat(cols / 3) { print("...") }; println("..")
            }
            4 -> { println("NO") }
        }
    }
}