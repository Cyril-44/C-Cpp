fun main() {
    val T = readln().toInt()
    repeat(T) {
        val n = readln().toInt()
        val arr = readln().split(' ').map(String::toInt)
        println((arr.groupBy {it}).entries.fold(0) {acc, lst -> acc + lst.value.size / 2})
    }
}
