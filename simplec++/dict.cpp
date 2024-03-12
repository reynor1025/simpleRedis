#define _CRT_SECURE_NO_WARNINGS

#include "dict.h"

#define HASHSIZE 10
unsigned int myHashFunction(char* key, int size) {
	char* charkey = (char*)key;
	unsigned int hash = 0;
	for (; *charkey; ++charkey) {
		hash = hash * 33 + *charkey;
	}
	return hash % size;
}

char* myKeyDup(char* key) {
	return key;
}

char* myValDup(char* obj) {
	return obj;
}

int myKeyCompare(char* key1, char* key2) {
	char* charkey1 = (char*)key1;
	char* charkey2 = (char*)key2;
	return strcmp(charkey1, charkey2);
}

void myKeyDestructor(char* key) {
	//free(key);
}

void myValDestructor(char* obj) {
	//free(obj);
}
dict *createDict(dictType* dt, int hashSize)
{
	dict* dic = (dict*)malloc(sizeof(dict));
	if (dic==NULL)
	{
		printf("dict alloc error");
		return NULL;
	}
	if (dt == NULL)
	{
		printf("dictType  is  null");
	}
	dic->ht = (dictht*)malloc(sizeof(dictht));
	dic->type = dt;
	dic->ht->size = hashSize;
	dic->ht->used = 0;
	dic->ht->table = (dictEntry**)malloc(sizeof(dictEntry*) * hashSize);

	for (int i = 0; i < hashSize; i++)
	{
		dic->ht->table[i] = NULL;
	}
	
	return dic;
}

bool insertDict(char* key, char* val, dict* d)
{
	unsigned int index;
	dictEntry* node;

	if (!(node = lookup(d, key))) {
		index = d->type->hashFunction(key, d->ht->size);
		//printf("key : %s\n",key);
		//printf("index :%d\n",index);
		node = (dictEntry*)malloc(sizeof(dictEntry));
		if (node == NULL)return false;
		//node->key=d->type->keyDup(key);
		//strcpy(node->key,key);
		//TODO
		node->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
		strcpy(node->key, key);
		//node->value="";
		node->next = d->ht->table[index];
		d->ht->table[index] = node;
	}

	node->value = (char*)malloc(sizeof(char) * (strlen(val) + 1));
	strcpy(node->value, val);
	return true;
}

dictEntry* lookup(dict* d, char* key)
{

	dictEntry* de;
	unsigned int index;
	index = d->type->hashFunction(key, d->ht->size);
	if (d->ht->table[index] == NULL) {
		printf("d->ht->table[index] is null ");
		return NULL;
	}
	for (de = d->ht->table[index]; de != NULL; de = de->next) {
		if (!(d->type->keyCompare(key, de->key))) {
			return de;
		}
	}
	return nullptr;
}

char* dictFetchValue(dict* d, char* key)
{
	dictEntry* de;
	if (!(de = lookup(d, key))) {
		return "";
	}
	return de->value;
}

void dictDelete(dict* d, char* key)
{
	dictEntry* node;
	dictEntry* temp;

	unsigned int index;
	index = d->type->hashFunction(key, d->ht->size);
	node = d->ht->table[index];

	if (!(d->type->keyCompare(key, node->key))) {
		d->ht->table[index] = node->next;
		d->type->keyDelete(node->key);
		d->type->valDelete(node->value);
		free(node);
		return;
	}
	temp = node;
	node = node->next;
	while (node) {
		if (!(d->type->keyCompare(key, node->key))) {
			temp->next = node->next;
			d->type->keyDelete(node->key);
			d->type->valDelete(node->value);
			free(node);
			return;
		}
		temp = node;
		node = node->next;
	}
	return;

}

void dictRelease(dict* d)
{
	if (d == NULL)return;
	dictEntry* node;
	dictEntry* temp;
	for (int i = 0; i < d->ht->size; i++) {
		node = d->ht->table[i];
		while (node != NULL) {
			//printf("hh\n");
			//char* t=(char*)node->value;
			temp = node;
			node = node->next;
			d->type->keyDelete(temp->key);
			d->type->valDelete(temp->value);
			free(temp);
		}
	}
	free(d->ht);
	/*	free(d->type);*/
	free(d);
}


int main(){
	dictType*type=(dictType*)malloc(sizeof(dictType));
	type->hashFunction=myHashFunction;
	type->keyDup=myKeyDup;
	type->valDup=myValDup;
	type->keyCompare=myKeyCompare;
	type->keyDelete =myKeyDestructor;
	type->valDelete =myValDestructor;
	dict* d= createDict(type,HASHSIZE);

	char*key1="sss";
	char*value1="111";
	bool result= insertDict(key1,value1, d);
	if(result){
		printf("insert1 success\n");
	}else{
		printf("insert1 fail\n");
	}

	char*key2="3sd";
	char*value2="ddd";
	result= insertDict(key2,value2,d);
	if(result){
		printf("insert2 success\n");
	}else{
		printf("insert2 fail\n");
	}

	char*key3="ddds";
	char*value3="1ss";
	result= insertDict(key3,value3, d);
	if(result){
		printf("insert3 success\n");
	}else{
		printf("insert3 fail\n");
	}

	char *value4=(char*)dictFetchValue(d,key3);
	printf("---%s\n",value4);

	dictDelete(d,key3);
	value4=(char*)dictFetchValue(d,key3);
	printf("---%s\n",value4);

	dictRelease(d);
	system("pause");
	return 0;
}

