fun main(args: Array<String>) {
    val v1 = OverrideTest(1, 2)
    val v2 = OverrideTest(3, 4)
    println((v1 + v2).toString())
}

data class OverrideTest(val x: Int, val y: Int)

operator fun OverrideTest.plus(o: OverrideTest): OverrideTest {
    return OverrideTest(x + o.x, y + o.y)
}