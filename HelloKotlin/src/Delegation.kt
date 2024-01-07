import kotlin.properties.Delegates

interface Base {
    val message: String
    fun print()
    fun overridePrint()
}

class BaseImpl(val x: Int) : Base {
    override val message: String = "test"

    override fun overridePrint() {
        println(x)
    }

    override fun print() {
        println("$x $message")
    }
}

class Derived(b: Base) : Base by b {
    override fun overridePrint() {
        println("override")
    }

    override val message: String
        get() = "test override"
}

var num = 1

val lazyStr: String by lazy {
    println("computed！")
    "lazy $num"
}

var observableStr: String by Delegates.observable("hi") { property, oldValue, newValue ->
    println("property:$property , oldValue:$oldValue , newValue:$newValue")
}

var vetoableStr: String by Delegates.vetoable("init") { property, oldValue, newValue ->
    println("$property $oldValue $newValue")
    false
}

fun main() {
//    val b = BaseImpl(10)
//    val d = Derived(b)
//    d.print()
//    d.overridePrint()

//    println(lazyStr)
//    num++
//    num++
//    println(lazyStr)

    println(observableStr)
    observableStr = "hello"
    println(observableStr)

//    println(vetoableStr)
//    vetoableStr = "hi"
//    println(vetoableStr)
}