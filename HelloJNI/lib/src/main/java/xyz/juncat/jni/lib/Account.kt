package xyz.juncat.jni.lib

data class Account(
    val id: Int,
    var nickname: String,
    val password: String
) {
    companion object {
        var staticId = 0
    }
}
