data class PersonL(val age: Int, val name: String)

lateinit var test: PersonL
var test2: PersonL? = null

fun main(args: Array<String>) {
    test = PersonL(0, "1")
    val p = ::PersonL
    val t = p(0, "2")
    println(::PersonL.name)
    test2?.let {
        println(it)
    }
    val list = listOf(1, 2, 3, 4, 5)
    list.asSequence()
            .map { it + 1 }
            .filter { it % 2 == 0 }
            .toList()

    testLambda {
        it == 1
    }
    val array = ArrayList<Boolean>(1000000000)
    lookFor4(array)
}

// lambda 就是一个对象
fun testLambda(test: (Int) -> Boolean): Boolean {
    return test.invoke(1)
}

fun lookFor(list: List<Boolean>) {
    list.forEach {
        if (it) {
            return
        }
    }
}

fun lookFor2(list: List<Boolean>) {
    list.forEach {
        println("return for each $it")
        if (!it) {
            return@forEach
        }
        println("return for each $it")
    }
    println("return")
}

fun lookFor3(list: List<Boolean>) {
    for (l in list) {
        if (l) {
            println("return for each $l")
            break
        }
        println("return")
    }
    println("return")
}

fun lookFor4(list: List<Boolean>) {
    list.forEach(fun(b) {
        println("return $b")
        if (b) {
            println("return $b")
            return
        }
        println("return $b")
    })

    println("return")
}