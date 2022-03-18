//
// Created by EDZ on 2019/12/18.
//

//#include "../include/mhk.h"

#include <jni.h>
#include <android/log.h>
#include "MyHook.h"




JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){



    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zhuotong_myihk_MainActivity_hookArtInvoke(JNIEnv *env, jobject thiz) {
    // TODO: implement hookArtInvoke()
    hookArtInvoke();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_zhuotong_myihk_MainActivity_JavaCallJni(JNIEnv *env, jobject thiz, jint i) {
    __android_log_print(4, "javajnitrace", "step2->JavaCallJni onEnter %0x", i);
    jclass class_MainActivity = env->FindClass("com/zhuotong/myihk/MainActivity");
    jmethodID  method_JniCallJni = env->GetMethodID(class_MainActivity, "JniCallJni", "(I)I");
    char *s ="i am aaaaaaaa";
    char *h = "aaa";
    s = strstr(s, h);
    if(s!= nullptr){
        SLOG("%s",s);
    } else{
        SLOG("Hook success")
    }


    int callResult = env->CallIntMethod(thiz, method_JniCallJni, 0x2222);
    return  callResult + 1;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_zhuotong_myihk_MainActivity_JniCallJni(JNIEnv *env, jobject thiz, jint i) {
    __android_log_print(4, "javajnitrace", "step3->JniCallJni onEnter %0x",i);
    jclass class_MainActivity = env->FindClass("com/zhuotong/myihk/MainActivity");
    jmethodID  method_JniCallJava = env->GetMethodID(class_MainActivity, "JniCallJava", "(I)I");
    int callResult = env->CallIntMethod(thiz, method_JniCallJava, 0x3333);
    return  callResult + 1;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_craft_nativeJNI_NativeHelper_startNewPhone(JNIEnv *env, jclass c, jobject json) {
    // TODO: implement startNewPhone()

    JavaVM* jvm;
    env->GetJavaVM(&jvm);
    jobject infoJson = env->NewGlobalRef(json);
    HookSystemPropty(infoJson,jvm,env);
    char sn[256];
    __system_property_get("ro.serialno", sn);


}
extern "C"
JNIEXPORT void JNICALL
Java_com_craft_nativeJNI_NativeHelper_startMethodTrace(JNIEnv *env, jclass clazz) {
    // TODO: implement startMethodTrace()
//    test();
    hookArtInvoke();
    hookDoCall();
    HookStrStr();

}