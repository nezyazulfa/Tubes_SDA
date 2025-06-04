// tree.h
#ifndef TREE_H
#define TREE_H

#include "sll.h"

typedef struct TreeNode {
    Paper data;
    struct TreeNode* child;
    struct TreeNode* sibling;
} TreeNode;

TreeNode* createTreeNode(Paper data);
void addChild(TreeNode* parent, TreeNode* child);
TreeNode* findNodeById(TreeNode* root, const char* id);
void insertCitationNode(TreeNode** root, Paper data);
TreeNode* buildCitationTree(Node* paperList);
void printTreeVisual(TreeNode* root, int depth);
void freeTree(TreeNode* root);

#endif
