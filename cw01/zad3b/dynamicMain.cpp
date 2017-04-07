#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/resource.h>
#include "library.h"
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>

static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;

void startClock();
void endClock();

int main() {
    void* lib = dlopen("library.so", RTLD_LAZY);

    struct List* (*createList)();
    struct ListNode* (*createListNode)(char* firstName, char* lastName, char* birthDate, char* email, char* phone, char* address);
    void (*addElementToList)(struct List* list, struct ListNode* node);
    void (*deleteListNode)(struct List* list, struct ListNode* node);
    struct ListNode* (*findListNode)(struct List* list, char* firstName, char* lastName);
    void (*sortList)(struct List* list, char* key);
    struct Tree* (*createTree)();
    struct TreeNode* (*createTreeNode)(char* firstName, char* lastName, char* birthDate,
                                       char* email, char* phone, char* address);
    void (*addElementToTree)(struct Tree* tree, struct TreeNode* node);
    void (*deleteElementFromTree)(struct Tree* tree, struct TreeNode* node);
    struct TreeNode* (*findTreeNode)(struct Tree* tree, struct TreeNode* node);
    void (*sortTree)(struct Tree* tree, char* key);

    createList = dlsym(lib, "createList");
    createListNode = dlsym(lib, "createListNode");
    addElementToList = dlsym(lib, "addElementToList");
    deleteListNode = dlsym(lib, "deleteListNode");
    findListNode = dlsym(lib, "findListNode");
    sortList = dlsym(lib, "sortList");
    createTree = dlsym(lib, "createTree");
    createTreeNode = dlsym(lib, "createTreeNode");
    addElementToTree = dlsym(lib, "addElementToTree");
    deleteElementFromTree = dlsym(lib, "deleteElementFromTree");
    findTreeNode = dlsym(lib, "findTreeNode");
    sortTree = dlsym(lib, "sortTree");

    char* data[6][10] = {{"Wojciech", "Marcin", "Tomasz", "Adam", "Marek", "Krzysztof", "Albert", "Kamil", "Andrzej", "Dawid"},
                         {"Koczy", "Kurek", "Zielinski", "Habzda", "Wrobel", "Gapinski", "Duz", "Malysz", "Lewandowsi", "Stoch"},
                         {"7.02.1961", "27.05.1991", "11.10.1993", "3.12.1970", "2.06.1973", "14.04.1944", "25.08.1980", "28.01.1999", "21.04.1971", "11.11.1969"},
                         {"fcbarca@com", "sport@pl", "facebook@com", "menshealth@com.pl", "crossfit@com", "mayheem@com.us", "agh@edu.pl", "bestmain@gmail.com", "bettermain@onet.pl", "wyspagier.org@com"},
                         {"515401122", "142254264", "985385826", "536722635", "493857483", "938483021", "847582938", "989387465", "983758462", "937584762"},
                         {"Baker Street 221", "Notthing Hill 12", "Skargi 12", "Lea 182", "Krolewska 13", "Alejandro 44", "Highbury 12", "Narutowicza 14", "Powstancow 55", "Chrobrego 11"}};
    printf("Tworzenie ksiazki adresowej (lista)\n");
    startClock();
    List* list = createList();
    endClock();

    ListNode* head = createListNode("Wojciech", "Head", "13.10.1996", "wojtek@gmail.com", "515402211", "Skargi 30");
    addElementToList(list,head);

    int i = 0;
    while(i < 1000){
        char* firstName = data[0][rand()%10];
        char* lastName = data[1][rand()%10];
        char* birthDate = data[2][rand()%10];
        char* email = data[3][rand()%10];
        char* phone = data[4][rand()%10];
        char* address = data[5][rand()%10];
        ListNode* node = createListNode(firstName, lastName, birthDate, email, phone, address);
        addElementToList(list, node);
        i++;
    }

    printf("Dodanie pojedynczego elementu (lista)\n");
    startClock();
    ListNode* tmp = createListNode("Wojciech", "Koczy", "13.10.1996", "wojtek@gmail.com", "515402211", "Skargi 30");
    addElementToList(list, tmp);
    endClock();

    printf("Usuwanie kontaktu optymistyczne (lista)\n");
    startClock();
    deleteListNode(list, list -> head);
    endClock();

    printf("Usuwanie kontaktu pesymistyczne (lista)\n");
    startClock();
    deleteListNode(list, tmp);
    endClock();

    printf("Wyszukiwanie optymistyczne (lista)\n");
    startClock();
    findListNode(list, "Wojciech", "Head");
    endClock();

    printf("Wyszukiwanie pesymistyczne (lista)\n");
    startClock();
    findListNode(list, "Wojciech", "Koczy");
    endClock();

    printf("Sortowanie/Przebudowanie (lista)\n");
    startClock();
    sortList(list, "firstName");
    endClock();

    printf("\n");

    printf("Tworzenie ksiazki adresowej (drzewo)\n");
    startClock();
    Tree* tree = createTree();
    endClock();

    TreeNode* headTree = createTreeNode("Wojciech", "Head", "13.10.1996", "wojtek@gmail.com", "515402211", "Skargi 30");
    addElementToTree(tree,headTree);

    int j = 0;
    while(j < 1000){
        char* firstName = data[0][rand()%10];
        char* lastName = data[1][rand()%10];
        char* birthDate = data[2][rand()%10];
        char* email = data[3][rand()%10];
        char* phone = data[4][rand()%10];
        char* address = data[5][rand()%10];
        TreeNode* nodeTree = createTreeNode(firstName, lastName, birthDate, email, phone, address);
        addElementToTree(tree, nodeTree);
        j++;
    }

    printf("Dodanie pojedynczego elementu (drzewo)\n");
    startClock();
    TreeNode* tmpTree = createTreeNode("Wojciech", "Koczy", "13.10.1996", "wojtek@gmail.com", "515402211", "Skargi 30");
    addElementToTree(tree, tmpTree);
    endClock();

    printf("Usuwanie kontaktu optymistyczne (drzewo)\n");
    startClock();
    deleteElementFromTree(tree, tree -> root);
    endClock();

    printf("Usuwanie kontaktu pesymistyczne (drzewo)\n");
    startClock();
    deleteElementFromTree(tree, tmpTree);
    endClock();

    printf("Wyszukiwanie optymistyczne (drzewo)\n");
    startClock();
    findTreeNode(tree, headTree);
    endClock();

    printf("Wyszukiwanie pesymistyczne (drzewo)\n");
    startClock();
    findTreeNode(tree, tmpTree);
    endClock();

    printf("Sortowanie/Przebudowanie (drzewo)\n");
    startClock();
    sortTree(tree, "firstName");
    endClock();

    return 0;
}

void startClock(){
    st_time = times(&st_cpu);
}

void endClock(){
    en_time = times(&en_cpu);

    printf("Real Time: %lf  ", (double)(en_time - st_time)/CLOCKS_PER_SEC);
    printf("User Time: %lf  ", abs((double)(en_cpu.tms_utime - st_cpu.tms_utime)/sysconf(_SC_CLK_TCK)));
    printf("System Time: %lf  ", (double)(en_cpu.tms_stime - st_cpu.tms_stime)/sysconf(_SC_CLK_TCK));
    printf("\n");
}