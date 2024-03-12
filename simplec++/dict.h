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
	//����hash����
	unsigned int (*hashFunction)(char* key, int ssize);

	//����value�ĺ���
	char* (*valDup)(char* val);

	//����key�ĺ���
	char* (*keyDup)(char* key);

	//�Ա�key�ĺ���
	int (*keyCompare)(char* key1, char* key2);
	//����key�ĺ���
	void (*keyDelete)(char* key);

	//����val�ĺ���
	void (*valDelete)(char* val);
}dictType;

typedef struct dict
{
	dictType* type;
	
	//����hash��
	dictht* ht;
}dict;


//����dict

dict *createDict(dictType* dt, int hashSize);

//����
bool insertDict(char* key, char* val, dict* d);

dictEntry* lookup(dict* d, char* key);

char* dictFetchValue(dict* d, char* key);

void dictDelete(dict* d, char* key);

void dictRelease(dict* d);






