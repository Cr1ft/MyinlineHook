//
// Created by a craft on 2022/2/16.
//

#include "mhk.h"
#include <fcntl.h>
#include <dlfcn.h>
#include <sys/system_properties.h>
#include "iModel.h"
#include "myutil.h"
#include "MyHook.h"
#include <jni.h>


void *artaddress = getModuleAddr("libart.so");
void *libc = getModuleAddr("libc.so");
void *strd = getAddrbySym(libc,"strstr");
void *invoke = getAddrbySym(artaddress,"_ZN3art9ArtMethod6InvokeEPNS_6ThreadEPjjPNS_6JValueEPKc");
void *PrettyMethod = getAddrbySym(artaddress,"_ZN3art9ArtMethod12PrettyMethodEb");
void *DoCall = getAddrbySym(artaddress,
                            "_ZN3art11interpreter6DoCallILb0ELb0EEEbPNS_9ArtMethodEPNS_6ThreadERNS_11ShadowFrameEPKNS_11InstructionEtPNS_6JValueE");
//void *libc;
//void *strd;
//void *invoke;
//void *PrettyMethod;
//void *DoCall;
JavaVM *global_jvm;
jobject global_json;


const char* global_name;
prop_info* gloabl_pi;
JNIEnv *global_env;


void test(){
    int a = 1+2;
    char* (*ori_strstr)(const char* h, const char* n);
    ori_strstr = strstr;
    void *asd = getAddrbySym(libc,"strstr");
    void *asdb = getAddrbySym(artaddress,"_ZN3art9ArtMethod6InvokeEPNS_6ThreadEPjjPNS_6JValueEPKc");
    __android_log_print(6,"TTTT","YYYYY:%p",(void*)asd);
    __android_log_print(6,"TTTT","YYYYY invoke:%p",(void*)asdb);
    __android_log_print(6,"TTTT","YYYYY:%p",ori_strstr);
}

JNIEnv *get_env(int *attach) {
    if (global_jvm == NULL) return NULL;

    *attach = 0;
    JNIEnv *jni_env = NULL;

    int status = global_jvm->GetEnv((void **)&jni_env, JNI_VERSION_1_6);

    if (status == JNI_EDETACHED || jni_env == NULL) {
        status = global_jvm->AttachCurrentThread(&jni_env, NULL);
        if (status < 0) {
            jni_env = NULL;
        } else {
            *attach = 1;
        }
    }
    return jni_env;
}

void del_env() {
    if (global_jvm != NULL) {
        global_jvm->DetachCurrentThread();
    }
}

const char* get_value(char* key) {
    int attach = 0;
    JNIEnv* env = get_env(&attach);
    jobject json = global_json;
    if(env == NULL || json == NULL) {
        return "";
    }

    // 获取JSONObject class
    // __android_log_print(3, "===========》", "获取JSONObject class");
    jclass JSONObjectClass = env->GetObjectClass(json);

    // 获取JSONObject get方法
    // __android_log_print(3, "===========》", "获取JSONObject get方法");
    jmethodID getID = env->GetMethodID(JSONObjectClass, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
    if (getID == NULL) return "";

    // 获取value
    // __android_log_print(3, "===========》", "获取value");
    jstring jkey = env->NewStringUTF(key);
    jstring value = (jstring)env->CallObjectMethod(json, getID, jkey);
    if (value == NULL) return "";

    // 把value转换为C string
    // __android_log_print(3, "===========》", "把value转换为C string");
    const char *str = env->GetStringUTFChars(value, NULL);
    if (str == NULL) return "";

    // 释放jstring value
    // __android_log_print(3, "===========》", "释放jstring value");
    env->ReleaseStringUTFChars(value, str);

    if (attach == 1) {
        del_env();
    }
    return str;
}

void match_name(const char *name, char *value) {
    // 暂时屏蔽，避免SDK变化导致crash
    // if(strcmp(name, "ro.build.version.sdk") == 0) {
    //     strcpy(value, (char*)get_value("sdk"));
    // } else
    if(strcmp(name, "ro.build.version.release") == 0) {
        strcpy(value, (char*)get_value("release"));
    } else if(strcmp(name, "ro.build.version.incremental") == 0) {
        strcpy(value, (char*)get_value("incremental"));
    } else if(strcmp(name, "ro.build.user") == 0) {
        strcpy(value, (char*)get_value("user"));
    } else if(strcmp(name, "ro.build.host") == 0) {
        strcpy(value, (char*)get_value("host"));
    } else if(strcmp(name, "ro.build.fingerprint") == 0) {
        strcpy(value, (char*)get_value("fingerprint"));
    } else if(strcmp(name, "ro.build.id") == 0) {
        strcpy(value, (char*)get_value("buildID"));
    } else if(strcmp(name, "ro.build.type") == 0) {
        strcpy(value, (char*)get_value("type"));
    } else if(strcmp(name, "ro.build.tags") == 0) {
        strcpy(value, (char*)get_value("tags"));
    } else if(strcmp(name, "ro.serialno") == 0) {
        strcpy(value, (char*)get_value("serial"));
    } else if(strcmp(name, "ro.hardware") == 0) {
        strcpy(value, (char*)get_value("hardware"));
    } else if(strcmp(name, "ro.product.model") == 0) {
        strcpy(value, (char*)get_value("model"));
    } else if(strcmp(name, "ro.product.brand") == 0) {
        strcpy(value, (char*)get_value("brand"));
    } else if(strcmp(name, "ro.product.board") == 0) {
        strcpy(value, (char*)get_value("board"));
    } else if(strcmp(name, "ro.product.name") == 0) {
        strcpy(value, (char*)get_value("product"));
    } else if(strcmp(name, "ro.product.device") == 0) {
        strcpy(value, (char*)get_value("device"));
    } else if(strcmp(name, "ro.product.manufacturer") == 0) {
        strcpy(value, (char*)get_value("manufacturer"));
    } else if(strcmp(name, "ro.build.display.id") == 0) {
        strcpy(value, (char*)get_value("display"));
    } else if(strcmp(name, "ro.build.date.utc") == 0) {
        strcpy(value, (char*)get_value("time"));
    } else if(strcmp(name,"ro.build.description") ==0 ){
        strcpy(value,(char*)get_value("fingerprint"));
    }else if(strcmp(name,"ro.build.version.ota") ==0){
        strcpy(value,(char*)get_value("fingerprint"));
    }
}


char* my_strstr(char* a,char *b){
    char *haystack = a;
    char *needle = b;
    auto oirstrstr = (def_strstr)getOriFunByHkFun((void *)my_strstr);
    if(oirstrstr(haystack,"frida") != nullptr || oirstrstr(needle,"frida") != nullptr ||
       oirstrstr(haystack,"gum-js-loop") != nullptr || oirstrstr(needle,"gum-js-loop") != nullptr||
       oirstrstr(haystack,"gmain") != nullptr || oirstrstr(needle,"gmain") != NULL ||
       oirstrstr(haystack,"linjector") != nullptr || oirstrstr(needle,"linjector") != nullptr
            ){
        return nullptr;
    } else{
        return (char*)oirstrstr(haystack,needle);
    }


}

void* my_invoke(void* t,void *a, void *b, void *c, void *d, void *e){
    auto pm = (def_PrettyMethod)PrettyMethod;
    auto oirinvoke = (def_invoke)getOriFunByHkFun((void*)my_invoke);
    int f = 1;
    std::string mmm = pm(t,true);
    if(!(strstr(mmm.c_str()," java.")!=NULL ||strstr(mmm.c_str()," android.") !=NULL|| strstr(mmm.c_str()," dalvik.")!=NULL
         ||strstr(mmm.c_str()," androidx.")!=NULL
    )){
        int *tid = (int*)((char*)a+0x10);
        LEE("[Invoke]:tid:%d %s",*tid,mmm.c_str());
    }

    void* ret = oirinvoke(t,a,b,c,d,e);
    return ret;
}

void* my_DoCall(void* method,void *thead, void *b, void *c, void *d, void *e){
    auto pm = (def_PrettyMethod)PrettyMethod;
    auto OriDoCall = (def_DoCall)getOriFunByHkFun((void*)my_DoCall);
    int f = 1;
    std::string mmm = pm(method,f);
    if(!(strstr(mmm.c_str()," java.")!=NULL ||strstr(mmm.c_str()," android.") !=NULL|| strstr(mmm.c_str()," dalvik.")!=NULL
         ||strstr(mmm.c_str()," androidx.")!=NULL
    )){
        int *tid = (int*)((char*)thead+0x10);
        LEE("[DoCall]:tid:%d %s",*tid,mmm.c_str());
    }


    void* ret = OriDoCall(method,thead,b,c,d,e);
    return ret;

}

int my_spg(const char *name, char *value){
    auto Ori_system_property_get = (old_system_property_get)getOriFunByHkFun((void*)my_spg);
    int ret = Ori_system_property_get(name,value);
    match_name(name, value);
    SLOG("__system_property_get %s %s",name,value);
    return ret;
}

int my_spr(prop_info *pi, char *name, char *value){
    auto Ori_system_property_read = (old_system_property_read)getOriFunByHkFun((void*)my_spr);
    int ret = Ori_system_property_read(pi,name,value);
    prop_info* p = pi;

    match_name((const char*)((char*)p+0x60), value);
    SLOG("__system_property_read %s %s",((char*)p+0x60),value);
    return ret;
}

prop_info* my_spf(const char *name){
    auto Ori_system_property_find= (old_system_property_find)getOriFunByHkFun((void*)my_spf);
    gloabl_pi = Ori_system_property_find(name);
    return gloabl_pi;
}

void HookStrStr(){
    const RetInfo &rep = dump_replace(strd,(void*)my_strstr,NULL, NULL,NULL);
    if (rep.status != success) {
        LE("hook docall error=%d", rep.status);
    }
}

void hookDoCall(){
    if (artaddress != NULL) {
        if(DoCall != NULL){
            const RetInfo &rep = dump_replace(DoCall, (void *) my_DoCall, NULL, NULL, NULL);
            if (rep.status != success) {
                LE("hook docall error=%d1", rep.status);
            }
        }
    }
}

void hookArtInvoke(){
    if (artaddress != NULL) {
        if(invoke != NULL){
            const RetInfo &rep = dump_replace(invoke, (void *) my_invoke, NULL, NULL, NULL);
            if (rep.status != success) {
                LE("hook dlopen error=%d", rep.status);
            }
        }
    }
}

void HookSystemPropty(jobject json,JavaVM *jvm,JNIEnv *env){
    global_json = json;
    global_jvm = jvm;
    global_env = env;
//    const RetInfo &rep  = dump_replace((void*)__system_property_get,(void*)my_spg,NULL, NULL,NULL);
    const RetInfo &rep1 = dump_replace((void*)__system_property_read,(void*)my_spr,NULL, NULL,NULL);
//    const RetInfo &rep2 = dump_replace((void*)__system_property_find,(void*)my_spf,NULL, NULL,NULL);
//    if (rep.status != success) {
//        LE("hook (void*)__system_property_get error=%d", rep.status);
//    }
    if (rep1.status != success) {
        LE("hook (void*)__system_property_read error=%d", rep1.status);
    }
//    if (rep2.status != success) {
//        LE("hook (void*)__system_property_find error=%d", rep.status);
//    }
}

