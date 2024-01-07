enum class Color {
    RED {
        override fun signal(): Color {
            return BLUE
        }
    },
    BLUE {
        override fun signal(): Color {
            return RED
        }
    },
    GREEN {
        override fun signal(): Color {
            return BLUE
        }
    };

    abstract fun signal(): Color
}

fun main(args: Array<String>) {
    println(Color.RED.signal())
}