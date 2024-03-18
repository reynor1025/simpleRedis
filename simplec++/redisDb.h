#pragma once
#include"dict.h"
typedef struct redisDb{
	int id;
	dict* dict;

}redisDb;

redisDb* redisDbCreate(dictType* type, int hashSize, int id);

redisDb* redisDbCreate(dictType* type, int hashSize);

bool redisDbInsert(redisDb* db, char* key, char* val);

void redisDbDelete(redisDb* db, char* key);

char* redisDbFetchValue(redisDb* db, char* key);

void redisDbRelease(redisDb* db);