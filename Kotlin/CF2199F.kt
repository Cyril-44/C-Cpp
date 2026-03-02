import kotlin.math.*
fun main() {
    val MOD = 998244353
    fun qpow(B : Int, N : Int) : Int {
        var res = 1; var b = B; var n = N
        while (n > 0) {
            if (n % 2 == 1) res = (res.toLong() * b % MOD).toInt()
            b = (b.toLong() * b % MOD).toInt()
            n /= 2
        }
        return res
    }
    fun readList() = readln().split(' ').map(String::toInt)
    val T = readln().toInt()
    repeat (T) {
        val n = readln().toInt()
        val a = readList()

        val freq = a.indices.groupBy { a[it] }
        var ans = qpow(2, a.count{it==0})
        var lCnt = 0; var rCnt = 0
        val lVal = IntArray(n){0}; val rVal = IntArray(n){0}
        for (i in n-1 downTo 0) {
            if (a[i] == 0) ++rCnt
            else rVal[i] = qpow(2, rCnt)
        }
        for (i in 0..<n) {
            if (a[i] == 0) ++ lCnt
            else lVal[i] = qpow(2, lCnt)
        }

        for ((x, ls) in freq) {
            if (x == 0) continue
            var curRVal = 0
            for (i in ls) curRVal = (curRVal + rVal[i]) % MOD
            for (i in ls) {
                curRVal = (curRVal - rVal[i] + MOD) % MOD
                ans = ((ans + lVal[i] * curRVal.toLong()) % MOD).toInt()
            }
        }
        println(ans)
    }
}