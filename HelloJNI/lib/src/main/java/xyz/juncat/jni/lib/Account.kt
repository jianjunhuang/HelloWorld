package xyz.juncat.jni.lib

data class Account(
    val id: Int,
    var nickname: String,
    val password: String
) {

    fun changeName(name: String): Boolean {
        nickname = name
        return true
    }

    companion object {
        var staticId = 0
    }
}
