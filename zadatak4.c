#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node{
    int multiBase;
    int exponent;
    struct node *next;
} node;

//function for making new node
node *newNode(int multiBase, int exponent);

//function for adding new node in sorted maner 
int insertSorted(node **head, node *current);

//function for reading file
int readFile(char *fileName, node **head);

//function for adding up values with same eksponents while just putting + for all else 
void addingUp(node *head);

//function for  multiplying velue in front of base number and adding up exponents if base numbers are the same 
void multipliction(node *head);

//function for freeing a list 
void freeList(node *head);

int main(){
    node *head = NULL;
    char *fileName = "polinom.txt";

    printf("\noriginal list_____\n");
    readFile(fileName, &head);
    printf("list added up_____\n");
    addingUp(head);
    printf("list multiplied up_____\n");
    multipliction(head);
    freeList(head);

    return EXIT_SUCCESS;
}

node *newNode(int multiBase, int exponent){
    node *current = (node*)malloc(sizeof(node));
    if(!current){
        printf("Greska u alokaciji memorije za newNode! ");
        return NULL;
    }

    current->multiBase = multiBase;
    current->exponent = exponent;
    current->next = NULL;

    return current;
}

int insertSorted(node **head, node *current){
    //if it is first element in list
    if(*head==NULL){
        current->next = *head;
        *head = current;
        return EXIT_SUCCESS;
    }

    node *temp = *head;
    node *prev = NULL;
    while(temp!=NULL){
        //if it the element is with the same exponet like current(6x^6 + 12x^7 + 3x^9 + 6x^7 +)
        if(temp->exponent == current->exponent){
            temp->multiBase += current->multiBase;
            free(current);
            return EXIT_SUCCESS;
        }
        
        //if temp->multiBase manji od current->multiBase
        if(temp->multiBase < current->multiBase && prev!=NULL)
        {
            prev->next = current;
            current->next = temp;
            return EXIT_SUCCESS;
        }
        
        //if prev==NULL
        if(prev==NULL && temp->multiBase < current->multiBase){
            current->next = temp;
            *head = current;
            return EXIT_SUCCESS;
        }

        //all other posibilities -> el. is last one
        if(temp->next==NULL){
            temp->next = current;
            current->next = NULL;
            return EXIT_SUCCESS;
        }

        prev = temp;
        temp = temp->next;
    }
}

int readFile(char *fileName, node **head){
    int multiBase, exponent;

    FILE *file = fopen(fileName, "r");
    if(!file){
        printf("Greska u otvaranju filea! ");
        return EXIT_FAILURE;
    }
    
    while(fscanf(file, "%dx^%d", &multiBase, &exponent)==2){
        printf("%dx^%d\n", multiBase, exponent);
        node *current = newNode(multiBase, exponent);
        insertSorted(head, current);
    }
    puts("\n");

    return EXIT_SUCCESS;
}

void addingUp(node *head){
    node *temp = head;
    while(temp!=NULL){
        printf(" %dx^%d +", temp->multiBase, temp->exponent);
        temp = temp->next;
    }
    puts("\n");
}

void multipliction(node *head){
    node *temp = head;
    int nextToBase=1, exponent=0; 

    while(temp!=NULL)
    {
        nextToBase *= temp->multiBase;
        exponent += temp->exponent;
        temp = temp->next;
    }

    printf("%dx^%d\n", nextToBase, exponent);
}

void freeList(node *head){
    node *temp = NULL;

    while(head!=NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}
