fun main(args: Array<String>) {
    val items = listOf(1, 2, 3, 4, 5, 6, 7, 8, 9)
//    println("${loop1(items)}")
//    println("${loop2(items)}")
//    while1(items)
//    loop3(items)
    loop4()
}

fun loop1(items: List<String>?) {
    if (items == null) {
        return
    }
    for (item in items) {
        println(item)
    }
}

fun loop2(items: List<Int>) {
    for (index in items.indices) {
        println(items[index])
    }
}

fun loop3(items: List<Int>) {
    loop@ for (i in 10..100) {
        for (j in 1..10) {
            if (j == 5) break@loop
            print(j)
        }
        print(i)
    }
}

fun loop4() {
    listOf(1, 2, 3, 4, 5).forEach(fun(value: Int) {
        if (value == 3) return  // 局部返回到匿名函数的调用者，即 forEach 循环
        print(value)
    })
    print(" done with anonymous function")
}

fun while1(items: List<String>) {
    var i = 0
    while (i < items.size) {
        println(items[i])
        i++
    }
}

