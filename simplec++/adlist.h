#pragma once

typedef struct listNode {
	struct listNode* prev;
	struct listNode* next;
	void* value;
}listNode;
typedef struct list
{
	listNode* head;
	listNode* tail;
	int len;

	void* (*dup)(void* ptr);

	void (*free)(void* ptr);

	int (*match)(void* ptr, void* key);

}list;


list* listCreate(void);

void listRelease(list* l);

list* listAddNodeHead(list* l, void* value);

list* listAddNodeTail(list* l, void* value);

list* listInsertNode(list* l, listNode* old_node, void* value, int after);

list* listDelNode(list* l, listNode* node);

list* listDup(list* orig);

listNode* listSearchKey(list* l, void* key);

listNode* listIndex(list* l, long index);

void listRotate(list* l);
