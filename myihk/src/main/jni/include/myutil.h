//
// Created by test on 2021/12/28.
//

#ifndef MY_INHK_MYUTIL_H
#define MY_INHK_MYUTIL_H

#include <elf.h>

#ifdef __LP64__
#define Elf_phdr Elf64_Phdr
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Addr  Elf64_Addr
#define Elf_Sym   Elf64_Sym
#define Elf_Xword  Elf64_Xword
#define Elf_Dyn  Elf64_Dyn
#else
#define Elf_phdr Elf32_Phdr
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Addr  Elf32_Addr
#define Elf_Sym   Elf32_Sym
#define Elf_Xword  Elf32_Sword
#define Elf_Dyn  Elf32_Dyn
#endif


double retDou(double a, double b);
float retFlo(float a, float b);

void *getModuleAddr(char *mod){
    char line[1024];
    int *start = nullptr;
    int *end = nullptr;
    int n=1;
    FILE *fp=fopen("/proc/self/maps","r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, mod) ) {
            __android_log_print(6,"craft","");
            if(n==1){
                start = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            else{
                strtok(line, "-");
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            n++;
        }
    }

    return (void*)start;
}


char * getAddrbySym(void *startr, char* symname){
    Elf_phdr cc;
    Elf_Ehdr header;
    Elf_Addr phof = NULL;
    char *strtab_;
    Elf_Sym *symtab_;
    Elf_Xword strsz;

    memcpy(&header, startr, sizeof(Elf_Ehdr));
//    __android_log_print(6,"TTTT","YYYYYPHOF1:%x",header.e_phoff);
    memcpy(&cc, ((char *) (startr) + header.e_phoff), sizeof(Elf_phdr));
    for (int y = 0; y < header.e_phnum; y++) {//寻找首段偏移
        memcpy(&cc, (char *) (startr) + header.e_phoff + sizeof(Elf_phdr) * y,
               sizeof(Elf_phdr));
        if (cc.p_type == 1) {
            //获取物理地址
            phof =cc.p_paddr;
            __android_log_print(6,"TTTT","YYYYYPHOF:%p",phof);
            break;
        }

    }
    for (int y = 0; y < header.e_phnum; y++) {
        memcpy(&cc, (char *) (startr) + header.e_phoff + sizeof(Elf_phdr) * y,
               sizeof(Elf_phdr));
        if (cc.p_type == 2) { // Dynamic Segment
            Elf_Dyn dd;
            for (y = 0; y == 0 || dd.d_tag != 0; y++) {
                __android_log_print(6,"TTTT","YYYYYcc.p_offset:%p elf_dyn:%lx",cc.p_vaddr,sizeof(Elf_Dyn));
                memcpy(&dd, (char *) (startr) + cc.p_vaddr + y * sizeof(Elf_Dyn),
                       sizeof(Elf_Dyn));

                if (dd.d_tag == 5) {//DT_STRTAB
                    //根据符号表解释 dptr指向了strtab的地址，此处需要
                    strtab_ = reinterpret_cast< char *>((char *) startr + dd.d_un.d_ptr - phof);
                }
                if (dd.d_tag == 6) {//DT_SYMTAB
                    __android_log_print(6,"TTTT","YYYYDT_SYMTAB:%p",dd.d_un.d_ptr);
                    symtab_ = reinterpret_cast<Elf_Sym *>((
                            (char *) startr + dd.d_un.d_ptr - phof));
                }
                if (dd.d_tag == 10) {//字符串表大小
                    strsz = dd.d_un.d_val;
                }


            }
        }
    }

    char strtab[strsz];
    memcpy(&strtab, strtab_, strsz);
    Elf_Sym mytmpsym;
    for (int n = 0; n < (long) strtab_ - (long) symtab_; n = n + sizeof(Elf_Sym)) {//遍历符号表
        memcpy(&mytmpsym,(char*)symtab_+n,sizeof(Elf_Sym));
        if(strstr(strtab_+mytmpsym.st_name,symname))
        {    __android_log_print(6,"r0ysue","%p %s",mytmpsym.st_value,strtab_+mytmpsym.st_name);
            break;
        }
    }
    __android_log_print(6,"TTTT","YYYYY %s mytmpsym:%lx",symname,mytmpsym.st_value);
    return (char*)startr+mytmpsym.st_value-phof;//
}

/**判断str1是否以str2开头
 * 如果是返回1
 * 不是返回0
 * 出错返回-1
 * */
int is_begin_with(const char * str1,char *str2)
{
    if(str1 == NULL || str2 == NULL)
        return -1;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if((len1 < len2) || (len1 == 0 || len2 == 0))
        return -1;
    char *p = str2;
    int i = 0;
    while(*p != '\0')
    {
        if(*p != str1[i])
            return 0;
        p++;
        i++;
    }
    return 1;
}

/**判断str1是否以str2结尾
 * 如果是返回1
 * 不是返回0
 * 出错返回-1
 * */
int is_end_with(const char *str1, char *str2)
{
    if(str1 == NULL || str2 == NULL)
        return -1;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if((len1 < len2) || (len1 == 0 || len2 == 0))
        return -1;
    while(len2 >= 1)
    {
        if(str2[len2 - 1] != str1[len1 - 1])
            return 0;
        len2--;
        len1--;
    }
    return 1;
}

#endif //MY_INHK_MYUTIL_H
