#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée.
 * Renvoie le nouveau nœud créé.
 */
static LNode * newLNode(void * data) {
	LNode* node = malloc(sizeof(LNode));
	node->data = data;
	node->pred = NULL;
	node->succ = NULL;
	return node;
}

List * newList(void (*viewData)(const void*), void (*freeData)(void*)) {
	List* liste = malloc(sizeof(List));
	liste->head = NULL;
	liste->tail = NULL;
	liste->numelm = 0;
	liste->viewData = viewData;
	liste->freeData = freeData;
}

void freeList(List * L, int deleteData) {
	assert(deleteData == 0 || deleteData == 1);
	for ( LNode* iterator = L->head; iterator; ) {
    	LNode* node = iterator;
    	iterator = iterator->succ;
		if (deleteData==1) {
			L->freeData(node->data);
		}
    	free(node);
    }
	free(L);
}

void viewList(const List * L) {
	printf( "[ " );
	for( LNode* iterator = L->head; iterator->succ; iterator = iterator->succ ) {
		L->viewData((int*)iterator->data); // il faut cast la data (de type void*) en int* car viewInt prend un int* en paramètre
		printf(", ");
  	}
	L->viewData((int*)L->tail->data);
 	printf( " ]\n" );
}

void listInsertFirst(List * L, void * data) {
	LNode* InsertedNode = newLNode(data);
	if (L->numelm == 0) {
		L->head = InsertedNode;
		L->tail = InsertedNode;
	}
	else {
		LNode* Head = L->head;
		Head->pred = InsertedNode;
		InsertedNode->succ = Head;
		L->head = InsertedNode;
	}
	L->numelm += 1;
}

void listInsertLast(List * L, void * data) {
	LNode* InsertedNode = newLNode(data);
	if (L->numelm == 0) {
		L->head = InsertedNode;
		L->tail = InsertedNode;
	}
	else {
		LNode* Tail = L->tail;
		Tail->succ = InsertedNode;
		InsertedNode->pred = Tail;
		L->tail = InsertedNode;
	}
	L->numelm += 1;
}

void listInsertAfter(List * L, void * data, LNode * ptrelm) {
	if( L->numelm == 0 || ptrelm == NULL ) {
		listInsertFirst(L, data);
	}
	else {
		LNode* InsertedNode = newLNode(data);
		InsertedNode->pred = ptrelm;
		InsertedNode->succ = ptrelm->succ;
		ptrelm->succ = InsertedNode;
		L->numelm += 1;
	}
}

LNode* listRemoveFirst(List * L) {
	assert(L->head);
	if (L->numelm > 0) {
		LNode* Head = L->head;
		L->head = L->head->succ;
		L->head->pred = NULL;
		L->numelm -= 1;
		return Head;
	}
	else {
		return NULL;
	}
}

LNode* listRemoveLast(List * L) {
	assert(L->head);
	if (L->numelm > 0) {
		LNode* Tail = L->tail;
		L->tail = L->tail->pred;
		L->tail->succ = NULL;
		L->numelm -= 1;
		return Tail;
	}
	else {
		return NULL;
	}
}

LNode* listRemoveNode(List * L, LNode * node) {
	if (L->numelm > 0) {
		if (node == L->head)
			return listRemoveFirst(L);
		else if (node == L->tail)
			return listRemoveLast(L);
		else {
			node->pred->succ = node->succ;
			node->succ->pred = node->pred;
			L->numelm -= 1;
			return node;
		}
	}
}
