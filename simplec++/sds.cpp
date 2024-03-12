//
// Created by 703 on 2024/3/5.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sds.h"

//创建一个包含给定c字符串的sds
sds sdsnew(const char *t){
   /* sds sh=(sds)malloc(sizeof(struct sdshdr));
    sh->len=strlen(t);
    sh->alloc=strlen(t)+1;
    sh->buff=(char*)malloc(sizeof(char)*(sh->len+1));
    memcpy(sh->buff, t, sh->len);
    (sh->buff)[sh->len+1]='\0';*/
    return sdsnewlen(t,strlen(t));
}

sds sdsnewlen(const char* t, int init) {
    sds sh = (sds)malloc(sizeof(struct sdshdr));
    sh->len = init;
    sh->alloc = init;
    sh->buff = (char*)malloc(sizeof(char) * (init + 1));
    memcpy(sh->buff, t, sh->len);
    (sh->buff)[init] = '\0';
    return sh;
}

sds sdsempty(void)
{
    return sdsnewlen("", 0);
}

/*
 * 1 表示大于
 * 0 表示等于
 * -1 表示小于
 */
int sdscmp(const sds s1,const sds s2){
    int s1len,s2len,minlen;

    s1len  = s1->len;
    s2len  = s2->len;

    minlen = s1len < s2len? s1len: s2len;
    int cmp;
    cmp = memcmp(s1->buff,s2->buff,minlen);
    if(cmp==0)
        return s1len > s2len? 1:(s1len<s2len?-1:0);
    return cmp;
}


sds sdsnewlen(sds s,int initlen);

void sdsfree(sds s);

sds sdsup(sds s);