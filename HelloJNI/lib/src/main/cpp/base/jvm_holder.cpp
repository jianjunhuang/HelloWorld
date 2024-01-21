//
// Created by jianjun huang on 2024/1/21.
//

#include <jni.h>
#include "jvm_holder.h"

static JavaVM *gVM = nullptr;

void setJVM(JavaVM *jvm) {
    gVM = jvm;
}

JavaVM * getJVM() {
    return gVM;
}
