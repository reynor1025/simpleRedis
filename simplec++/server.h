#pragma once
#include <WINSOCK2.H>
#include "redisDb.h"

typedef struct redisServer{
	SOCKET sListen;//¼àÌý
	SOCKADDR_IN local;

	int port;

	redisDb**db;

	int dbnum;

}redisServer;


//void redisServerRelease(redisServer*SERVER){
//	for(int i=0;i<SERVER->dbnum;i++){
//		//if(SERVER->db[i]->dict->ht->table[0]==NULL)
//		//printf("%d\n",i);
//		
//		redisDbRelease(SERVER->db[i]);
//	}
//	//free(server->db);
//	//free(server);
//}
