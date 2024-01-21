package xyz.juncat.jni.lib

class HelloJNI {

    init {
        System.loadLibrary("hellojni")
    }

    external fun stringFromJNI(): String

    external fun dynamicStringFromJNI(): String

    external fun stringInNative(input: String): String

    external fun referenceInJNI(strArray: Array<String>): Array<String>

    external fun accessAccount(account: Account)

    external fun accessStaticFiled(account: Account)

    external fun accessAccountMethod(account: Account)
    external fun accessAccountMethodWithArg(account: Account)
}