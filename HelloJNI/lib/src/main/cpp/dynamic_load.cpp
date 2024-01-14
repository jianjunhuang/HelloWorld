#include <jni.h>
#include <string>
#include <logutil.h>

jstring getString(JNIEnv *env, jobject jobj) {
    return env->NewStringUTF("get string");
}

/*
 * Java 中的函数
 */
static JNINativeMethod gMethods[] = {
        {/*函数名称*/"dynamicStringFromJNI", /*返回值*/"()Ljava/lang/String;", /*函数指针, 要在这个方法前*/
                     (void *) getString}
};

int registerNativeMethod(JNIEnv *env, const char *clazzName, const JNINativeMethod *methods,
                         jint nMethods) {
    jclass jcls;
    jcls = env->FindClass(clazzName);
    if (jcls == nullptr) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(jcls, methods, nMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
/*
 * dynamic register
 *
 * callback when System.loadLibrary()
 */
JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) < 0) {
        return JNI_FALSE;
    }
    registerNativeMethod(env, "xyz/juncat/jni/lib/HelloJNI", gMethods, /*方法数量*/1);
    LOGD("jni on load!");
    return JNI_VERSION_1_6;
}
