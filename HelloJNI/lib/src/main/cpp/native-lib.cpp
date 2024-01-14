#include <jni.h>
#include <string>
#include <logutil.h>

extern "C" JNIEXPORT jstring

/*
 * static register
 * <package>_<class>_<method> name
 * 需要查找
 */
JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from CPP";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_stringInNative(
        JNIEnv *env,
        jobject,
        jstring jstr
) {
    const char *str = env->GetStringUTFChars(jstr, 0);

    int len = env->GetStringUTFLength(jstr);
    LOGD("jstring len=%d", len);
    char buf[128];

    env->GetStringUTFRegion(jstr, 0, len - 1, buf);
    LOGD("jstring region=%s", buf);

    //must, 避免内存泄露
    env->ReleaseStringUTFChars(jstr, str);

    return env->NewStringUTF("hi");
}

void baseType() {
    /*
     * jint
     * jbyte
     * jchar
     * jshort
     * jlong
     * jfloat
     * jdouble
     */
}