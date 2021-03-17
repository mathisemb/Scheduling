#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

int main(int argv, char *argc[]) {
	/*
	Instance I = readInstance("./data/exemple");
	viewInstance(I);
	reorderInstance(I, EBST, LPT);
	viewInstance(I);

	Schedule *SOL = newSchedule(OL, 1);
	computeSchedule(SOL, I);
	viewSchedule(SOL);
	saveSchedule(SOL, "./data/output_OL");
	printf("Makespan=%ld\n", makespan(SOL));
	printf("SumWjCj=%ld\n", SumWjCj(SOL));
	printf("SumWjFj=%ld\n", SumWjFj(SOL));
	printf("SumWjTj=%ld\n", SumWjTj(SOL));
	freeSchedule(SOL);

	Schedule *SBST = newSchedule(BST, 1);
	computeSchedule(SBST, I);
	viewSchedule(SBST);
	saveSchedule(SBST, "./data/output_BST");
	printf("Makespan=%ld\n", makespan(SBST));
	printf("SumWjCj=%ld\n", SumWjCj(SBST));
	printf("SumWjFj=%ld\n", SumWjFj(SBST));
	printf("SumWjTj=%ld\n", SumWjTj(SBST));
	freeSchedule(SBST);

	Schedule *SEBST = newSchedule(EBST, 1);
	computeSchedule(SEBST, I);
	viewSchedule(SEBST);
	saveSchedule(SEBST, "./data/output_EBST");
	printf("Makespan=%ld\n", makespan(SEBST));
	printf("SumWjCj=%ld\n", SumWjCj(SEBST));
	printf("SumWjFj=%ld\n", SumWjFj(SEBST));
	printf("SumWjTj=%ld\n", SumWjTj(SEBST));
	freeSchedule(SEBST);

	freeInstance(I, 1);
	*/



	// Test List
	List* L = newList(viewInt, freeInt); // L = []

	int *i1 = malloc(sizeof(int));
	*i1 = 1;
	listInsertLast(L, i1); // L = [ 1 ]

	int *i2 = malloc(sizeof(int));
	*i2 = 253;
	listInsertFirst(L, i2); // L = [ 253, 1 ]

	LNode *node = L->head;

	int *i3 = malloc(sizeof(int));
	*i3 = 10;
	listInsertAfter(L, i3, node); // L = [ 253, 10, 1 ]

	int *i4 = malloc(sizeof(int));
	*i4 = 14;
	listInsertLast(L, i4); // L = [ 253, 10, 1, 14 ]

	viewList(L); // L = [ 253, 10, 1, 14 ]

	LNode *first = listRemoveFirst(L); // L = [ 10, 1, 14 ]
	LNode *last = listRemoveLast(L); // L = [ 10, 1 ]

	LNode *node1 = L->head;

	LNode *node2 = listRemoveNode(L, node1); // L = [ 1 ]

	viewList(L); // L = [ 1 ]

	// Il faut free les nodes retirés car listRemove ne libère pas leur mémoire :
	L->freeData(first->data);
    free(first);
	L->freeData(last->data);
    free(last);
	L->freeData(node2->data);
    free(node2);
	freeList(L,1);



	// Test OList
	OList* OL = newOList(compareInt , viewInt, viewInt, freeInt, freeInt); // L = []

	int *k1 = malloc(sizeof(int));
	*k1 = 1;
	int *n1 = malloc(sizeof(int));
	*n1 = 1;
	OListInsert(OL, k1, n1); // L = [ 1 ]

	int *k2 = malloc(sizeof(int));
	*k2 = 7;
	int *n2 = malloc(sizeof(int));
	*n2 = 55;
	OListInsert(OL, k2, n2); // L = [ 1, 55 ]

	int *k3 = malloc(sizeof(int));
	*k3 = 3;
	int *n3 = malloc(sizeof(int));
	*n3 = 3;
	OListInsert(OL, k3, n3); // L = [ 1, 3, 55 ]

	viewOList(OL); // L = [ 1, 3, 55 ]

	List* L2 = OListToList(OL);

	freeOList(OL,1,1);

	viewList(L2); // L = [ 1, 3, 55 ]
	freeList(L2, 1); // car OListToList utilise newList



	return EXIT_SUCCESS;
}
