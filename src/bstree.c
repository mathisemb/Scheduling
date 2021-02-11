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
	/* A FAIRE */
}

BSTree * newBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {
	/* A FAIRE */
}

/**
 * @brief
 * Insérer un nouveau nœud de clé key et donnée data
 * au sous-arbre binaire de recherche (non-équilibré) raciné au nœud curr.
 * Les clés sont comparées en utilisant la fonction preceed.
 * NB : fonction récursive.
 */
static BSTNode* insertBSTNode(BSTNode* curr, void* key, void* data, int (*preceed)(const void*, const void*)) {
	/* A FAIRE */
}

/**
 * NB : Utiliser la fonction récursive insertBSTNode.
 */
void BSTreeInsert(BSTree* T, void* key, void* data) {
	/* A FAIRE */
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
	/* A FAIRE */
}

BSTree * newEBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {
	/* A FAIRE */
}

/**
 * @brief
 * Effectuer une rotation gauche autour du nœud y.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 4 cas à considérer :
 * (+) bfactor(y)=-2 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=1
 * (+) bfactor(y)=-1 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=0
 * Assurez vous que le nœud y ainsi que son fils droit existent.
 */
static BSTNode* rotateLeft(BSTNode* y) {
	assert(y);
	assert(y->right);
	/* A FAIRE */
}

/**
 * @brief
 * Effectuer une rotation droite autour du nœud x.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 4 cas à considérer :
 * (+) bfactor(x)=2 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=-1
 * (+) bfactor(x)=1 et bfactor(x->left)=0
 * Assurez vous que le nœud x ainsi que son fils gauche existent.
 */
static BSTNode* rotateRight(BSTNode* x) {
	assert(x);
	assert(x->left);
	/* A FAIRE */
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
	/* A FAIRE */
}

/**
 * NB : Utiliser la fonction récursive insertEBSTNode.
 */
void EBSTreeInsert(BSTree* T, void* key, void* data) {
	/* A FAIRE */
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
	/* A FAIRE */
}

/**
 * NB : Utiliser la procédure récursive freeBSTNode.
 * Vous devez utiliser les bons paramètres freeKey et freeData
 * par rapport aux valeurs deleteKey et deleteData.
 */
void freeBSTree(BSTree* T, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);
	/* A FAIRE */
}

/**
 * @brief
 * Afficher récursivement le sous-arbre raciné au nœud curr
 * en utilisant un ordre infixe.
 * NB : procédure récursive.
 */
static void inorderView(BSTNode *curr, void (*viewKey)(const void*), void (*viewData)(const void*)) {
	/* A FAIRE */
}

/**
 * NB : Utiliser la procédure récursive inorderView.
 */
void viewBSTree(const BSTree* T) {
	/* A FAIRE */
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
	/* A FAIRE */
}

/**
 * NB : Utiliser la procédure récursive treetolist.
 */
List* BSTreeToList(const BSTree* T) {
	/* A FAIRE */
}

BSTNode* BSTMin(BSTNode* node) {
	assert(node != NULL);
	/* A FAIRE */
}

BSTNode* BSTMax(BSTNode* node) {
	assert(node != NULL);
	/* A FAIRE */
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
	/* A FAIRE */
}

/**
 * NB : Utiliser la fonction récursive predecessor.
 */
BSTNode * findPredecessor(const BSTree * T, const BSTNode* node) {
	assert(T->root != NULL);
	assert(node != NULL);
	/* A FAIRE */
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
	/* A FAIRE */
}

/**
 * NB : Utiliser la fonction récursive successor.
 */
BSTNode * findSuccessor(const BSTree * T, const BSTNode* node) {
	assert(T->root != NULL);
	assert(node != NULL);
	/* A FAIRE */
}