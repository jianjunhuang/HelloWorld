abstract class ObjectTest {
    val x: String = "objectTest"
    abstract fun test()
}

fun main(args: Array<String>) {
    test(object : ObjectTest() {
        init {
            println("object init")
        }

        override fun test() {
            println("test")
        }
    })

    val o = object {
        val member = "member"
        override fun toString(): String {
            return member
        }
    }
    println(o.member)
    println(o.toString())
    println("private ${foo().x}")
    println("public ${publicFoo().toString()}")
    println("public 2 ${publicFoo2().x}")

    SingleInstance.test()
}

fun test(test: ObjectTest) {
    test.test()
}

private fun foo() = object {
    val x: String = "x"
}

// Public function, so the return type is Any
fun publicFoo() = object {
    val x: String = "x"
}

fun publicFoo2() = object : ObjectTest() {
    override fun test() {

    }
}

object SingleInstance {
    init {
        println("Single instance init")
    }

    fun test() {
        println("single instance test")
    }

}


class CompanionClass {
    companion object {
        const val TEST = "Test"
        fun test() {

        }
    }
}

class CompanionClass2 {
    companion object Tst {
        const val TEST = "Test"
        fun test() {

        }
    }
}

class CompanionTestClass {
    fun main(args: Array<String>) {
//        println(CompanionClass.TEST)
//        CompanionClass.test()
        t(object : TestInterface {
            override fun test() {
                println("hh")
            }

        })
    }

    fun t(i: TestInterface) {
        i.test()
    }
}

interface TestInterface {
    fun test()
}


