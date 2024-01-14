package xyz.juncat.jni.lib

class HelloJNI {

    init {
        System.loadLibrary("hellojni")
    }

    external fun stringFromJNI(): String
}