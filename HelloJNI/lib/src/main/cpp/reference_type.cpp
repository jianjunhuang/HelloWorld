//
// Created by jianjun huang on 2024/1/15.
//
#include <jni.h>
#include <string>
#include <logutil.h>

/*
 * Java Reference          | Native
 * ----------------------------------------
 * object                  | jobject
 * java.lang.Class         | jclass
 * java.lang.String        | jstring
 * Object[]                | jobjectArray
 * boolean[]               | jbooleanArray
 * byte[]                  | jbyteArray
 * java.lang.Throwable     | jthrowable
 * char[]                  | jcharArray
 * short[]                 | jshortArray
 * int[]                   | jintArray
 * long[]                  | jlongArray
 * float[]                 | jfloatArray
 * double[]                | jdoubleArray
 */
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_referenceInJNI(
        JNIEnv *env,
        jobject,
        jobjectArray str_array
) {
    int len = env->GetArrayLength(str_array);
    LOGD("len is %d", len);
    for (int i = 0; i < len; ++i) {
        jstring str = static_cast<jstring>(env->GetObjectArrayElement(str_array, i));
        const char *cStr = env->GetStringUTFChars(str, 0);
        LOGD("index: %d, str: %s", i, cStr);
        env->ReleaseStringUTFChars(str, cStr);
    }
    jclass jcls = env->FindClass("java/lang/String");
    return env->NewObjectArray(2, jcls, /*init*/NULL);
}