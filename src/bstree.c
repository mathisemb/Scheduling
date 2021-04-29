#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "bstree.h"

/*********************************************************************
 * Arbre Binaire de Recherche (non-équilibré).
 *********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche (non-équilibré).
 * Renvoie le nouveau nœud créé.
 */
static BSTNode* newBSTNode(void* key, void* data) {
	BSTNode* node = malloc(sizeof(BSTNode));
	node->key = key;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
}

BSTree * newBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {
	BSTree* arbre = malloc(sizeof(BSTree));
	arbre->root = NULL;
	arbre->numelm = 0;
	arbre->preceed = preceed;
	arbre->viewKey = viewKey;
	arbre->viewData = viewData;
	arbre->freeKey = freeKey;
	arbre->freeData = freeData;
	arbre->balanced = 0;
}

/**
 * @brief
 * Insérer un nouveau nœud de clé key et donnée data
 * au sous-arbre binaire de recherche (non-équilibré) raciné au nœud curr.
 * Les clés sont comparées en utilisant la fonction preceed.
 * NB : fonction récursive.
 */
static BSTNode* insertBSTNode(BSTNode* curr, void* key, void* data, int (*preceed)(const void*, const void*)) {
	if (curr == NULL) {
		return newBSTNode(key, data);
	}
	else {
		if (preceed(key, curr->key)) {
			if (curr->left == NULL) {
				curr->left = newBSTNode(key, data);
			}
			else
				curr->left = insertBSTNode(curr->left, key, data, preceed);
		}
		else {
			if (curr->right == NULL) {
				curr->right = newBSTNode(key, data);
			}
			else
				curr->right = insertBSTNode(curr->right, key, data, preceed);
		}
		return curr;
	}
}

/**
 * NB : Utiliser la fonction récursive insertBSTNode.
 */
void BSTreeInsert(BSTree* T, void* key, void* data) {
	T->root = insertBSTNode(T->root, key, data, T->preceed);
}

/*********************************************************************
 * Arbre Binaire de Recherche Équilibré
 *********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche équilibré (EBST).
 * Renvoie le nouveau nœud créé.
 * N'oubliez pas à initialiser le facteur d'équilibre.
 */
static BSTNode* newEBSTNode(void* key, void* data) {
	BSTNode* node = malloc(sizeof(BSTNode));
	node->key = key;
	node->data = data;
	node->bfactor = 0;
	node->left = NULL;
	node->right = NULL;
}

BSTree * newEBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {
	BSTree* arbre = malloc(sizeof(BSTree));
	arbre->root = NULL;
	arbre->numelm = 0;
	arbre->preceed = preceed;
	arbre->viewKey = viewKey;
	arbre->viewData = viewData;
	arbre->freeKey = freeKey;
	arbre->freeData = freeData;
	arbre->balanced = 1;
}

/**
 * @brief
 * Effectuer une rotation gauche autour du nœud y.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 5 cas à considérer :
 * (+) bfactor(y)=-2 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=1
 * (+) bfactor(y)=-1 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=0
 * (+) bfactor(y)=-2 et bfactor(y->right)=-2
 * Assurez vous que le nœud y ainsi que son fils droit existent.
 */
static BSTNode* rotateLeft(BSTNode* y) {
	assert(y);
	assert(y->right);
	BSTNode* x = y->right;
	y->right = x->left;
	x->left = y;
	if (y->bfactor == -2 && y->right->bfactor == -1) {
		y->bfactor = 0;
		x->bfactor = 0;
	}
	else if (y->bfactor == -1 && y->right->bfactor == 1) {
		y->bfactor = 0;
		x->bfactor = 2;
	}
	else if (y->bfactor == -1 && y->right->bfactor == -1) {
		y->bfactor = 1;
		x->bfactor = 1;
	}
	else if (y->bfactor == -1 && y->right->bfactor == 0) {
		y->bfactor = 0;
		x->bfactor = 1;
	}
	else if (y->bfactor == -2 && y->right->bfactor == -2) {
		y->bfactor = 1;
		x->bfactor = 0;
	}
	return x;
}

/**
 * @brief
 * Effectuer une rotation droite autour du nœud x.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 5 cas à considérer :
 * (+) bfactor(x)=2 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=-1
 * (+) bfactor(x)=1 et bfactor(x->left)=0
 * (+) bfactor(x)=2 et bfactor(x->left)=2
 * Assurez vous que le nœud x ainsi que son fils gauche existent.
 */
static BSTNode* rotateRight(BSTNode* x) {
	assert(x);
	assert(x->left);
	BSTNode* y = x->left;
	x->left = y->right;
	y->right = x;
	if (x->bfactor == 2 && x->left->bfactor == 1) {
		x->bfactor = 0;
		y->bfactor = 0;
	}
	else if (x->bfactor == 1 && x->left->bfactor == 1) {
		x->bfactor = -1;
		y->bfactor = -1;
	}
	else if (x->bfactor == 1 && x->left->bfactor == -1) {
		x->bfactor = 0;
		y->bfactor = -2;
	}
	else if (x->bfactor == 1 && x->left->bfactor == 0) {
		x->bfactor = 0;
		y->bfactor = -1;
	}
	else if (x->bfactor == 2 && x->left->bfactor == 2) {
		x->bfactor = -1;
		y->bfactor = 0;
	}
	return y;
}

/**
 * @brief
 * Insérer un nouveau nœud de clé key et donnée data
 * au sous-arbre binaire de recherche équilibré raciné au nœud curr.
 * Les clés sont comparées en utilisant la fonction preceed.
 * N'oubliez pas à mettre à jour la facteur d'équilibre du nœud curr.
 * N'oubliez pas à faire les rotations nécessaires (4 cas à considérer).
 * NB : fonction récursive.
 */
static BSTNode* insertEBSTNode(BSTNode* curr, void* key, void* data, int (*preceed)(const void*, const void*)) {
	if (curr == NULL) {
		return newEBSTNode(key, data);
	}
	else {
		if (preceed(key, curr->key)) { // key à insérer < key du noeud courant alors on descend dans le fils gauche
			if (curr->left == NULL) { // si pas de fils gauche, c'est le moment de l'insérer comme feuille
				curr->left = newEBSTNode(key, data);
				curr->bfactor++; // le seul cas où on sait que la hauteur du fils gauche augmente
			}
			else // le fils gauche est un arbre qu'il faut de nouveau parcourir par un appel récursif
				curr->left = insertEBSTNode(curr->left, key, data, preceed); 
		}
		else { // key à insérer > key du noeud courant alors on descend dans le fils droit
			if (curr->right == NULL) { // si pas de fils droit, c'est le moment de l'insérer comme feuille
				curr->right = newEBSTNode(key, data);
				curr->bfactor--; // le seul cas où on sait que la hauteur du fils droit augmente
			}
			else // le fils droit est un arbre qu'il faut de nouveau parcourir par un appel récursif
				curr->right = insertEBSTNode(curr->right, key, data, preceed);
		}

		// Test des bfactor : rotations si besoin
		BSTNode* x = curr;
		if (x->bfactor == 2 && x->left->bfactor == 1)
			rotateRight(x);
		else if (x->bfactor == 2 && x->left->bfactor == -1) {
			rotateLeft(x->left);
			rotateRight(x);
		}

		BSTNode* y = curr;
		if (y->bfactor == -2 && y->right->bfactor == -1)
			rotateLeft(y);
		else if (y->bfactor == -2 && y->right->bfactor == 1) {
			rotateRight(y->right);
			rotateLeft(y);
		}

		return curr;
	}
}

/**
 * NB : Utiliser la fonction récursive insertEBSTNode.
 */
void EBSTreeInsert(BSTree* T, void* key, void* data) {
	T->root = insertEBSTNode(T->root, key, data, T->preceed);
}

/*********************************************************************
 * Fonctions communes pour :
 * Arbre Binaire de Recherche (non-équilibré), et
 * Arbre Binaire de Recherche Équilibré.
 *********************************************************************/

/**
 * @brief
 * Libérer récursivement le sous-arbre raciné au nœud curr.
 * Dans le cas où le pointeur de fonction freeKey (resp. freeData) n'est pas NULL,
 * la mémoire de la clé (resp. de la donnée) du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 */
static void freeBSTNode(BSTNode* curr, void (*freeKey)(void*), void (*freeData)(void*)) {
	if (curr != NULL) {
		freeBSTNode(curr->left, freeKey, freeData);
		freeBSTNode(curr->right, freeKey, freeData);

		if (freeKey != NULL)
			freeKey(curr->key);
		if (freeData != NULL)
			freeData(curr->data);
		free(curr);
	}
}

/**
 * NB : Utiliser la procédure récursive freeBSTNode.
 * Vous devez utiliser les bons paramètres freeKey et freeData
 * par rapport aux valeurs deleteKey et deleteData.
 */
void freeBSTree(BSTree* T, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);
	if (deleteKey == 1) {
		if (deleteData == 1)
			freeBSTNode(T->root, T->freeKey, T->freeData);
		else
			freeBSTNode(T->root, T->freeKey, NULL);
	}
	else {
		if (deleteData == 1)
			freeBSTNode(T->root, NULL, T->freeData);
		else
			freeBSTNode(T->root, NULL, NULL);
	}
	free(T);
}

/**
 * @brief
 * Afficher récursivement le sous-arbre raciné au nœud curr
 * en utilisant un ordre infixe.
 * NB : procédure récursive.
 */
static void inorderView(BSTNode *curr, void (*viewKey)(const void*), void (*viewData)(const void*)) {
	if (curr != NULL) {
		inorderView(curr->left, viewKey, viewData);
		printf("key : "); viewKey(curr->key);
		printf(", data : "); viewData(curr->data);
		printf("\n");
		inorderView(curr->right, viewKey, viewData);
	}
}

/**
 * NB : Utiliser la procédure récursive inorderView.
 */
void viewBSTree(const BSTree* T) {
	inorderView(T->root, T->viewKey, T->viewData);
}

/**
 * @brief
 * Transformer récursivement le sous-arbre raciné au nœud curr
 * à une liste doublement chaînée.
 * L'argument list est en entrée/sortie.
 * La liste va contenir les éléments de l'arbre dans l'ordre infixe.
 * NB : procédure récursive.
 */
static void treetolist(BSTNode* curr, List* list) {
	if (curr != NULL) {
		treetolist(curr->left, list);
		listInsertLast(list, curr->data);
		treetolist(curr->right, list);
	}
}

/**
 * NB : Utiliser la procédure récursive treetolist.
 */
List* BSTreeToList(const BSTree* T) {
	List* list = newList(T->viewData, T->freeData);
	treetolist(T->root, list);
	return list;
}

BSTNode* BSTMin(BSTNode* node) {
	assert(node != NULL);
	if (node->left == NULL)
		return node;
	else
		return BSTMin(node->left);
}

BSTNode* BSTMax(BSTNode* node) {
	assert(node != NULL);
	if (node->right == NULL)
		return node;
	else
		return BSTMax(node->right);
}

/**
 * @brief
 * Chercher récursivement dans le sous-arbre raciné au nœud curr
 * et renvoyer le noeud qui contient la clé qui précède la clé key (prédécesseur).
 * Le pointeur de fonction preceed donne l'ordre entre deux clés.
 * NB : fonction récursive.
 */
static BSTNode* predecessor(BSTNode* curr, void* key, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);
	BSTNode* pred;
	if (preceed(key, curr->key)) { // key <= key de curr
		if (curr->left != NULL)
			return predecessor(curr->left, key, preceed);
		else
			return NULL; // pas de predecesseur	
	}
	else { // key >= key de curr
		if (preceed(curr->key, key)) { // key > key de curr
			if (curr->right != NULL) {
				pred = predecessor(curr->right, key, preceed);
				if (preceed(pred->key, key))
					return pred;
				else 
					return NULL; // pas de predecesseur	
			}
			else
				return NULL; // pas de predecesseur	
		}
		else { // key = key de curr
			if (curr->left != NULL)
				return BSTMax(curr->left);
			else
				return NULL; // pas de predecesseur	
		}
	}
}

/**
 * NB : Utiliser la fonction récursive predecessor.
 */
BSTNode * findPredecessor(const BSTree * T, const BSTNode* node) {
	assert(T->root != NULL);
	assert(node != NULL);
	return predecessor(T->root, node->key, T->preceed);
}

/**
 * @brief
 * Chercher récursivement dans le sous-arbre raciné au nœud curr
 * et renvoyer le noeud qui contient la clé qui succède la clé key (successeur).
 * Le pointeur de fonction preceed donne l'ordre entre deux clés.
 * NB : fonction récursive.
 */
static BSTNode* successor(BSTNode* curr, void* key, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);
	BSTNode* succ;
	if (preceed(curr->key, key)) { // key > key de curr
		if (curr->right != NULL)
			return successor(curr->right, key, preceed);
		else
			return NULL; // pas de successeur	
	}
	else { // key <= key de curr
		if (preceed(key, curr->key)) { // key < key de curr
			if (curr->left != NULL) {
				succ = successor(curr->left, key, preceed);
				if (preceed(key, succ->key))
					return succ;
				else 
					return NULL; // pas de predecesseur	
			}
			else
				return NULL; // pas de successeur
		}
		else { // key = key de curr
			if (curr->right != NULL)
				return BSTMin(curr->right);
			else
				return NULL; // pas de predecesseur	
		}
	}
}

/**
 * NB : Utiliser la fonction récursive successor.
 */
BSTNode * findSuccessor(const BSTree * T, const BSTNode* node) {
	assert(T->root != NULL);
	assert(node != NULL);
	return successor(T->root, node->key, T->preceed);
}