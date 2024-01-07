data class Person1(val name: String) {
    var age: Int = 0
}

fun main(args: Array<String>) {
    val person1 = Person1("John")
    val person2 = Person1("John")
    person1.age = 10
    person2.age = 20
    println("person1 == person2: ${person1 == person2}")
    println("person1 with age ${person1.age}: ${person1}")
    println("person2 with age ${person2.age}: ${person2}")
    val person3 = person1.copy(name = "jianjun")
    println("persion3 $person3 ${person3.age}")
}