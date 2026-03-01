fun main() {
    val T = readln().toInt()
    repeat(T) {
        val (n, m, k) = readln().split(" ").map(String::toInt)
        val a = readln().split(' ').map(String::toInt)
        val q = readln().split(' ').map(String::toInt)
        var ans = BooleanArray(n+1){false}
        when (q.size) {
            (n-1) -> {
                var idx = 1;
                var miss = -1
                for (qi in q) {
                    if (qi != idx) {
                        miss = idx; break
                    }
                    ++idx
                }
                if (miss == -1) miss = n
                ans[miss] = true
            }
            n -> { ans.fill(true) }
        }
        for (query in a)
            if (ans[query]) print('1') else print('0')
        println()
    }
}