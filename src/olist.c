#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "olist.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée ordonnée.
 * Renvoie le nouveau nœud créé.
 */
static OLNode * newOLNode(void* key, void* data) {
	/* A FAIRE */
	OLNode* node = malloc(sizeof(OLNode));
	node->key = key;
	node->data = data;
	node->pred = NULL;
	node->succ = NULL;
	return node;
}

OList * newOList(int (*preceed)(const void*, const void*),
				void (*viewKey)(const void*), void (*viewData)(const void*),
				void (*freeKey)(void*), void (*freeData)(void*)) {
	/* A FAIRE */
	OList* liste = malloc(sizeof(OList));
	liste->head = NULL;
	liste->tail = NULL;
	liste->numelm = 0;
	liste->preceed = preceed;
	liste->viewKey = viewKey;
	liste->viewData = viewData;
	liste->freeKey = freeKey;
	liste->freeData = freeData;
}

void freeOList(OList * L, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);
	/* A FAIRE */
	for ( OLNode* iterator = L->head; iterator; ) {
    	OLNode* node = iterator;
    	iterator = iterator->succ;
		if (deleteKey==1) {
			L->freeKey(node->key);
		}
		if (deleteData==1) {
			L->freeData(node->data);
		}
    	free(node);
    }
	free(L);
}

void viewOList(const OList * L) {
	/* A FAIRE */
	printf( "[ " );
	for( OLNode* iterator = L->head; iterator->succ; iterator = iterator->succ ) {
		printf("key : ")
		L->viewKey(iterator->key);
		printf(" data : ")
		L->viewData(iterator->data);
		printf(", ");
  	}
	printf("key : ")
	L->viewKey(L->tail->key);
	printf(" data : ")
	L->viewData(L->tail->data);
 	printf( "]\n\n" );
}

void OListInsert(OList * L, void * key, void * data) {
	/* A FAIRE */
	OLNode* InsertedNode = newOLNode(key, data);
	if (L->numelm==0) {
		// insertion dans une liste vide
		L->head = InsertedNode;
		L->tail = InsertedNode;
	}
	else { // la liste n'est pas vide
		if (preceed(key, L->head->key) == 1) {
			// insertion en tête
			L->head->pred = InsertedNode;
			InsertedNode->succ = L->head;
			L->head = InsertedNode;
		}
		else {
			if (preceed(L->tail->key, key) == 1) {
			// insertion en queue
				L->tail->succ = InsertedNode;
				InsertedNode->pred = L->tail;
				L->tail = InsertedNode;
			}
			else {
				// insertion en une position quelconque de la liste

				OLNode* iterator = L->head;
				// On cherche à quelle position il faut insérer le nouveau noeud :

				// preceed(iterator->key, key) retourne :
				// 1 si iterator->key < key
				// 0 sinon
				while(L->preceed(iterator->key, key) == 1) { // tant que iterator->key < key
					iterator = iterator->succ;
				}
				// ici iterator->key >= key
				// il faut donc insérer le noeud en key juste avannt iterator

				InsertedNode->pred = iterator->pred;
				InsertedNode->succ = iterator;

				iterator->pred->succ = InsertedNode;
				iterator->pred = InsertedNode;
				free(iterator);
			}
		}
	}
	L->numelm += 1;
}

List* OListToList(const OList* L) {
	/* A FAIRE */
	List res = newList(L->viewData, L->freeData);
	for( OLNode* iterator = L->head; iterator; iterator = iterator->succ ){
		listInsertLast(res, iterator->data);
	}
	return res;
}