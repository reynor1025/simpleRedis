// simplec++.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#pragma warning(disable: 4786)
#pragma comment(lib, "ws2_32.lib")  
#include <WINSOCK2.H>   
#include <stdio.h>     
#include <string>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <vector>
#include "server.h"
#include "redisClient.h"
using namespace std;


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

//定义服务器的大小
#define DBNUM  16
#define PORT 9874
#define MSGSIZE  1024
#define MAXCLIENTNUM 10
#define HASHSIZE 10

redisServer* server;
vector<string> split(string str, string ch) {
	vector<string>vec;
	if ("" == str) {
		return vec;
	}
	string strs = str + ch;

	size_t pos = strs.find(ch);
	size_t size = strs.size();
	while (pos != std::string::npos) {
		string x = strs.substr(0, pos);
		vec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(ch);
	}
	return vec;
}
void redisServerRelease(redisServer* SERVER) {
	for (int i = 0; i < SERVER->dbnum; i++) {
		//if(SERVER->db[i]->dict->ht->table[0]==NULL)
		//printf("%d\n",i);

		redisDbRelease(SERVER->db[i]);
	}
	//free(server->db);
	//free(server);
}
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
	free(key);
}

void myValDestructor(char* obj) {
	free(obj);
}

//获取当前线程id
DWORD WINAPI WorkerThread(LPVOID lpParam);
int main() {
	dictType* type = (dictType*)malloc(sizeof(dictType));
	type->hashFunction = myHashFunction;
	type->keyDup = myKeyDup;
	type->valDup = myValDup;
	type->keyCompare = myKeyCompare;
	type->keyDelete = myKeyDestructor;
	type->valDelete = myValDestructor;

	WSADATA wsaData;
	int iaddrSize = sizeof(SOCKADDR_IN);
	DWORD dwThreadId;
	WSAStartup(0x0202, &wsaData);


	//初始化server
	server = (redisServer*)malloc(sizeof(redisServer));

	server->sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	(server->local).sin_family = AF_INET;
	(server->local).sin_port = htons(PORT);
	(server->local).sin_addr.s_addr = htonl(INADDR_ANY);
	bind(server->sListen, (struct sockaddr*)&(server->local), sizeof(SOCKADDR_IN));

	server->dbnum = DBNUM;

	server->db = (redisDb**)malloc(sizeof(redisDb*) * server->dbnum);
	for (int k = 0; k < server->dbnum; k++) {
		server->db[k] = redisDbCreate(type, HASHSIZE, k);
	}
	listen(server->sListen, 1);

	HANDLE hThread[MAXCLIENTNUM];
	for (int i = 0; i < MAXCLIENTNUM; i++) {
		redisClient* client = redisClientCreate();
		client->sClient = accept(server->sListen, (struct sockaddr*)&(client->my_client), &iaddrSize);
		client->db = server->db[0];
		printf("Accepted client:%s:%d\n", inet_ntoa((client->my_client).sin_addr),
			ntohs((client->my_client).sin_port));

		hThread[i] = CreateThread(NULL, 0, WorkerThread, client, 0, NULL);
	}
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	closesocket(server->sListen);
	printf("Redis : Threads are fully executed\n");

	redisServerRelease(server);

	system("pause");
	return 0;
}

DWORD WINAPI WorkerThread(LPVOID lpParam) {
	char szMessage[MSGSIZE];
	int ret;
	redisClient* client = (redisClient*)lpParam;
	client->db->dict;
	while (true) {
		//服务器监听端口获取消息
		ret = recv(client->sClient, szMessage, MSGSIZE, 0);
		//把szMessage变成c字符串
		szMessage[ret] = '\0';
		printf("Redis : Received [%d bytes]: '%s'\n", ret, szMessage);


		string message(szMessage);

		if (message == "-v" || message == "--version") {
			char* str = "create by dz";

			send(client->sClient, str, strlen(str) + sizeof(char), NULL);
			printf("Redis : Send version information success.\n");

		}
		else if (message == "-h" || message == "--help") {
			char* str = "help document not yet perfect";
			send(client->sClient, str, strlen(str) + sizeof(char), NULL);
			printf(" Redis : Send help document success.\n");
		}
		else {

			string ch = " ";
			vector<string>vec = split(message, ch);


			if (vec[0] == "set") {

				if (vec.size() == 3) {

					vector<string>::iterator t;
					t = vec.begin();
					t++;
					string key = *t;
					t++;
					string value = *t;
					char* charkey = (char*)key.data();
					char* charvalue = (char*)value.data();
					//cout<<"DEBUG:"<<key<<" "<<value<<endl;

					bool flag = redisClientInsertDB(client, charkey, charvalue);

					if (flag) {
						char* str1 = "set ok";
						send(client->sClient, str1, strlen(str1) + sizeof(char), NULL);
						printf("Redis : Send set ok\n");
					}
					else {
						char* str2 = "set error";
						send(client->sClient, str2, strlen(str2) + sizeof(char), NULL);
						printf("Redis : Send set error\n");
					}


				}
				else {
					char* str = "Redis ERROR : Wrong number of arguments for 'set' command";
					send(client->sClient, str, strlen(str) + sizeof(char), NULL);
					printf("Redis : Send set error.\n");
				}
			}
			else if (vec[0] == "get") {

				if (vec.size() == 2) {

					vector<string>::iterator t;
					t = vec.begin();
					t++;
					string key = *t;
					char* charkey = (char*)key.data();
					char* getValue = redisClientFetchDB(client, charkey);

					if (getValue == "") {
						char* str = "Redis ERROR : Can't find such key-value pairs";
						send(client->sClient, str, strlen(str) + sizeof(char), NULL);
						printf("Redis : Send get error\n");

					}
					else {
						send(client->sClient, getValue, strlen(getValue) + sizeof(char), NULL);
						printf("Redis : Send get ok\n");
					}

				}
				else {
					char* str = "Redis ERROR : Wrong number of arguments for 'get' command";
					send(client->sClient, str, strlen(str) + sizeof(char), NULL);
					printf("Redis : Send get error.\n");
				}

			}
			else {
				char* str = "Redis ERROR : No such command";
				send(client->sClient, str, strlen(str) + sizeof(char), NULL);
				printf("Redis : Send command error.\n");
			}

		}

	}
	return 0;
}