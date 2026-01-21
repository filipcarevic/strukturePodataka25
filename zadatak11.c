#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 69
#define HASH_SIZE 11

typedef struct _cityTree {
    char name[MAX_NAME];
    int population;
    struct _cityTree *left, *right;
} CityTree;

typedef struct _countryNode {
    char name[MAX_NAME];
    CityTree *cityRoot;
    struct _countryNode *next;
} CountryNode;

int calculateHash(char* name);
CityTree* insertCityTree(CityTree* r, char* name, int pop);
void insertIntoTable(CountryNode** table, char* countryName, CityTree* cityRoot);
void printTable(CountryNode** table);
void searchTable(CountryNode** table, char* target, int limit);

int calculateHash(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++) {
        sum += (int)name[i];
    }
    return sum % HASH_SIZE;
}

CityTree* createCityNode(char* name, int pop) {
    CityTree* n = (CityTree*)malloc(sizeof(CityTree));
    if (!n) return NULL;
    strcpy(n->name, name);
    n->population = pop;
    n->left = n->right = NULL;
    return n;
}

CityTree* insertCityTree(CityTree* r, char* name, int pop) {
    if (!r) return createCityNode(name, pop);
    if (pop < r->population || (pop == r->population && strcmp(name, r->name) < 0))
        r->left = insertCityTree(r->left, name, pop);
    else
        r->right = insertCityTree(r->right, name, pop);
    return r;
}

//HASH TABLE
void insertIntoTable(CountryNode** table, char* countryName, CityTree* cityRoot) {
    int index = calculateHash(countryName);
    
    CountryNode* n = (CountryNode*)malloc(sizeof(CountryNode));
    if (!n) return;
    strcpy(n->name, countryName);
    n->cityRoot = cityRoot;

    // Insert into sorted linked list at table[index]
    if (!table[index] || strcmp(countryName, table[index]->name) < 0) {
        n->next = table[index];
        table[index] = n;
        return;
    }

    CountryNode* t = table[index];
    while (t->next && strcmp(countryName, t->next->name) > 0) 
        t = t->next;
    
    n->next = t->next;
    t->next = n;
}

void printCityInOrder(CityTree* r) {
    if (!r) return;
    printCityInOrder(r->left);
    printf("\t%s (%d)\n", r->name, r->population);
    printCityInOrder(r->right);
}

void printTable(CountryNode** table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        if (table[i]) {
            printf("\nBucket [%d]:\n", i);
            CountryNode* t = table[i];
            while (t) {
                printf("  Country: %s\n", t->name);
                printCityInOrder(t->cityRoot);
                t = t->next;
            }
        }
    }
}

void printFilteredCities(CityTree* r, int limit) {
    if (!r) return;
    printFilteredCities(r->left, limit);
    if (r->population > limit) printf("\t%s (%d)\n", r->name, r->population);
    printFilteredCities(r->right, limit);
}

void searchTable(CountryNode** table, char* target, int limit) {
    int index = calculateHash(target);
    CountryNode* t = table[index];
    
    while (t && strcmp(t->name, target) != 0) 
        t = t->next;

    if (t) {
        printf("Cities in %s with population > %d:\n", t->name, limit);
        printFilteredCities(t->cityRoot, limit);
    } else {
        printf("Country %s not found in bucket %d.\n", target, index);
    }
}

void readData(char* file, CountryNode** table) {
    FILE* f = fopen(file, "r");
    if (!f) return;

    char cName[MAX_NAME], fName[MAX_NAME], cityName[MAX_NAME];
    int pop;

    while (fscanf(f, "%s %s", cName, fName) == 2) {
        FILE* cf = fopen(fName, "r");
        if (!cf) continue;

        CityTree* cityRoot = NULL;
        while (fscanf(cf, " %[^,], %d", cityName, &pop) == 2) {
            cityRoot = insertCityTree(cityRoot, cityName, pop);
        }
        insertIntoTable(table, cName, cityRoot);
        fclose(cf);
    }
    fclose(f);
}

int main() {
    CountryNode* hashTable[HASH_SIZE] = { NULL }; 

    readData("drzave.txt", hashTable);
    printTable(hashTable);

    char searchC[MAX_NAME];
    int minPop;
    printf("\nEnter country and min population to search: ");
    if (scanf("%s %d", searchC, &minPop) == 2) {
        searchTable(hashTable, searchC, minPop);
    }

    return 0;
}