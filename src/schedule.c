#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

Schedule * newSchedule(DataStructure structtype, int backfilling) {
	Schedule * sched = calloc(1, sizeof(Schedule));

	sched->structtype = structtype;
	sched->backfilling = backfilling;

	switch (structtype) {
	case OL:
		sched->scheduledTasks = newOList(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	case BST:
		sched->scheduledTasks = newBSTree(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	case EBST:
		sched->scheduledTasks = newEBSTree(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	default:
		error("Schedule:newSchedule : invalid data structure.");
		break;
	}

	return sched;
}

void viewSchedule(Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		viewOList(sched->scheduledTasks);
		break;
	case BST:
		viewBSTree(sched->scheduledTasks);
		break;
	case EBST:
		viewBSTree(sched->scheduledTasks);
		break;
	default:
		error("Schedule:viewSchedule : invalid data structure.");
		break;
	}
}

void freeSchedule(Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		freeOList(sched->scheduledTasks, 1, 0);
		break;
	case BST:
		freeBSTree(sched->scheduledTasks, 1, 0);
		break;
	case EBST:
		freeBSTree(sched->scheduledTasks, 1, 0);
		break;
	default:
		error("Schedule:freeSchedule : invalid data structure.");
		break;
	}
	free(sched);
}

void addTaskToSchedule(Schedule* sched, int startingTime, Task* task) {
	int *stime = malloc(sizeof(int));
	*stime = startingTime;
	switch (sched->structtype) {
	case OL:
		OListInsert(sched->scheduledTasks, stime, task);
		break;
	case BST:
		BSTreeInsert(sched->scheduledTasks, stime, task);
		break;
	case EBST:
		EBSTreeInsert(sched->scheduledTasks, stime, task);
		break;
	default:
		error("Schedule:addTaskToSchedule : invalid data structure.");
		break;
	}
}

/*****************************************************************************
 * Compute schedule
 *****************************************************************************/

/**
 * @brief
 * Vérifier si on peut faire du backfilling avec la tâche task dans l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 * Si le backfilling est possible, renvoie la date de début la plus petite.
 * Sinon (on ne peut pas faire du backfilling), renvoie -1.
 * NB : fonction itérative.
 */
static int OLFindBackfillingPosition(const OList* scheduledTasks, const Task* task) {
	/* A FAIRE */
}

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute
 * dans la structure scheduledTasks de type liste ordonnée.
 * La date de début dépend de l'application du backfilling (backfilling=1) ou pas (backfilling=0).
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 */
static int OLFindStartingTime(const OList *scheduledTasks, const Task* task, int backfilling) {
	/* A FAIRE */
}

/**
 * @brief
 * Vérifier si on peut faire du backfilling avec la tâche task dans l'ordonnancement
 * représenté par le sous-arbre raciné à curr de l'arbre binaire de recherche scheduledTasks.
 * Si le backfilling est possible, renvoie la date de début la plus petite.
 * Sinon (on ne peut pas faire du backfilling), renvoie -1.
 * NB : fonction récursive, l'ordre infixe est conseillé.
 */
static int BSTFindBackfillingPosition(const BSTree* scheduledTasks, const BSTNode* curr, const Task* task) {
	/* A FAIRE */
}

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute
 * dans la structure scheduledTasks de type arbre binaire de recherche.
 * La date de début dépend de l'application du backfilling (backfilling=1) ou pas (backfilling=0).
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 *      Utiliser la fonction récursive findBackfillingPosition.
 */
static int BSTFindStartingTime(const BSTree *scheduledTasks, const Task* task, int backfilling) {
	/* A FAIRE */
}

int findStartingTime(const Schedule *sched, const Task* task) {
	switch (sched->structtype) {
	case OL:
		return OLFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	case BST:
		return BSTFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	case EBST:
		return BSTFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	default:
		error("Schedule:findStartingTime : invalid data structure.");
		return -1;
	}
}

void computeSchedule(Schedule *sched, const Instance I) {
	for (LNode* curr = I->head; curr; curr = curr->succ)
		addTaskToSchedule(sched, findStartingTime(sched, curr->data), curr->data);
}

/*****************************************************************************
 * Save schedule
 *****************************************************************************/

/**
 * @brief
 * Sauvegarder l'ordonnancement représenté par la liste ordonnée scheduledTasks
 * dans le ficher indiqué par le descripteur fd.
 * NB : Procédure itérative
 */
static void OLSaveSchedule(const OList* scheduledTasks, FILE* fd) {
	/* A FAIRE */
}

/**
 * @brief
 * Sauvegarder l'ordonnancement représenté par le sous-arbre raciné au nœud curr
 * dans le ficher indiqué par le descripteur fd.
 * NB : procédure récursive
 *      Pensez à un parcours infixe.
 */
static void BSTSaveSchedule(const BSTNode* curr, FILE* fd) {
	/* A FAIRE */
}

void saveSchedule(const Schedule * sched, char* filename) {
	FILE* fd;
	if ((fd = fopen(filename, "w")) == NULL)
		error("schedule:saveSchedule : Error while opening the file");

	switch (sched->structtype) {
	case OL:
		OLSaveSchedule(sched->scheduledTasks, fd);
		break;
	case BST:
		BSTSaveSchedule(((BSTree*) sched->scheduledTasks)->root, fd);
		break;
	case EBST:
		BSTSaveSchedule(((BSTree*) sched->scheduledTasks)->root, fd);
		break;
	default:
		error("Schedule:saveSchedule : invalid data structure.");
		break;
	}
	fclose(fd);
}

/*****************************************************************************
 * OBJECTIFS
 *****************************************************************************/

/////////////////////// makespan ///////////////////////

long makespan(const Schedule * sched) {
	/* A FAIRE */
}

/////////////////////// SumWjCj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjCj(const OList* scheduledTasks) {
	/* A FAIRE */
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjCj(const BSTNode* curr) {
	/* A FAIRE */
}

long SumWjCj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjCj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjCj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjCj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjCj : invalid data structure.");
		return -1;
	}
}

/////////////////////// SumWjFj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjFj(const OList* scheduledTasks) {
	/* A FAIRE */
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjFj(const BSTNode* curr) {
	/* A FAIRE */
}

long SumWjFj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjFj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjFj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjFj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjFj : invalid data structure.");
		return -1;
	}
}

/////////////////////// SumWjTj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjTj(const OList* scheduledTasks) {
	/* A FAIRE */
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjTj(const BSTNode* curr) {
	/* A FAIRE */
}

long SumWjTj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjTj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjTj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjTj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjTj : invalid data structure.");
		return -1;
	}
}