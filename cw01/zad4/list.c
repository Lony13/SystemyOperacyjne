#include "library.h"
#include <stdlib.h>
#include <stdio.h>

List* createList(){
    List* newList = (List *) malloc(sizeof(List));
    if(newList == NULL)
        exit(EXIT_FAILURE);

    newList -> head = newList -> tail = NULL;
    return newList;
}

int isEmptyList(List* list){
    if(list == NULL){
        return 1;
    } else{
        return 0;
    }
}

int isEmptyListNode(ListNode* node){
    if(node == NULL){
        return 1;
    } else{
        return 0;
    }
}

int compareNodesList(ListNode* node1, ListNode* node2, char* key){
    char* node1Data = compareTreeKey(node1, key);
    char* node2Data = compareTreeKey(node2, key);
    return strcmp(node1Data, node2Data);
}

void addElementToList(List* list, ListNode* node){
    if(isEmptyList(list) == 1){
        printf("This list is empty");
        return;
    }

    if(isEmptyListNode(node) == 1){
        printf("This node is empty");
        return;
    }

    if(list -> head == NULL){
        list -> head = list -> tail = node;
    } else{
        list -> tail -> next = node;
        node -> prev = list -> tail;
        list -> tail = node;
    }
}

char* compareKey(ListNode* node, char* key){
    switch (key[0]){
        case 'f':
            return node -> firstName;
        case 'l':
            return node -> lastName;
        case 'b':
            return node -> birthDate;
        case 'e':
            return node -> email;
        case 'p':
            return node -> phone;
        case 'a':
            return node -> address;
    }
}

void sortList(List* list, char* key){
    if(isEmptyList(list) == 1 || list -> head -> next == NULL){
        return;
    }

    List* tmpList = createList();
    ListNode* tmp;
    ListNode* min;
    tmp = min = list -> head;

    while(list -> head != NULL){
        while(tmp != NULL){
            if (compareNodesList(tmp, min, key) < 0)
                min = tmp;
            tmp = tmp -> next;
        }

        if(list -> head == min && list -> tail == min){
            list -> head = list -> tail = NULL;
        } else if(list -> head == min){
            list -> head = min -> next;
            list -> head -> prev = NULL;
            min -> next = NULL;
        } else if(list -> tail == min){
            list -> tail = min -> prev;
            list -> tail -> next = NULL;
            min -> prev = NULL;
        } else{
            min -> prev -> next = min -> next;
            min -> next -> prev = min -> prev;
            min -> next = min -> prev = NULL;
        }

        addElementToList(tmpList, min);

        tmp = min = list -> head;
    }

    list -> head = tmpList -> head;
    list -> tail = tmpList -> tail;
    tmpList -> head = tmpList -> tail = NULL;
    free(tmpList);
}

void deleteList(List* list){
    if(isEmptyList(list) == 1){
        return;
    }

    ListNode* tmp = list -> head;

    while(tmp != NULL){
        tmp = tmp -> next;
        free(tmp -> prev);
    }

    free(list);
}

void deleteListNode(List* list, ListNode* node){
    if(isEmptyList(list) == 1){
        printf("This list is empty");
        return;
    }

    if(isEmptyListNode(node) == 1){
        printf("This node is empty");
        return;
    }

    ListNode* tmp = list -> head;
    while(tmp != NULL && tmp != node){
        tmp = tmp -> next;
    }

    if(tmp == NULL){
        printf("This node is not a member of this list");
        return;
    }

    if(list -> head == tmp && list -> tail == tmp){
        list -> head = list -> tail = NULL;
    } else if(list -> head == tmp){
        list -> head = tmp -> next;
        list -> head -> prev = NULL;
        tmp -> next = NULL;
    } else if(list -> tail == tmp){
        list -> tail = tmp -> prev;
        list -> tail -> next = NULL;
        tmp -> prev = NULL;
    } else{
        tmp -> prev -> next = tmp -> next;
        tmp -> next -> prev = tmp -> prev;
        tmp -> next = tmp -> prev = NULL;
    }

    free(node);
}

void printList(List* list){
    if(isEmptyList(list) == 1){
        printf("This list is empty");
        return;
    }

    ListNode* tmp = list -> head;
    while(tmp != NULL){
        printf(tmp -> firstName);
        printf(" ");
        printf(tmp -> lastName);
        printf(" ");
        printf(tmp -> birthDate);
        printf(" ");
        printf(tmp -> email);
        printf(" ");
        printf(tmp -> phone);
        printf(" ");
        printf(tmp -> address);
        printf("\n");

        tmp = tmp -> next;
    }
}

ListNode* findListNode(List* list, char* firstName, char* lastName){
    if(isEmptyList(list) == 1){
        printf("This list is empty");
        return NULL;
    }

    ListNode* tmp = list -> head;
    while(tmp != NULL){
        if((strcmp(tmp -> lastName, lastName) == 0) && (strcmp(tmp -> firstName, firstName) == 0))
            break;
        tmp = tmp -> next;
    }
    return tmp;
}

ListNode* createListNode(char* firstName, char* lastName, char* birthDate,
                         char* email, char* phone, char* address){
    ListNode* newNode = (ListNode *) malloc(sizeof(ListNode));
    if(newNode == NULL)
        exit(EXIT_FAILURE);

    newNode -> firstName = firstName;
    newNode -> lastName = lastName;
    newNode -> birthDate = birthDate;
    newNode -> email = email;
    newNode -> phone = phone;
    newNode -> address = address;

    newNode -> next = NULL;
    newNode -> prev = NULL;

    return newNode;
}





Tree* createTree(){
    Tree* newTree = (Tree *) malloc(sizeof(Tree));
    if(newTree == NULL)
        exit(EXIT_FAILURE);

    newTree -> root = NULL;
    newTree -> key = "lastName";
    return newTree;
}

TreeNode* minTree(TreeNode* node){
    if(node -> left != NULL)
        return minTree(node -> left);
    else
        return node;
}

TreeNode* createTreeNode(char* firstName, char* lastName, char* birthDate,
                         char* email, char* phone, char* address){
    TreeNode* newNode = (TreeNode *) malloc(sizeof(TreeNode));
    if(newNode == NULL)
        exit(EXIT_FAILURE);

    newNode -> firstName = firstName;
    newNode -> lastName = lastName;
    newNode -> birthDate = birthDate;
    newNode -> email = email;
    newNode -> phone = phone;
    newNode -> address = address;

    newNode -> parent = NULL;
    newNode -> right = NULL;
    newNode -> left = NULL;

    return newNode;
}

char* compareTreeKey(TreeNode* node, char* key){
    switch (key[0]){
        case 'f':
            return node -> firstName;
        case 'l':
            return node -> lastName;
        case 'b':
            return node -> birthDate;
        case 'e':
            return node -> email;
        case 'p':
            return node -> phone;
        case 'a':
            return node -> address;
    }
}

void addElementToTree(Tree* tree, TreeNode* node){
    if(tree == NULL){
        printf("The tree does not exist");
        return;
    }

    if(tree -> root == NULL){
        tree -> root = node;
        return;
    }

    TreeNode* jumper;
    TreeNode* prev;
    jumper = tree -> root;
    prev = NULL;

    while(jumper != NULL) {
        prev = jumper;
        (compareNodesTree(node, jumper, tree->key) > 0) ? (jumper = jumper->right) : (jumper = jumper->left);
    }
    (compareNodesTree(node, prev, tree -> key) > 0) ? (prev -> right = node) : (prev -> left = node);
    node -> parent = prev;
}

void deleteElementFromTree(Tree* tree, TreeNode* node){
    if(node -> left == NULL && node -> right == NULL){
        if(node -> parent == NULL){
            tree -> root = NULL;
        } else if(node -> parent -> left == node){
            node -> parent -> left = NULL;
        } else{
            node -> parent -> right = NULL;
        }
    } else if(node -> left == NULL || node -> right == NULL){
        if(node -> left == NULL){
            if(node -> parent == NULL){
                tree -> root = node -> right;
            } else if(node -> parent -> left == node){
                node -> parent -> left = node -> right;
            } else{
                node -> parent -> right = node -> right;
            }
            node -> right -> parent = node -> parent;
        } else{
            if(node -> parent == NULL){
                tree -> root = node -> left;
            } else if(node -> parent -> left == node){
                node -> parent -> left = node -> left;
            } else{
                node -> parent -> right = node -> left;
            }
            node -> left -> parent = node -> parent;
        }
    } else{
        TreeNode* tmp = minTree(node -> right);
        TreeNode* tmp2 = createTreeNode(node -> firstName, node->lastName, node->birthDate, node->email, node->phone, node->address);

        node -> firstName = tmp -> firstName;
        node -> lastName = tmp -> lastName;
        node -> birthDate = tmp -> birthDate;
        node -> email = tmp -> email;
        node -> phone = tmp -> phone;
        node -> address = tmp -> address;

        tmp -> firstName = tmp2 -> firstName;
        tmp -> lastName = tmp2 -> lastName;
        tmp -> birthDate = tmp2 -> birthDate;
        tmp -> email = tmp2 -> email;
        tmp -> phone = tmp2 -> phone;
        tmp -> address = tmp2 -> address;

        deleteElementFromTree(tree, tmp);
    }
}

void sortTree(Tree* tree, char* key){
    if(tree == NULL)
        return;

    Tree* newTree = createTree();
    newTree -> key = key;
    TreeNode* tmp;

    while(tree -> root != NULL){
        tmp = tree -> root;
        tmp = createTreeNode(tmp -> firstName, tmp -> lastName, tmp -> birthDate, tmp -> email, tmp -> phone, tmp -> address);
        addElementToTree(newTree, tmp);
        deleteElementFromTree(tree, tree -> root);
    }

    tree -> root = newTree -> root;
    tree -> key = key;
    free(newTree);
    return;
}

void deleteAllNodes(TreeNode* node){
    if(node != NULL){
        deleteAllNodes(node -> left);
        deleteAllNodes(node -> right);
        free(node);
    }
}

void deleteTree(Tree* tree){
    deleteAllNodes(tree -> root);
    tree -> root = NULL;
    free(tree);
}

void printData(TreeNode* node) {
    if (node != NULL){
        printf(node -> firstName);
        printf(" ");
        printf(node -> lastName);
        printf(" ");
        printf(node -> birthDate);
        printf(" ");
        printf(node -> email);
        printf(" ");
        printf(node -> phone);
        printf(" ");
        printf(node -> address);
        printf("\n");
    }
}

void printTreeNode(TreeNode* node){
    if(node != NULL){
        printTreeNode(node -> left);
        printData(node);
        printTreeNode(node -> right);
    }
}

void printTree(Tree* tree){
    if(tree -> root != NULL){
        printTreeNode(tree -> root);
    }
}

int compareNodesTree(TreeNode* node1, TreeNode* node2, char* key){
    char* node1Data = compareTreeKey(node1, key);
    char* node2Data = compareTreeKey(node2, key);
    return strcmp(node1Data, node2Data);
}

TreeNode* findTreeNode(Tree* tree, TreeNode* node){
    if(tree == NULL)
        return NULL;

    TreeNode* tmp = tree -> root;
    int flaga = 0;

    while(tmp != NULL && flaga == 0){
        if(compareNodesTree(tmp, node, tree -> key) < 0)
            tmp = tmp -> right;
        else
            (compareNodesTree(tmp, node, tree -> key) == 0) ? (flaga = 1) : (tmp = tmp -> left);
    }
    return tmp;
}