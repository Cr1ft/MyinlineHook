//
// Created by a craft on 2022/2/16.
//

#ifndef MY_INHK_MYHOOK_H
#define MY_INHK_MYHOOK_H
#include <string>
#include <sys/system_properties.h>
#endif //MY_INHK_MYHOOK_H



#define SLOG_TAG "MYHOOK"
#define SLOG(fmt, args...) __android_log_print(ANDROID_LOG_INFO, SLOG_TAG, fmt, ##args);


#define MY_PAGE_START(addr, page_size)	(~(page_size - 1) & (addr))
#define MY_PAGE_END(addr, page_size)   (((addr) + page_size - 1) & ~(page_size - 1))

typedef char* (*def_strstr)(char*,char*);
typedef std::string (*def_PrettyMethod)(void *t,bool b);
typedef void* (*def_invoke)(void* t,void *a, void *b, void *c, void *d, void *e);
//bool DoCall(ArtMethod* called_method, Thread* self, ShadowFrame& shadow_frame,const Instruction* inst, uint16_t inst_data, JValue* result)
typedef void* (*def_DoCall)(void*,void*,void*,void*,void*,void*);

typedef prop_info* (*old_system_property_find)(const char* name);
typedef int (*old_system_property_get)(const char *name, char *value);
typedef int (*old_system_property_read)(const prop_info *pi, char *name, char *value);

void test();
void hookArtInvoke();
void hookDoCall();
void HookStrStr();
void HookSystemPropty(jobject json,JavaVM *jvm,JNIEnv *env);