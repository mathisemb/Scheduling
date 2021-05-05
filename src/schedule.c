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
	Task* task1;
	Task* task2;

	// date de commencement des tâches task1 et task2
	int task1Start;
	int task2Start;

	if (scheduledTasks->numelm >=2) {
		for(OLNode* iterator = scheduledTasks->head; iterator->succ; iterator = iterator->succ) { // tant que le succ existe
			task1Start = *((int*)iterator->key);
			task1 = (Task*)(iterator->data); // la tache de l'iterator
			task2Start = *((int*)iterator->succ->key);
			task2 = (Task*)(iterator->succ->data); // la tache du successeur de l'iterator

			if (task2Start - (task1Start + task1->processingTime) >= task->processingTime) { // si il y a de la place pour mettre task
				if (task2Start - task->releaseTime >= task->processingTime){// et si task est libérée suffisamment tôt
					return max(task1Start + task1->processingTime, task->releaseTime);
				}
			}
		}
	}
	else
		if (scheduledTasks->numelm == 1) {
			task1Start = *((int*)scheduledTasks->head->key);
			task1 = (Task*)(scheduledTasks->head->data);

			if (task1Start > task->processingTime) { // si il y a de la place pour mettre task
				if (task1Start - task->releaseTime > task->processingTime){// et si task est libérée suffisamment tôt
					return task->releaseTime;
				}
			}
		}
		else // (==0)
			return task->releaseTime;

	return -1;
}

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute
 * dans la structure scheduledTasks de type liste ordonnée.
 * La date de début dépend de l'application du backfilling (backfilling=1) ou pas (backfilling=0).
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 */
static int OLFindStartingTime(const OList *scheduledTasks, const Task* task, int backfilling) {
	if (backfilling == 1) {
		int backfill = OLFindBackfillingPosition(scheduledTasks, task);
		if (backfill != -1)
			return backfill;
	}
	if (scheduledTasks->numelm > 0) {
		OLNode* tail = scheduledTasks->tail;
		int tailStart = *((int*)tail->key);
		Task* tailTask = (Task*)(tail->data);
		return max(tailStart + tailTask->processingTime, task->releaseTime);
	}
	else
		return task->releaseTime;
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
	if (curr == NULL) {
		return -1;
	}
	else {
		if (task->releaseTime + task->processingTime <= *((int*)curr->key)) { // il est possible que task rentre dans le fils gauche de curr puisqu'il se pourrait
																	 		  // qu'elle ne dépasse pas le startingTime de curr
			int left = BSTFindBackfillingPosition(scheduledTasks, curr->left, task); // on regarde dans le fils gauche récursivement
			if (left == -1) { // pas possible à gauche
				// on regarde entre curr et son prédecesseur
				BSTNode * currPred = findPredecessor(scheduledTasks, curr);
				if (currPred != NULL) { // curr a bien un prédecesseur
					if ( *((int*)curr->key) - ( *((int*)currPred->key) + ((Task*)currPred)->processingTime) > task->processingTime ) // il y a la place et 
																																	 // d'après le premier if
																												 					 // task ne dépassera pas sur curr
						return max( *((int*)currPred->key) + ((Task*)currPred)->processingTime, task->releaseTime);
					else // il n'y a pas la place entre curr et son prédecesseur => on a tout essayé à gauche, donc on cherche à droite
						return BSTFindBackfillingPosition(scheduledTasks, curr->right, task); // il se pourrait que ça fonctionne à droite
																							  // si ça retourne -1 c'est juste car on aura
																							  // tout essayé
				}
				else // curr n'a pas de predecesseur, c'est donc la première tache => on a tout essayé à gauche, donc on cherche à droite
					return BSTFindBackfillingPosition(scheduledTasks, curr->right, task); // il se pourrait que ça fonctionne à droite
																						  // si ça retourne -1 c'est juste car on aura
																						  // tout essayé
			}
			else // c'est possible à gauche
				return left;
		}
		else // impossible dans le fils gauche donc on renvoie le résultat du fils droit
			return BSTFindBackfillingPosition(scheduledTasks, curr->right, task);
	}
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
	if (backfilling == 1) {
		int backfill = BSTFindBackfillingPosition(scheduledTasks, scheduledTasks->root, task);
		if (backfill != -1)
			return backfill;
	}
	if (scheduledTasks->numelm > 0) {
		BSTNode* last = BSTMax(scheduledTasks->root);
		int lastStart = *((int*)last->key);
		Task* lastTask = (Task*)(last->data);
		return max(lastStart + lastTask->processingTime, task->releaseTime);
	}
	else
		return task->releaseTime;
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
	for(OLNode* iterator = scheduledTasks->head; iterator; iterator = iterator->succ) {
		Task * iterTask = (Task*)(iterator->data);
		char * id = iterTask->id;
		int p = iterTask->processingTime;
		int r = iterTask->releaseTime;
		int d = iterTask->deadline;
		int w = iterTask->weight;
		int s = *((int*)iterator->key);
		fprintf(fd, "%s\t%d\t%d\t%d\t%d\t%d\n", id, p, r, d, w, s);
	}
}

/**
 * @brief
 * Sauvegarder l'ordonnancement représenté par le sous-arbre raciné au nœud curr
 * dans le ficher indiqué par le descripteur fd.
 * NB : procédure récursive
 *      Pensez à un parcours infixe.
 */
static void BSTSaveSchedule(const BSTNode* curr, FILE* fd) {
	if (curr != NULL) {
		BSTSaveSchedule(curr->left, fd);

		Task * currTask = (Task*)(curr->data);
		char * id = currTask->id;
		int p = currTask->processingTime;
		int r = currTask->releaseTime;
		int d = currTask->deadline;
		int w = currTask->weight;
		int s = *((int*)curr->key);
		fprintf(fd, "%s\t%d\t%d\t%d\t%d\t%d\n", id, p, r, d, w, s);
		
		BSTSaveSchedule(curr->right, fd);
	}
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
	OList * liste;
	OLNode* tail;
	int tailStart;
	Task* tailTask;

	BSTree * arbre;
	BSTNode* last;
	int lastStart;
	Task* lastTask;
	switch (sched->structtype) {
		case OL: 
			liste = (OList*)(sched->scheduledTasks);
			tail = liste->tail;
			tailStart = *((int*)tail->key);
			tailTask = (Task*)(tail->data);
			return tailStart + tailTask->processingTime;
			break;
		case BST:
			arbre = (BSTree*)(sched->scheduledTasks);
			last = BSTMax(arbre->root);
			lastStart = *((int*)last->key);
			lastTask = (Task*)(last->data);
			return lastStart + lastTask->processingTime;
			break;
		case EBST:
			arbre = (BSTree*)(sched->scheduledTasks);
			last = BSTMax(arbre->root);
			lastStart = *((int*)last->key);
			lastTask = (Task*)(last->data);
			return lastStart + lastTask->processingTime;
			break;
		default:
			error("Schedule:makespan : invalid data structure.");
			break;
	}
}

/////////////////////// SumWjCj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjCj(const OList* scheduledTasks) {
	long sum = 0;
	for(OLNode * iter = scheduledTasks->head; iter; iter = iter->succ) {
		Task * iterTask = (Task*)(iter->data);
		int s = *((int*)iter->key);
		int C = s + iterTask->processingTime;
		int w = iterTask->weight;
		sum += (w*C);
	}
	return sum;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjCj(const BSTNode* curr) {
	if (curr != NULL) {
		Task * currTask = (Task*)(curr->data);
		int s = *((int*)curr->key);
		int C = s + currTask->processingTime;
		int w = currTask->weight;
		return BSTSumWjCj(curr->left) + (w*C) + BSTSumWjCj(curr->right);
	}
	else
		return 0;
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
	long sum = 0;
	for(OLNode * iter = scheduledTasks->head; iter; iter = iter->succ) {
		Task * iterTask = (Task*)(iter->data);
		int s = *((int*)iter->key);
		int C = s + iterTask->processingTime;
		int F = C - iterTask->releaseTime;
		int w = iterTask->weight;
		sum += (w*F);
	}
	return sum;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjFj(const BSTNode* curr) {
	if (curr != NULL) {
		Task * currTask = (Task*)(curr->data);
		int s = *((int*)curr->key);
		int C = s + currTask->processingTime;
		int F = C - currTask->releaseTime;
		int w = currTask->weight;
		return BSTSumWjFj(curr->left) + (w*F) + BSTSumWjFj(curr->right);
	}
	else
		return 0;
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
	long sum = 0;
	for(OLNode * iter = scheduledTasks->head; iter; iter = iter->succ) {
		Task * iterTask = (Task*)(iter->data);
		int s = *((int*)iter->key);
		int C = s + iterTask->processingTime;
		int T = max(0, C - iterTask->deadline);
		int w = iterTask->weight;
		sum += (w*T);
	}
	return sum;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjTj(const BSTNode* curr) {
	if (curr != NULL) {
		Task * currTask = (Task*)(curr->data);
		int s = *((int*)curr->key);
		int C = s + currTask->processingTime;
		int T = max(0, C - currTask->deadline);
		int w = currTask->weight;
		return BSTSumWjTj(curr->left) + (w*T) + BSTSumWjTj(curr->right);
	}
	else
		return 0;
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