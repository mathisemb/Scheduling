#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"

/***********************************************
 * TASK
 ***********************************************/

Task * newTask(char* id, int proctime, int reltime, int deadline, int weight) {
	assert(proctime > 0);
	assert(reltime >= 0);
	assert((deadline >= reltime + proctime));
	Task* task = malloc(sizeof(Task));
	task->id = malloc(sizeof(id));
	strcpy(task->id, id);
	task->processingTime = proctime;
	task->releaseTime = reltime;
	task->deadline = deadline;
	task->weight = weight;
	return task;
}

void freeTask(void* task) {
	Task * T = (Task *) task;
	free(T->id);
	free(T);
}

void viewTask(const void *task) {
	Task * T = (Task *) task;
	printf("Task : %s", T->id);
	printf("\nProcessing Time = %d", T->processingTime);
	printf("\nRelease Time = %d", T->releaseTime);
	printf("\nDeadline = %d", T->deadline);
	printf("\nWeight = %d\n", T->weight);
}

/************************************************
 * INSTANCE
 ************************************************/

Instance readInstance(char * filename) {
    FILE* file = fopen(filename,"r"); // on ouvre le fichier filename en mode lecture  

	if (file == NULL) printf("Problème avec l'ouverture du ficher : %s", filename);

	Instance I = newList(viewTask, freeTask); // une instance est une list de noeud de data de type Task

    // strtok permet de découper le contenu du fichier selon les espaces
    // dans le premier appel de strtok on doit préciser qu'on travaille la chaine task alors qu'ensuite il faut utiliser NULL

	char task[BUFSIZ];
	while (fgets(task, BUFSIZ, file) != NULL) {
		/*
		char* id = strtok(task,"\t");
		int proctime = atoi(strtok(NULL,"\t"));
		int reltime = atoi(strtok(NULL,"\t"));
		int deadline = atoi(strtok(NULL,"\t"));
		int weight = atoi(strtok(NULL,"\t"));
		*/

		char* id = strtok(task,"	\t ");
		int proctime = atoi(strtok(NULL,"	\t "));
		int reltime = atoi(strtok(NULL,"	\t "));
		int deadline = atoi(strtok(NULL,"	\t "));
		int weight = atoi(strtok(NULL,"	\t "));

		Task* T = newTask(id, proctime, reltime, deadline, weight);
		listInsertLast(I, T);
	}
	fclose(file);
	return I;
}

void viewInstance(Instance I) {
	for(LNode* iterator = I->head; iterator; iterator = iterator->succ) {
		viewTask(iterator->data);
		printf("\n");
	}
}

void freeInstance(Instance I, int deleteData) {
	freeList(I, deleteData);
}

/*****************************************************************************
 * Ordonner l'instance
 *****************************************************************************/

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre SPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre SPT : a précède b si
 * (+) durée de a < durée de b
 * (+) durée de a = durée de b ET date de libération de a < date de libération de b
 */
static int spt(const void* a, const void* b) {
	Task* taskA = (Task*) a;
	Task* taskB = (Task*) b;
	if (taskA->processingTime < taskB->processingTime)
		return 1;
	else if ((taskA->processingTime == taskB->processingTime) && (taskA->releaseTime < taskB->releaseTime))
		return 1;
	else
		return 0;
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre LPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre LPT : a précède b si
 * (+) durée de a > durée de b
 * (+) durée de a = durée de b ET date de libération de a < date de libération de b
 */
static int lpt(const void* a, const void* b) {
	Task* taskA = (Task*) a;
	Task* taskB = (Task*) b;
	if (taskA->processingTime > taskB->processingTime)
		return 1;
	else if ((taskA->processingTime == taskB->processingTime) && (taskA->releaseTime < taskB->releaseTime))
		return 1;
	else
		return 0;
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre WSPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre WSPT : a précède b si
 * (+) poids / durée de a > poids / durée de b
 * (+) poids / durée de a = poids / durée de b
 *     ET durée de a < durée de b
 * (+) poids / durée de a = poids / durée de b
 *     ET durée de a = durée de b
 *     ET date de libération de a < date de libération de b
 */
static int wspt(const void* a, const void* b) {
	Task* taskA = (Task*) a;
	Task* taskB = (Task*) b;
	if ((taskA->weight/taskA->processingTime) > (taskB->weight/taskB->processingTime))
		return 1;
	else if (((taskA->weight/taskA->processingTime) == (taskB->weight/taskB->processingTime)) && (taskA->processingTime < taskB->processingTime))
		return 1;
	else if (((taskA->weight/taskA->processingTime) == (taskB->weight/taskB->processingTime)) && (taskA->processingTime == taskB->processingTime) && (taskA->releaseTime < taskB->releaseTime))
		return 1;
	else
		return 0;
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre FCFS :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre FCFS : a précède b si
 * (+) date de libération de a < date de libération de b
 * (+) date de libération de a = date de libération de b
 *     ET durée de a > durée de b
 */
static int fcfs(const void* a, const void* b) {
	Task* taskA = (Task*) a;
	Task* taskB = (Task*) b;
	if (taskA->releaseTime < taskB->releaseTime)
		return 1;
	else if ((taskA->releaseTime == taskB->releaseTime) && (taskA->processingTime > taskB->processingTime))
		return 1;
	else
		return 0;
}

void reorderInstance(Instance I,  DataStructure structtype, Order order) {
	OList * list;
	BSTree * tree;
	List * toList;

	// On choisit notre ordre d'ordonnancement
	int (*preceed)(const void *, const void *);
	switch (order) {
		case SPT : preceed = &spt;
			break;
		case LPT : preceed = &lpt;
			break;
		case WSPT : preceed = &wspt;
			break;
		case FCFS : preceed = &fcfs;
			break;
	}

	switch (structtype) {
		case OL :
			list = newOList(*preceed, viewTask, viewTask, freeTask, freeTask);
			for(LNode* iterator = I->head; iterator; iterator = iterator->succ) {
				OListInsert(list, iterator->data, iterator->data);
			}
			
			for ( LNode* iterator = I->head; iterator; ) {
				LNode* node = iterator;
				iterator = iterator->succ;
				free(node);
			}

			toList = OListToList(list);
			*I = *((Instance)toList);
			free(toList);

			freeOList(list, 0, 0);
			break;

		case BST :
			tree = newBSTree(*preceed, viewTask, viewTask, freeTask, freeTask);
			for(LNode* iterator = I->head; iterator; iterator = iterator->succ) {
				BSTreeInsert(tree, iterator->data, iterator->data);
			}

			for ( LNode* iterator = I->head; iterator; ) {
				LNode* node = iterator;
				iterator = iterator->succ;
				free(node);
			}

			toList = BSTreeToList(tree);
			*I = *((Instance)toList);
			free(toList);

			freeBSTree(tree, 0, 0);
			break;

		case EBST :
			tree = newEBSTree(*preceed, viewTask, viewTask, freeTask, freeTask);
			for(LNode* iterator = I->head; iterator; iterator = iterator->succ) {
				EBSTreeInsert(tree, iterator->data, iterator->data);
			}

			for ( LNode* iterator = I->head; iterator; ) {
				LNode* node = iterator;
				iterator = iterator->succ;
				free(node);
			}

			toList = BSTreeToList(tree);
			*I = *((Instance)toList);
			free(toList);

			freeBSTree(tree, 0, 0);
			break;
	}
}
