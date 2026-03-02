import kotlin.math.*
fun main() {
    val T = readln().toInt()
    repeat (T) {
        val num = readln().trim()
        val freq = num.groupingBy {it}.eachCount().withDefault {0}
        var rem = 0
        for (ch : Char in '0'..'9')
            rem = (rem + (ch - '0') * freq.getValue(ch)) % 9
        val dp = BooleanArray(9+18+18 + 1) {false}
        dp[rem] = true
        repeat (min(freq.getValue('2'), 9)) {
            for (i in dp.lastIndex downTo 2)
                dp[i] = dp[i] || dp[i - 2]
        }
        repeat(min(freq.getValue('3'), 3)) {
            for (i in dp.lastIndex downTo 6)
                dp[i] = dp[i] || dp[i - 6]
        }
        var ok = false
        for (i in 0..dp.lastIndex step 9)
            ok = ok || dp[i]
        println(if (ok) "YES" else "NO")
    }
}
