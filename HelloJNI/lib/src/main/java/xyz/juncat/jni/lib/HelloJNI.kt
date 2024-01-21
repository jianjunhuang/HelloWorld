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

    external fun callbackFromJNI(callback: Callback)

    external fun callbackFromJNIThread(callback: Callback)

    external fun createAccountByJNI(): Account


    interface Callback {
        fun onCall()
    }
}