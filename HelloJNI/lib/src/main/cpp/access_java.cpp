#include <jni.h>
#include <logutil.h>
#include <bits/pthread_types.h>
#include <pthread.h>
#include "jvm_holder.h"

//
// Created by jianjun huang on 2024/1/17.
//

/*
 * Java     | JNI
*   boolean | Z
*   byte    | B
*   char    | C
*   short   | S
*   int     | I
*   Long    | J
*   float   | F
*   double  | D
 */

/*
 *|Java     | JNI
 *|String   | Ljava/lang/String;
 *|Class    | Ljava/lang/Class;
 *|Throwable| Ljava/lang/Throwable;
 *|int[]    | [I
 *|Object[] | [Ljava/lang/Object;
 */

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_accessAccount(JNIEnv *env, jobject thiz, jobject account) {
    jclass cls = env->GetObjectClass(account);
    jfieldID fId = env->GetFieldID(cls, "nickname", "Ljava/lang/String;");
    jstring str = env->NewStringUTF("hhhh");
    env->SetObjectField(account, fId, str);
}
extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_accessStaticFiled(JNIEnv *env, jobject thiz, jobject account) {
    jclass cls = env->GetObjectClass(account);
    jfieldID fId = env->GetStaticFieldID(cls, "staticId", "I");
    int id = env->GetStaticIntField(cls, fId);
    LOGD("get static id -> %d", id);
    env->SetStaticIntField(cls, fId, 11);

}

/*
 * 方法 ID
 * 格式：(<参数1><参数2>)<返回值>
 * V -> void
 *
 *  Java                    | JNI
 *  String fun();           | ()Ljava/lang/String;
 *  long f(int i, Class c); | (ILjava/lang/Class;)J
 *  String(byte[] bytes);   | ([B)V
 *
 * env->Call<return value>Method()
 */
extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_accessAccountMethod(JNIEnv *env, jobject thiz, jobject account) {
    jclass cls = env->GetObjectClass(account);
    jmethodID mId = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
    jstring str = static_cast<jstring>(env->CallObjectMethod(account, mId));
    const char *cStr = env->GetStringUTFChars(str, 0);
    LOGD("accessAccountMethod -> %s", cStr);
    env->ReleaseStringUTFChars(str, cStr);
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_accessAccountMethodWithArg(JNIEnv *env, jobject thiz,
                                                            jobject account) {
    jclass cls = env->GetObjectClass(account);
    jmethodID mId = env->GetMethodID(cls, "changeName", "(Ljava/lang/String;)Z");
    jstring name = env->NewStringUTF("new name");
    jboolean result = env->CallBooleanMethod(account, mId, name);
    LOGD("accessAccountMethodWithArg -> %d", result);
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_callbackFromJNI(JNIEnv *env, jobject thiz, jobject callback) {
    jclass cls = env->GetObjectClass(callback);
    jmethodID mId = env->GetMethodID(cls, "onCall", "()V");
    env->CallVoidMethod(callback, mId);
}

static jclass threadClazz;
static jmethodID threadMethod;
static jobject threadObject;


void *threadCallback(void *) {

    JNIEnv *env = nullptr;
    JavaVM *gVM = getJVM();
    if (gVM->AttachCurrentThread(&env, nullptr) == 0) {

        env->CallVoidMethod(threadObject, threadMethod);

        gVM->DetachCurrentThread();
    }
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_callbackFromJNIThread(JNIEnv *env, jobject thiz,
                                                       jobject callback) {
    threadObject = env->NewGlobalRef(callback);
    threadClazz = env->GetObjectClass(callback);
    threadMethod = env->GetMethodID(threadClazz, "onCall", "()V");

    pthread_t handle;
    pthread_create(&handle, nullptr, threadCallback, nullptr);

}
