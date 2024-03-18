#include "redisDb.h"

redisDb* redisDbCreate(dictType* type, int hashSize, int id)
{
    redisDb* db = (redisDb*)malloc(sizeof(redisDb));
    db->dict = createDict(type,hashSize);
    db->id = id;
    return db;
}

redisDb* redisDbCreate(dictType* type, int hashSize)
{
    redisDb* db = (redisDb*)malloc(sizeof(redisDb));
    db->dict = createDict(type, hashSize);
    db->id = NULL;
    return db;
}

bool redisDbInsert(redisDb* db, char* key, char* val)
{
    bool sign = insertDict(key,val,db->dict);
    if (sign) {
        printf("Redis : redisDb insert/refresh success.\n");
    }
    else {
        printf("Redis : redisDb insert/refresh fail.\n");
    }
    return sign;
}

void redisDbDelete(redisDb* db, char* key)
{
    dictDelete(db->dict, key);
}

char* redisDbFetchValue(redisDb* db, char* key)
{
    char * res = dictFetchValue(db->dict, key);
    if (res == "") {
        printf("Redis WARNING: redisDb fetch NULL.\n");
    }
    else {
        printf("Redis : redisDb fetch success.\n");
    }
    return res;
}

void redisDbRelease(redisDb* db)
{
    if (db==NULL)
    {
        return;
    }
    dictRelease(db->dict);
    free(db);
}








