#define _CRT_SECURE_NO_WARNINGS
#include "adlist.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>


list* listCreate(void)
{
    list* ls = (list*) malloc(sizeof(list));
    ls->head = NULL;
    ls->tail = NULL;
    ls->len = 0;

    ls->dup = NULL;
    ls->free = NULL;
    ls->match = NULL;

    return ls;
}

void listRelease(list* l)
{
    if (l == NULL) {
        return;
    }

    if (l->head == NULL) {
        return;
    }

    listNode* temp = l->head;
    while (temp->next != NULL) {
        temp = temp->next;

        if (l->free != NULL) {
            l->free(temp->value);
        }
        else {
            printf("Redis WARNING : List->free is not define.\n");
        }
        free(temp->prev);
    }
    free(temp);
    l->head = NULL;
    l->tail = NULL;
    free(l);
    l = NULL;
    return;
}
#define listSetDupMethod(l,m) ((l)->dup = (m))
list* listAddNodeHead(list* l, void* value)
{
    if (l==NULL)
    {
        printf("list->free is NUll,can not addNodeHead");
    }

    if (l->head == NULL) {
        l->head = (listNode*)malloc(sizeof(listNode));
        l->head->next = NULL;
        l->head->prev = NULL;
        if (l->dup != NULL) {
            l->head->value = l->dup(value);
        }
        else
        {
            printf("simple Redis WARNING : List->dup is not define.\n");
            l->head->value = value;
        }
        l->len = 1;
        l->tail = l->head;
        return l;

    }
    else
    {
        listNode *temp = (listNode*)malloc(sizeof(listNode));
        if (l->dup != NULL) {
            temp->value = l->dup(l->head);
        }
        else
        {
            printf("simple Redis WARNING : List->dup is not define.\n");
            temp->value = value;
        }
        
        temp->next = l->head;
        l->head->prev = temp;
        l->head = temp;
        temp->prev = NULL;
        
        l->len++;
        return l;
        
    }
}

list* listAddNodeTail(list* l, void* value)
{
    if (l == NULL)
    {
        printf("list->free is NUll,can not addNodeHead");
    }

    if (l->head == NULL) {
        l->head = (listNode*)malloc(sizeof(listNode));
        l->head->next = NULL;
        l->head->prev = NULL;
        if (l->dup != NULL) {
            l->head->value = l->dup(value);
        }
        else
        {
            printf("simple Redis WARNING : List->dup is not define.\n");
            l->head->value = value;
        }
        l->len = 1;
        l->tail = l->head;
        return l;

    }
    else
    {
        listNode* temp = (listNode*)malloc(sizeof(listNode));
        if (l->dup != NULL) {
            temp->value = l->dup(l->head);
        }
        else
        {
            printf("simple Redis WARNING : List->dup is not define.\n");
            temp->value = value;
        }

        temp->next = NULL;
        temp->prev = l->tail;
        l->tail->next = temp;
        l->tail = temp;

        l->len++;
        return l;

    }
}

list* listInsertNode(list* l, listNode* old_node, void* value, int after)
{
    listNode* newone = (listNode*)malloc(sizeof(listNode));
    //newone->value=value;
    if (l->dup != NULL) {
        newone->value = l->dup(value);
    }
    else {
        printf("Redis WARNING : List->dup is not define.\n");
        newone->value = value;
    }
    l->len++;
    if (after) {
        newone->next = old_node->next;
        newone->prev = old_node;
        old_node->next->prev = newone;
        old_node->next = newone;
        if (l->tail == old_node) {
            l->tail = newone;
        }
        return l;
    }
    else {
        newone->next = old_node;
        newone->prev = old_node->prev;
        old_node->prev->next = newone;
        old_node->prev = newone;
        if (l->head == old_node) {
            l->head = newone;
        }
        return l;
    }
}

list* listDelNode(list* l, listNode* node)
{
    if (l->free != NULL) {
        l->free(node->value);
    }
    else {
        printf("Redis WARNING : List->free is not define.\n");
    }
    if (l->head == node && l->tail == node) {
        free(node);
        l->head = NULL;
        l->tail = NULL;
        l->len--;
        return l;
    }
    else if (l->head == node) {
        printf("head\n");
        l->head = node->next;
        l->head->prev = NULL;
        free(node); 
        l->len--;
        return l;
    }
    else if (l->tail == node) {
        l->tail = node->prev;
        l->tail->next = NULL;
        free(node);
        l->len--;
        return l;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    l->len--;
    return l;
}

list* listDup(list* orig)
{
    if (orig == NULL) {
        return NULL;
    }
    if (orig->head == NULL) {
        list* l = listCreate();
        return l;
    }
    else {
        list* l = listCreate();
        listNode* temp = orig->head;
        while (temp != NULL) {
            l = listAddNodeTail(l, temp->value);
            temp = temp->next;
        }
        return l;
    }
}

listNode* listSearchKey(list* l, void* key)
{
    if (l == NULL) {
        printf("Redis ERROR : List NULL.\n");
        return NULL;
    }
    else if (l->head == NULL) {
        printf("Redis ERROR : List does't have nodes.\n");
        return NULL;
    }
    else {
        listNode* temp = l->head;

        if (l->match == NULL) {
            printf("Redis ERROR : List->match is not define.\n");
            return NULL;
        }

        while (temp != NULL && !(l->match(key, temp->value))) {
            temp = temp->next;
        }
        if (temp == NULL) {
            printf("Redis ERROR : List doesn't have this node.\n");
            return NULL;
        }
        else {
            return temp;
        }
    }
}

listNode* listIndex(list* l, long index)
{
    if (l == NULL) {
        printf("Redis ERROR : List NULL.\n");
        return NULL;
    }
    else if (l->head == NULL) {
        printf("Redis ERROR : List doesn't have node.\n");
        return NULL;
    }
    listNode* temp = l->head;
    for (int i = 0; i < index && temp != NULL; i++) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Redis ERROR : Subscript out of range.\n");
        return NULL;
    }
    return temp;
}

void listRotate(list* l)
{
    if (l == NULL) {
        printf("Redis ERROR : List NULL.\n");
        return;
    }
    else if (l->head == NULL) {
        printf("Redis ERROR : List doesn't have node.\n");
        return;
    }
    else if (l->head == l->tail) {
        printf("Redis ERROR : List only have one node.\n");
        return;
    }

    listNode* node = l->tail->prev;
    l->tail->prev->next = NULL;
    l->tail->next = l->head;
    l->head->prev = l->tail;
    l->head = l->tail;
    l->tail = node;
    l->head->prev = NULL;
}
