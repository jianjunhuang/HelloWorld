open class A {
    open fun foo(i: Int = 10, j: Int) {
        println("$i,$j")
    }

}

class B : A() {
    override fun foo(i: Int, j: Int) {
        println(i)
    }  // 不能有默认值

    fun foo1(bar: Int = 0, baz: Int = 1, qux: () -> Unit) {
        println("$bar , $baz ")
    }
}

fun <T> asList(vararg ts: T): List<T> {
    val result = ArrayList<T>()
    result.addAll(ts)
    return result
}

class C {

    infix fun add(i: Int): Int {
        return i * 10
    }

    fun get(): Int {
        return this add 10
    }
}


fun main(args: Array<String>) {
//    A().foo(j = 20)
//    B().foo1(1) { println("hi") }
    println(asList(*arrayOf(1, 2, 4, 5)))
    println(C().get())
}