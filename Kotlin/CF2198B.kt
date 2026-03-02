fun main() {
    val T = readln().toInt()
    repeat(T) {
        val a = readln().split(' ').map(String::toInt)
        val req = listOf(a[0] + a[1], a[2] - a[1], a[3] - a[2])
        println(req.groupingBy{it}.eachCount().values.max())
    }
}