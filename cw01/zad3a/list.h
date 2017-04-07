#ifndef LIBRARY_H
#define LIBRARY_H

typedef struct ListNode{
    struct ListNode* next;
    struct ListNode* prev;

    char* firstName;
    char* lastName;
    char* birthDate;
    char* email;
    char* phone;
    char* address;
}ListNode;

typedef struct List{
    struct ListNode* head;
    struct ListNode* tail;
}List;

typedef struct TreeNode{
    struct TreeNode* parent;
    struct TreeNode* left;
    struct TreeNode* right;

    char* firstName;
    char* lastName;
    char* birthDate;
    char* email;
    char* phone;
    char* address;
}TreeNode;

typedef struct Tree{
    struct TreeNode* root;
    char* key;
}Tree;

List* createList();
int isEmptyList(List* list);
int isEmptyListNode(ListNode* node);
int compareNodesList(ListNode* node1, ListNode* node2, char* key);
void addElementToList(List* list, ListNode* node);
void sortList(List* list, char* key);
void deleteList(List* list);
void deleteListNode(List* list, ListNode* node);
void printList(List* list);
char* compareKey(ListNode* node, char* key);
ListNode* findListNode(List* list, char* firstName, char* lastName);
ListNode* createListNode(char* firstName, char* lastName, char* birthDate,
                         char* email, char* phone, char* address);

Tree* createTree();
char* compareTreeKey(TreeNode* node, char* key);
int compareNodes(TreeNode* node1, TreeNode* node2, char* key);
void addElementToTree(Tree* tree, TreeNode* node);
void deleteElementFromTree(Tree* tree, TreeNode* node);
void sortTree(Tree* tree, char* key);
void deleteAllNodes(TreeNode* node);
void deleteTree(Tree* tree);
void printData(TreeNode* node);
void printTreeNode(TreeNode* node);
void printTree(Tree* tree);
TreeNode* findTreeNode(Tree* tree, TreeNode* node);
TreeNode* minTree(TreeNode* node);
TreeNode* createTreeNode(char* firstName, char* lastName, char* birthDate,
                         char* email, char* phone, char* address);

#endif