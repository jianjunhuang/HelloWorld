fun main(args: Array<String>) {
    //box & unbox
    val a: Int = -127
    val boxedA: Int? = a
    println(a == boxedA)
    println(a === boxedA)

    val anotherA: Int? = a
    println(boxedA == anotherA)
    println(boxedA === anotherA)
    println("$a,$boxedA,$anotherA")

    //Explicit Conversions
    val l: Long = 1000
    val c = a + l
    println("c = $c ${c.javaClass}")
}