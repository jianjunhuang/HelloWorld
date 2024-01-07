fun main(args: Array<String>) {
    ranges5()
}

/*
 *Check if a number is within a range using in operator:
 */
fun ranges1() {
    val x = 1
    val y = 10
    if (x in 0..y) {
        println("fits in the range")
    }
}

/*
check if a number is out of range
 */
fun ranges2() {
    val list = listOf("a", "b", "c")
    if (-1 !in 0..list.lastIndex) {
        println("-1 is out of range")
    }
    if (list.size !in list.indices) {
        println("list size is out of valid list indices range too")
    }
}

/*
iterating over a range
 */
fun ranges3() {
    for (x in 1..5) {
        println(x)
    }
}

/*
over a progression(级数)
 */
fun ranges4() {
    for (x in 1..10 step 2) {
        println(x)
    }
    println()
    for (x in 20 downTo 0 step 3) {
        println(x)
    }
}

fun ranges5() {
    for (x in 1..5) {
        print(x)
    }
    println()
    for(x in 1 until 5){
        print(x)
    }
}