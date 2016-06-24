//---------------------------------------------------------------
// File: TreeMain.c
// Purpose: Main file for a demonstration of a binary tree
// Programming Language: C
// Author: Dr. Rick Coleman
// Date: February, 2002
//---------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int main(void)
{
    treenode *newNode;
    char     dummy; // Used in the scanf() to to pause before continuing

    // Do initialization stuff
    create_tree();

    printf("Building tree...\n");
    // Do simple insert of 15 nodes into tree.
    // Insert with keys in the order.
    //   8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15 
    // First 5 nodes are inserted using Insert1(). Remainder using Insert2()

    // Node 1
    newNode = (treenode *)malloc(sizeof(treenode));
    newNode->key = 8;
    newNode->ivalue = 2;
    newNode->fvalue = 2.3f;
    strcpy(newNode->carray, "Node1");
    newNode->left = newNode->right = NULL;
    insert1(newNode);

    // Node 2
    // Note: Each time a new node is allocated we reuse the same pointer
    // Access to the previous node is not lost because it is not in the tree.
    newNode = (treenode *)malloc(sizeof(treenode));
    newNode->key = 4;
    newNode->ivalue = 4;
    newNode->fvalue = 3.4f;
    strcpy(newNode->carray, "Node2");
    newNode->left = newNode->right = NULL;
    insert1(newNode);

    // Node 3
    newNode = (treenode *)malloc(sizeof(treenode));
    newNode->key = 12;
    newNode->ivalue = 8;
    newNode->fvalue = 4.5f;
    strcpy(newNode->carray, "Node3");
    newNode->left = newNode->right = NULL;
    insert1(newNode);

    // Node 4
    newNode = (treenode *)malloc(sizeof(treenode));
    newNode->key = 2;
    newNode->ivalue = 16;
    newNode->fvalue = 5.6f;
    strcpy(newNode->carray, "Node4");
    newNode->left = newNode->right = NULL;
    insert1(newNode);

    // Node 5
    newNode = (treenode *)malloc(sizeof(treenode));
    newNode->key = 6;
    newNode->ivalue = 32;
    newNode->fvalue = 6.7f;
    strcpy(newNode->carray, "Node5");
    newNode->left = newNode->right = NULL;
    insert1(newNode);

    // Node 6
    // Remainder of the nodes are inserted using Insert2()
    insert2(10, 7.8f, 64, "Node6");

    // Node 7
    insert2(14, 8.9f, 128, "Node7");

    // Node 8
    insert2(1, 9.0f, 256, "Node8");

    // Node 9
    insert2(3, 0.9f, 512, "Node9");

    // Node 10
    insert2(5, 9.8f, 1024, "Node10");

    // Node 11
    insert2(7, 8.7f, 2048, "Node11");

    // Node 12
    insert2(9, 7.6f, 4096, "Node12");

    // Node 13
    insert2(11, 6.5f, 8192, "Node13");

    // Node 14
    insert2(13, 5.4f, 16384, "Node14");

    // Node 15
    insert2(15, 4.3f, 32768, "Node15");

    printf("All nodes inserted\n");

    // Print the tree
    printf("-----------------------------------------------------\n");
    print_tree();
    printf("Press Enter to continue...");
    scanf("%c", &dummy);
    printf("-----------------------------------------------------\n");

    // Find some nodes and print them
    printf("-----------------------------------------------------\n");
    printf("Testing the search function\n");
    newNode = search_tree(13);
    if(newNode != NULL)
    {
        print_one(newNode);
        free(newNode); // Remember this is a duplicate node of the one in
                       // in the tree and main() is responsible for deleting it.
    }
    else
        printf("Search key not found.\n");

    newNode = search_tree(6);
    if(newNode != NULL)
    {
        print_one(newNode);
        free(newNode);
    }
    else
        printf("Search key not found.\n");

    newNode = search_tree(1);
    if(newNode != NULL)
    {
        print_one(newNode);
        free(newNode);
    }
    else
        printf("Search key not found.\n");

    newNode = search_tree(25); // Note: there is no Key=25 in the tree
    if(newNode != NULL)
    {
        print_one(newNode);
        free(newNode);
    }
    else
        printf("Search key not found.\n");

    // Delete some nodes
    printf("-----------------------------------------------------\n");
    printf("Testing delete function\n");
    printf("-----------------------------------------------------\n");
    printf("Testing deleting a leaf...\n");
    delete(7);    // Delete a known leaf
    print_tree();
     printf("Press Enter to continue...");
    scanf("%c", &dummy);
    printf("-----------------------------------------------------\n");

    printf("-----------------------------------------------------\n");
    printf("Testing deleting a node with 2 children...\n");
    delete(12);    // Delete a node known to have 2 children
    print_tree();
    printf("Press Enter to continue...");
    scanf("%c", &dummy);
    printf("-----------------------------------------------------\n");

    printf("-----------------------------------------------------\n");
    printf("Testing deleting a node with 1 child...\n");
    delete(6);    // Delete a node known to have only 1 child
    print_tree();
    printf("Press Enter to continue...");
    scanf("%c", &dummy);
    printf("-----------------------------------------------------\n");

    printf("-----------------------------------------------------\n");
    printf("Testing trying to delete a node that is not in the tree...\n");
    delete(7);    // Delete a node that is not there
    print_tree();
    printf("Press Enter to continue...");
    scanf("%c", &dummy);
    printf("-----------------------------------------------------\n");

    printf("-----------------------------------------------------\n");
    printf("Testing deleting the root...\n");
    delete(8);    // Delete the root
    print_tree();
    printf("Done.\nPress Enter to continue...");
    scanf("%c", &dummy);

    printf("-----------------------------------------------------\n");
}


