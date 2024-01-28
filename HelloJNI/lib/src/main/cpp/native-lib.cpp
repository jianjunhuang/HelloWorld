#include <jni.h>
#include <string>
#include <logutil.h>
#include <android/bitmap.h>

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


extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_localReferenceInJNI(JNIEnv *env, jobject thiz) {
    /*
     * 局部引用，结束后会自动释放，但是也有上限，最好尽快释放
     */
    for (int i = 0; i < 10000; ++i) {
        jclass cls = env->FindClass("java/lang/String");
        jmethodID mId = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");
        jstring str = env->NewStringUTF("index");
        env->NewObject(cls, mId, str);
        env->DeleteLocalRef(cls);
        //env->DeleteLocalRef(jmethodID);
        env->DeleteLocalRef(str);
    }

}
extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_globalReferenceInJNI(JNIEnv *env, jobject thiz) {
    /*
     * 全局引用,可以用做缓存
     */
    static jclass strClass = nullptr;
    if (strClass == nullptr) {
        jclass cls = env->FindClass("java/lang/String");
        strClass = static_cast<jclass>(env->NewGlobalRef(cls));
        LOGD("global reference new");
        env->DeleteLocalRef(cls);
    } else {
        LOGD("global reference using cache");
    }

}
extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_weakGlobalReferenceInJNI(JNIEnv *env, jobject thiz) {
    /*
    * 弱引用, same as Java
    */
    static jclass strClass = nullptr;
    if (strClass == nullptr) {
        jclass cls = env->FindClass("java/lang/String");
        strClass = static_cast<jclass>(env->NewWeakGlobalRef(cls));
        LOGD("weak global reference new");
        env->DeleteLocalRef(cls);
    } else {
        LOGD("weak global reference using cache");
    }

    //check gc
    jboolean isGC = env->IsSameObject(strClass, nullptr);


}


jobject generateBitmap(JNIEnv *env, uint32_t width, uint32_t height) {

    jclass bitmapCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls,
                                                            "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(
            bitmapConfigClass, "valueOf",
            "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
                                                       valueOfBitmapConfigFunction, configName);

    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls,
                                                    createBitmapFunction,
                                                    width,
                                                    height,
                                                    bitmapConfig);

    return newBitmap;
}


extern "C"
JNIEXPORT jobject JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_bitmapInJNI(JNIEnv *env, jobject thiz, jobject bmp) {
    LOGD("bitmap in jni");
    AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(env, bmp, &bitmapInfo);
    LOGD("width: %d", bitmapInfo.width);
    LOGD("height: %d", bitmapInfo.height);
    void *bitmapPixels;
    AndroidBitmap_lockPixels(env, bmp, &bitmapPixels);

    uint32_t width = bitmapInfo.width;
    uint32_t height = bitmapInfo.height;
    uint32_t *newBitmapPixels = new uint32_t[width * height];
    int whereToGet = 0;

    //左右翻转
    for (int y = 0; y < height; ++y) {
        for (int x = width - 1; x >= 0; x--) {
            uint32_t pixel = ((uint32_t *) bitmapPixels)[whereToGet++];
            newBitmapPixels[width * y + x] = pixel;
        }
    }

    AndroidBitmap_unlockPixels(env, bmp);

    jobject newBitmap = generateBitmap(env, width, height);

    void *resultBitmapPixels;
    AndroidBitmap_lockPixels(env, newBitmap, &resultBitmapPixels);
    memcpy((uint32_t *) resultBitmapPixels, newBitmapPixels, sizeof(uint32_t) * width * height);
    AndroidBitmap_unlockPixels(env, newBitmap);
    delete[]newBitmapPixels;
    return newBitmap;
}

