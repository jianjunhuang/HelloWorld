open class Person(name: String) {

    init {
        println("init $name")
    }

    constructor(name: String, sex: Int) : this(name = name) {
        println("sub $name , $sex")
    }

    open fun eat() {
        println("eat")
    }
}

class Student : Person(name = "test") {
    override fun eat() {
        println("student eat")
    }
}

fun main(args: Array<String>) {
//    val person = Person("haha", 1)
    val student = Student()
    student.eat()
}