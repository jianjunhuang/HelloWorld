//
// Created by jianjun huang on 2024/1/21.
//

#include <jni.h>


extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_handleExceptionFromJava(JNIEnv *env, jobject thiz) {
    jclass cls = env->GetObjectClass(thiz);
    jmethodID mId = env->GetMethodID(cls, "createException", "()V");
    env->CallVoidMethod(thiz, mId);
    jthrowable exc = env->ExceptionOccurred();
    if (exc) {
        //print exception
        env->ExceptionDescribe();
        //avoid crash
        env->ExceptionClear();
    }
}
extern "C"
JNIEXPORT jthrowable JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_throwExceptionFromJNI(JNIEnv *env, jobject thiz) {
    jclass cls = env->FindClass("java/lang/IllegalArgumentException");
    env->ThrowNew(cls, "exception in JNI");
    return nullptr;
}