#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    //new->lower_than = lower_than;
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if (tree == NULL || key == NULL || value == NULL) return;
    if (tree->root == NULL)
    {
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
    }
    else
    {
        TreeNode *aux = tree->root;
        while (aux != NULL)
            {
                if (is_equal(tree, key, aux->pair->key) == 1)
                {
                    return;
                }
                else
                {
                    if (tree->lower_than(key, aux->pair->key) == 1)
                    {
                        if (aux->left == NULL)
                        {
                            TreeNode *new = createTreeNode(key, value);
                            new->parent = aux;
                            aux->left = new;
                            tree->current = new;
                            return;
                        }
                        else
                        {
                            aux = aux->left;
                        }
                        
                    }
                    else
                    {
                        if (aux->right == NULL)
                        {
                            TreeNode *new = createTreeNode(key, value);
                            new->parent = aux;
                            aux->right = new;
                            tree->current = new;
                            return;
                        }
                        
                    }
                }
            }
    }

}

TreeNode * minimum(TreeNode * x)
{
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    if (tree == NULL || node == NULL) return;
    if (tree->root == NULL) return;
    if (tree->root == node)
    {
        tree->root = NULL;
        tree->current = NULL;
        return;
    }
    else
    {
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        else
        {
            node->parent->right = NULL;
        }
    }
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        else
        {
            node->parent->right = NULL;
        }
    }
    if(tree->root->left != NULL && tree->root->right != NULL)
    {
        TreeNode *aux = minimum(tree->root->right);
        tree->root->pair->key = aux->pair->key;
        tree->root->pair->value = aux->pair->value;
        removeNode(tree, aux);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode* aux = tree->root;
    while (aux != NULL)
    {
        if (is_equal(tree, key, aux->pair->key) == 1)
        {
            tree->current = aux;
            return aux->pair;
        }
        else if (tree->lower_than(key, aux->pair->key) == 1)
        {
            aux = aux->left;
        }
        else
        {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
    TreeNode* current = tree->root;
    Pair* resultado = NULL;
    while (current != NULL)
    {
        if (is_equal(tree, key, current->pair->key) == 1)
        {
            return current->pair;
        }
        else if (tree->lower_than(key, current->pair->key) == 1)
        {
            resultado = current->pair;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return resultado;
}

Pair * firstTreeMap(TreeMap * tree) 
{
    TreeNode *aux = tree->root;
    if (aux == NULL) return NULL;
    while (aux->left != NULL)
    {
        aux = aux->left;
    }
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    TreeNode *aux = tree->current;
    if (aux == NULL) return NULL;
    if (aux->right != NULL)
    {
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    }
    if(aux->parent != NULL)
    {
        while (aux->parent != NULL && aux->parent->right == aux)
        {
            aux = aux->parent;
        }
        tree->current = aux->parent;
        if (aux->parent != NULL)
        {
            return aux->parent->pair;
        }
    }
    return NULL;
}
