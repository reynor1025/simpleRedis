#pragma once
#include <string>
using namespace std;
typedef struct dictEntry {
	char* key;
	char* value;

	struct dictEntry* next;
}dictEntry;

typedef struct dictht {
	dictEntry** table;
	int size;
	int used;
} dictht;

typedef struct dictType {
	//定义hash函数
	unsigned int (*hashFunction)(char* key, int ssize);

	//复制value的函数
	char* (*valDup)(char* val);

	//辅助key的函数
	char* (*keyDup)(char* key);

	//对比key的函数
	int (*keyCompare)(char* key1, char* key2);
	//销毁key的函数
	void (*keyDelete)(char* key);

	//销毁val的函数
	void (*valDelete)(char* val);
}dictType;

typedef struct dict
{
	dictType* type;
	
	//定义hash表
	dictht* ht;
}dict;


//创建dict

dict *createDict(dictType* dt, int hashSize);

//插入
bool insertDict(char* key, char* val, dict* d);

dictEntry* lookup(dict* d, char* key);

char* dictFetchValue(dict* d, char* key);

void dictDelete(dict* d, char* key);

void dictRelease(dict* d);






