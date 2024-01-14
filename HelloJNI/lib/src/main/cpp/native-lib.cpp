#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

/*
 * static register
 * <package>_<class>_<method> name
 */
JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from CPP";
    return env->NewStringUTF(hello.c_str());
}