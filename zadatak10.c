#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// For part A: 
typedef struct _cityTree{
    char name[69];
    int population;
    struct _cityTree *left, *right;
} CityTree;

typedef struct _countryList{
    char name[69];
    CityTree *root;
    struct _countryList *next;
} CountryList;

// For part B: 
typedef struct _cityList{
    char name[69];
    int population;
    struct _cityList *next;
} CityList;

typedef struct _countryTree{
    char name[69];
    CityList *cityHead;
    struct _countryTree *left, *right;
} CountryTree;

//PART A FUNCTIONS

CityTree* createCityNode(char* name, int pop){
    CityTree* n = (CityTree*)malloc(sizeof(CityTree));
    if(!n){
        printf("fault in allocatnig memory! \n");
        return NULL;
    }

    strcpy(n->name, name);
    n->population = pop;
    n->left = n->right = NULL;

    return n;
}

CityTree* insertCityTree(CityTree* r, char* name, int pop){
    if (!r) return createCityNode(name, pop);
    if (pop < r->population || (pop == r->population && strcmp(name, r->name) < 0))
        r->left = insertCityTree(r->left, name, pop);
    else
        r->right = insertCityTree(r->right, name, pop);

    return r;
}

void insertCountryList(CountryList** head, char* name, CityTree* root){
    CountryList* n = (CountryList*)malloc(sizeof(CountryList));
    if(!n){
        printf("fault in allocatnig memory! \n");
        return;
    }

    strcpy(n->name, name);
    n->root = root;
    if (!*head || strcmp(name, (*head)->name) < 0) {
        n->next = *head;
        *head = n;
        return;
    }

    CountryList* t = *head;
    while (t->next && strcmp(name, t->next->name) > 0) t = t->next;
    n->next = t->next;
    t->next = n;
}

void printCityTree(CityTree* r) {
    if (!r) return;

    printCityTree(r->left);
    printf("\t%s, %d\n", r->name, r->population);
    printCityTree(r->right);
}

void printPartA(CountryList* head) {    
    while (head) {
        printf("%s:\n", head->name);
        printCityTree(head->root);
        head = head->next;
    }
}

//PART B FUNCTIONS

CityList* insertCityList(CityList* head, char* name, int pop){
    CityList* n = (CityList*)malloc(sizeof(CityList));
    if(!n){
        printf("fault in allocatnig memory! \n");
        return NULL;
    }

    strcpy(n->name, name);
    n->population = pop;

    if (!head || pop < head->population || (pop == head->population && strcmp(name, head->name) < 0)) {
        n->next = head;
        return n;
    }

    CityList* t = head;
    while (t->next && (t->next->population < pop || (t->next->population == pop && strcmp(name, t->next->name) < 0)))
        t = t->next;
    n->next = t->next;
    t->next = n;

    return head;
}

CountryTree* insertCountryTree(CountryTree* r, char* name, CountryTree** target){
    if (!r) {
        *target = (CountryTree*)malloc(sizeof(CountryTree));
        strcpy((*target)->name, name);
        (*target)->cityHead = NULL;
        (*target)->left = (*target)->right = NULL;
        return *target;
    }

    if (strcmp(name, r->name) < 0) r->left = insertCountryTree(r->left, name, target);
    else if (strcmp(name, r->name) > 0) r->right = insertCountryTree(r->right, name, target);
    else *target = r;

    return r;
}

void printFilteredCities(CityTree* r, int limit){
    if (!r) return;
    printFilteredCities(r->left, limit);

    if (r->population > limit) printf("\t%s (%d)\n", r->name, r->population);
    printFilteredCities(r->right, limit);
}

void findPartA(CountryList* head, char* target, int limit){
    while (head && strcmp(head->name, target) != 0) head = head->next;

    if (head) printFilteredCities(head->root, limit);
    else printf("Country not found.\n");
}

void readData(char* file, CountryList** headA, CountryTree** rootB){
    FILE* f = fopen(file, "r");
    if(!f){
        printf("Could not open file %s\n", file);
        return;
    }

    char cName[69], fName[69], cityName[69];
    int pop;

    while (fscanf(f, "%s %s", cName, fName) == 2) {
        FILE* cf = fopen(fName, "r");
        if (!cf) {
            printf("Could not open file %s\n", fName);
            continue;
        }

        CityTree* rootA = NULL;
        CountryTree* nodeB = NULL;
        *rootB = insertCountryTree(*rootB, cName, &nodeB);

        while (fscanf(cf, " %[^,], %d", cityName, &pop) == 2) {
            rootA = insertCityTree(rootA, cityName, pop);
            nodeB->cityHead = insertCityList(nodeB->cityHead, cityName, pop);
        }


        insertCountryList(headA, cName, rootA);
        fclose(cf);
    }
    fclose(f);
}

int main(){
    CountryList* headA = NULL;
    CountryTree* rootB = NULL;

    readData("drzave.txt", &headA, &rootB);
    printPartA(headA);
    
    char searchC[69];
    int minPop;

    printf("Enter country and min population: ");
    scanf("%s %d", searchC, &minPop);
    findPartA(headA, searchC, minPop);

    return 0;
}