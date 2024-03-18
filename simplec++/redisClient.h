#pragma once
#include <WINSOCK2.H>
#include <stdio.h>
#include "server.h"
#include "redisDb.h"

using namespace std;
typedef struct redisClient {
	SOCKET sClient;
	SOCKADDR_IN my_client;
	redisDb* db;
}redisClient;

redisClient* redisClientCreate();

bool redisClientChangeDB(redisServer* server, int dbId, redisClient*& client);

bool redisClientInsertDB(redisClient* client, char* key, char* val);

void redisClientDeleteDB(redisClient* client, char* key);

char* redisClientFetchDB(redisClient* client, char* key);


