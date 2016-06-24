#ifndef _TREE_H_
#define _TREE_H_
#include <stdio.h>

typedef struct treenode_s
{
    int                 key;
    float               fvalue;
    int                 ivalue;
    char                carray[7];
    struct treenode_s *left;
    struct treenode_s *right;
}treenode;

void create_tree();
int is_empty();
treenode *search_tree(int key);
int insert1(treenode *newnode);
int insert2(int key, float f, int i, char *carr);
int delete(int key);
void print_one(treenode *t);
void print_tree();

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#endif




