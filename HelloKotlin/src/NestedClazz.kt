class Outer {
    private val bar: Int = 1

    init {
        println("Outer init")
    }
    class Nested {
        init {
            println("Nested init")
        }
        fun foo() = 2
//        fun out() = Outer.bar
    }

    inner class Inner {
        init {
            println("Inner init")
        }
        fun foo() = bar
    }
}

fun main(args: Array<String>) {
//    val demo = Outer.Nested().foo() // == 2
//    println(demo)
    println(Outer().Inner().foo())
}