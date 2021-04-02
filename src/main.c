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
	OList* OrdL = newOList(compareInt , viewInt, viewInt, freeInt, freeInt); // L = []

	int *k1 = malloc(sizeof(int));
	*k1 = 1;
	int *n1 = malloc(sizeof(int));
	*n1 = 1;
	OListInsert(OrdL, k1, n1); // L = [ 1 ]

	int *k2 = malloc(sizeof(int));
	*k2 = 7;
	int *n2 = malloc(sizeof(int));
	*n2 = 55;
	OListInsert(OrdL, k2, n2); // L = [ 1, 55 ]

	int *k3 = malloc(sizeof(int));
	*k3 = 3;
	int *n3 = malloc(sizeof(int));
	*n3 = 3;
	OListInsert(OrdL, k3, n3); // L = [ 1, 3, 55 ]

	viewOList(OrdL); // L = [ 1, 3, 55 ]

	List* L2 = OListToList(OrdL);

	freeOList(OrdL,1,0);

	viewList(L2); // L = [ 1, 3, 55 ]
	freeList(L2, 1); // car OListToList utilise newList




	// Test Tree
	printf("1er test (BST) :\n");
	BSTree* T = newBSTree(compareInt, viewInt, viewInt, freeInt, freeInt);
	
	int *k18 = malloc(sizeof(int)); *k18 = 2;
	int *d18 = malloc(sizeof(int)); *d18 = 36;
	BSTreeInsert(T, k18, d18);

	int *k19 = malloc(sizeof(int)); *k19 = 1;
	int *d19 = malloc(sizeof(int)); *d19 = 7;
	BSTreeInsert(T, k19, d19);

	int *k4 = malloc(sizeof(int)); *k4 = 4;
	int *d4 = malloc(sizeof(int)); *d4 = 18;
	BSTreeInsert(T, k4, d4);

	int *k20 = malloc(sizeof(int)); *k20 = 3;
	int *d20 = malloc(sizeof(int)); *d20 = 10;
	BSTreeInsert(T, k20, d20);

	viewBSTree(T);

	BSTNode* pred = findPredecessor(T, T->root);
	BSTNode* succ = findSuccessor(T, T->root);
	printf("Pred : "); viewInt((int*)pred->data);
	printf("\nSucc : "); viewInt((int*)succ->data);
	printf("\n");

	freeBSTree(T, 1, 1);


	printf("\n2eme test (EBST) :\n");
	BSTree* T2 = newEBSTree(compareInt, viewInt, viewInt, freeInt, freeInt);

	int *k5 = malloc(sizeof(int)); *k5 = 5;
	int *d5 = malloc(sizeof(int)); *d5 = 43;
	EBSTreeInsert(T2, k5, d5);

	int *k6 = malloc(sizeof(int)); *k6 = 2;
	int *d6 = malloc(sizeof(int)); *d6 = 4;
	EBSTreeInsert(T2, k6, d6);

	int *k7 = malloc(sizeof(int)); *k7 = 3;
	int *d7 = malloc(sizeof(int)); *d7 = 3;
	EBSTreeInsert(T2, k7, d7);

	viewBSTree(T2);
	List* L3 = BSTreeToList(T2);
	printf("Liste T2 :\n");
	viewList(L3);

	BSTNode* pred2 = findPredecessor(T2, T2->root);
	BSTNode* succ2 = findSuccessor(T2, T2->root);
	printf("Pred : "); viewInt((int*)pred2->data);
	printf("\nSucc : "); viewInt((int*)succ2->data);
	printf("\n");

	freeBSTree(T2, 1, 0);
	freeList(L3, 1);
	

	printf("\n3eme test (EBST) :\n");
	BSTree* T3 = newEBSTree(compareInt, viewInt, viewInt, freeInt, freeInt);

	int *k8 = malloc(sizeof(int)); *k8 = 3;
	int *d8 = malloc(sizeof(int)); *d8 = 3;
	EBSTreeInsert(T3, k8, d8);

	int *k9 = malloc(sizeof(int)); *k9 = 1;
	int *d9 = malloc(sizeof(int)); *d9 = 1;
	EBSTreeInsert(T3, k9, d9);

	int *k17 = malloc(sizeof(int)); *k17 = 2;
	int *d17 = malloc(sizeof(int)); *d17 = 2;
	EBSTreeInsert(T3, k17, d17);

	int *k10 = malloc(sizeof(int)); *k10 = 5;
	int *d10 = malloc(sizeof(int)); *d10 = 5;
	EBSTreeInsert(T3, k10, d10);

	int *k11 = malloc(sizeof(int)); *k11 = 4;
	int *d11 = malloc(sizeof(int)); *d11 = 4;
	EBSTreeInsert(T3, k11, d11);

	viewBSTree(T3);
	List* L4 = BSTreeToList(T3);
	printf("Liste T3 :\n");
	viewList(L4);

	BSTNode* predd1 = findPredecessor(T3, T3->root->left->right);
	BSTNode* succc1 = findSuccessor(T3, T3->root->left->right);
	BSTNode* predd2 = findPredecessor(T3, T3->root->right->left);
	BSTNode* succc2 = findSuccessor(T3, T3->root->right->left);
	printf("Pred de 2 : "); viewInt((int*)predd1->data);
	printf("\nSucc 2 : "); viewInt((int*)succc1->data);
	printf("\n");
	printf("Pred de 4 : "); viewInt((int*)predd2->data);
	printf("\nSucc 4 : "); viewInt((int*)succc2->data);
	printf("\n");

	freeBSTree(T3, 1, 0);
	freeList(L4, 1);


	printf("\n4eme test (EBST) :\n");
	BSTree* T4 = newEBSTree(compareInt, viewInt, viewInt, freeInt, freeInt);

	int *k12 = malloc(sizeof(int)); *k12 = 4;
	int *d12 = malloc(sizeof(int)); *d12 = 4;
	EBSTreeInsert(T4, k12, d12);

	int *k13 = malloc(sizeof(int)); *k13 = 2;
	int *d13 = malloc(sizeof(int)); *d13 = 2;
	EBSTreeInsert(T4, k13, d13);

	int *k16 = malloc(sizeof(int)); *k16 = 3;
	int *d16 = malloc(sizeof(int)); *d16 = 3;
	EBSTreeInsert(T4, k16, d16);

	int *k14 = malloc(sizeof(int)); *k14 = 5;
	int *d14 = malloc(sizeof(int)); *d14 = 5;
	EBSTreeInsert(T4, k14, d14);

	int *k15 = malloc(sizeof(int)); *k15 = 6;
	int *d15 = malloc(sizeof(int)); *d15 = 6;
	EBSTreeInsert(T4, k15, d15);

	viewBSTree(T4);

	BSTNode* pred3 = findPredecessor(T4, T4->root->right->right);
	BSTNode* succ3 = findSuccessor(T4, T4->root->left);
	BSTNode* succ4 = findSuccessor(T4, T4->root->left->right);
	printf("Pred 6 : "); viewInt((int*)pred3->data);
	printf("\nSucc de 2 : "); viewInt((int*)succ3->data);
	printf("\nSucc de 3 : "); viewInt((int*)succ4->data);
	printf("\n");

	freeBSTree(T4, 1, 1);




	// Test Instance
	Instance I = readInstance("./data/exemple");
	//viewInstance(I);

	reorderInstance(I, OL, SPT);
	//reorderInstance(I, BST, SPT);
	//reorderInstance(I, EBST, SPT);

	viewInstance(I);

	freeInstance(I, 1);



	return EXIT_SUCCESS;
}
