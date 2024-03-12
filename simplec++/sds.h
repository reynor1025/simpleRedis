//
// Created by 703 on 2024/3/5.
//

#ifndef SIMPLEREDIS_SDS_H
#define SIMPLEREDIS_SDS_H

/**
 * 实现redis字段
 *
 * 没有按照标准实现内存对齐，留下demo
 * 仅实现其中的一种
 */
//typedef struct __attribute__ ((__packed__)) sdshdr_64
typedef struct sdshdr{
    int len;//已用的长度
    int alloc;//分配的长度
    //char buf[];//字符串数组，使用数组的形式，则需要在后期进行扩容

    char *buff;//使用指针方式
}* sds;

//创建一个新的c语言字符串

sds sdsnew(const char* t);

//创建一个空的字符串
void sdsempty(void);

//为sds分配内存大小
sds sdsnewlen(sds s,int initlen);
//释放内存
void sdsfree(sds s);

//创建sds的副本
sds sdsup(sds s);

//sds拼接c语言字符串
sds sdscat(sds s,const char* t);

//sds拼接sds
sds sdscatsds(sds s1,const sds s2);

//清空sds保存的字符串内容
sds sdsclear(sds s);

//将给定的c字符串复制到sds里面，覆盖原有的字符串
sds sdscpy(sds s,const char *t);

//返回已经使用的长度


//大小写不敏感
sds sdstrim(sds s,const char *t);

//对比两个sds字符串是否相同
int sdscmp(const sds s1,const sds s2);

//分配新的空间,当原始的sds的内存不够的时候
sds sdsMakeRoomFor(sds s, int addlen);

//返回sds已使用空间的字节数:len
static inline int sdslen(const sds sh){
    return sh->len;
}

//返回sds分配的空间的字节数:alloc
static inline int sdsavail(const sds sh){
    return sh->alloc;
}


#endif //SIMPLEREDIS_SDS_H
