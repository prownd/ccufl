#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

// Declare this as static so no code outside of this source
// can access it.
static treenode *root;    // Declare global pointer to root of the tree

// Prototype this function so CreateTree can call it
static void clear_tree(treenode *t); 

//--------------------------------------------
// Function: CreateTree()                                    
// Purpose: Initialize tree to empty.                        
//--------------------------------------------
void create_tree()
{
    clear_tree(root);
    root = NULL;
    return;
}

//--------------------------------------------
// Function: clear_tree()                                    
// Purpose: Perform a recursive traversal of
//        a tree destroying all nodes.  
// Note: Function is declared static so it cannot
//        be accessed from outside                      
//--------------------------------------------
static void clear_tree(treenode *t)
{
    if(t==NULL) return;  // Nothing to clear
    if(t->left != NULL) clear_tree(t->left); // Clear left sub-tree
    if(t->right != NULL) clear_tree(t->right); // Clear right sub-tree
    free(t);    // Destroy this node
    return;
}

//--------------------------------------------
// Function: is_empty()                                     
// Purpose: Return TRUE if tree is empty.                   
//--------------------------------------------
int is_empty()
{
    return(root==NULL);
}

//--------------------------------------------
// Function: DupNode()                                    
// Purpose: Duplicate a node in the tree.  This
//        is used to allow returning a complete
//        structure from the tree without giving
//        access into the tree through the pointers.               
// Preconditions: None
// Returns: Pointer to a duplicate of the node arg
// Note: Function is declared static so it cannot
//        be accessed from outside                      
//--------------------------------------------
static treenode *dup_node(treenode * t)
{
    treenode *dupNode;

    dupNode = (treenode *)malloc(sizeof(treenode));
    *dupNode = *t;    // Copy the data structure
    dupNode->left = NULL;    // Set the pointers to NULL
    dupNode->right = NULL;
    return dupNode;
}

//--------------------------------------------
// Function: SearchTree()                                    
// Purpose: Perform an iterative search of the tree and     
//        return a pointer to a treenode containing the  
//        search key or NULL if not found.               
// Preconditions: None
// Returns: Pointer to a duplicate of the node found
//--------------------------------------------
treenode *search_tree(int key)
{
    //int      value_in_tree = FALSE;
    treenode *temp;

    temp = root;
    while((temp != NULL) && (temp->key != key))
    {
        if(key < temp->key)
            temp = temp->left;  // Search key comes before this node.
        else
            temp = temp->right; // Search key comes after this node 
    }
    if(temp == NULL) return temp;    // Search key not found
    else
        return(dup_node(temp));    // Found it so return a duplicate
}

//--------------------------------------------
// Function: Insert()                                        
// Insert a new node into the tree.                
// Preconditions: None
// Returns: int (TRUE if successful, FALSE otherwise)
//--------------------------------------------
int insert1(treenode *newNode)
{
    treenode *temp;
    treenode *back;

    temp = root;
    back = NULL;

    while(temp != NULL) // Loop till temp falls out of the tree 
    {
        back = temp;
        if(newNode->key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }

    // Now attach the new node to the node that back points to 
    if(back == NULL) // Attach as root node in a new tree 
        root = newNode;
    else
    {
        if(newNode->key < back->key)
            back->left = newNode;
        else
            back->right = newNode;
    }
   return(TRUE);
}

//--------------------------------------------
// Function: Insert2()                                        
// Insert a new node into the tree.                
// Preconditions: None
// Returns: int (TRUE if successful, FALSE otherwise)
//--------------------------------------------
int insert2(int key, float f, int i, char *ca)
{
    treenode *newNode;

    // Create the new node and copy data into it
    newNode = (treenode *)malloc(sizeof(treenode));
    newNode->key = key;
    newNode->fvalue = f;
    newNode->ivalue = i;
    strcpy(newNode->carray, ca);
    newNode->left = newNode->right = NULL;

    // Call Insert1() to do the actual insertion
    return(insert1(newNode));
}

//--------------------------------------------
// Function: Delete()                                        
// Purpose: Delete a node from the tree.                    
// Preconditions: Tree contains the node to delete
// Returns: int (TRUE if successful, FALSE otherwise)                               
//--------------------------------------------
int delete(int key)
{
    treenode *back;
    treenode *temp;
    treenode *delParent;    // Parent of node to delete
    treenode *delNode;      // Node to delete

    temp = root;
    back = NULL;

    // Find the node to delete 
    while((temp != NULL) && (key != temp->key))
    {
        back = temp;
        if(key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if(temp == NULL) // Didn't find the one to delete 
    {
        printf("Key not found. Nothing deleted.\n");
        return FALSE;
    }
    else
    {
        if(temp == root) // Deleting the root 
        {
            delNode = root;
            delParent = NULL; 
        }                
        else
        {
            delNode = temp;
            delParent = back;
        }
    }

    // Case 1: Deleting node with no children or one child 
    if(delNode->right == NULL)
    {
        if(delParent == NULL)    // If deleting the root    
        {
            root = delNode->left;
            free(delNode);
            return TRUE;
        }
        else
        {
            if(delParent->left == delNode)
                delParent->left = delNode->left;
            else
                delParent->right = delNode->left;
            free(delNode);
            return TRUE;
        }
    }
    else // There is at least one child 
    {
        if(delNode->left == NULL)    // Only 1 child and it is on the right
        {
            if(delParent == NULL)    // If deleting the root    
            {
                root = delNode->right;
                free(delNode);
                return TRUE;
            }
            else
            {
                if(delParent->left == delNode)
                    delParent->left = delNode->right;
                else
                    delParent->right = delNode->right;
                free(delNode);
                return TRUE;
            }
        }
        else // Case 2: Deleting node with two children 
        {
            // Find the replacement value.  Locate the node  
            // containing the largest value smaller than the 
            // key of the node being deleted.                
            temp = delNode->left;
            back = delNode;
            while(temp->right != NULL)
            {
                back = temp;
                temp = temp->right;
            }
            // Copy the replacement values into the node to be deleted 
            delNode->key = temp->key;
            delNode->fvalue = temp->fvalue;
            delNode->ivalue = temp->ivalue;
            strcpy(delNode->carray, temp->carray);

            // Remove the replacement node from the tree 
            if(back == delNode)
                back->left = temp->left;
            else
                back->right = temp->left;
            free(temp);
            return TRUE;
        }
    }
}

//--------------------------------------------
// Function: PrintOne()                                      
// Purpose: Print data in one node of a tree.
// Preconditions: None
// Returns: void                               
//--------------------------------------------
void print_one(treenode *t)
{
    printf("%d\t%f\t%d\t%s\n", t->key, t->fvalue, t->ivalue, t->carray);
}

//--------------------------------------------
// Function: PrintAll()                                     
// Purpose: Print the tree using a recursive
//        traversal 
// Preconditions: None
// Returns: void                               
// Note: Function is declared static so it cannot
//        be accessed from outside                      
//--------------------------------------------
static void print_all(treenode *t)
{
    if(t != NULL)
    {
        print_all(t->left);
        print_one(t);
        print_all(t->right);
    }
}

//--------------------------------------------
// Function: PrintTree()                                     
// Purpose: Print the tree using a recursive
//        traversal.  This gives the user access
//        to PrintAll() without giving access to
//        the root of the tree.
// Preconditions: None
// Returns: void                               
//--------------------------------------------
void print_tree()
{
    print_all(root);
}
