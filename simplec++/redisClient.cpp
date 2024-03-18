#include "redisClient.h"

redisClient* redisClientCreate()
{
    redisClient* client = (redisClient*)malloc(sizeof(redisClient));

    return client;
}

bool redisClientChangeDB(redisServer* server, int dbId, redisClient*& client)
{
    if (dbId > server->dbnum || dbId < 0)
    {
        printf("this is  a bad idea");
        return false;
    }
    client->db = server->db[dbId];

    return true;
}

bool redisClientInsertDB(redisClient* client, char* key, char* val)
{
    return redisDbInsert(client->db, key, val);
}

void redisClientDeleteDB(redisClient* client, char* key)
{
    redisDbDelete(client->db, key);
}

char* redisClientFetchDB(redisClient* client, char* key)
{
    return redisDbFetchValue(client->db,key);
}


