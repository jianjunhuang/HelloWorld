interface MyInterface {
    val property: String
    fun bar()
    fun ball() {

    }
}

class Clazz : MyInterface {

    override val property: String
        get() = ""

    override fun bar() {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

}