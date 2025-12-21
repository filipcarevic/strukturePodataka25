#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct node{
    double postfix;
    struct node *next;
} node;

//creating new node
node *newNode(double postfix);

//add to stog__push
int push(node **head, node *current);

//take form stog__pop
double pop(node **head);

//read from file
int readFile(char *fileName, node **head);

double calculate(double a, double b, char operator);

//free list
void freeList(node **head);

int  main(){
    node *head = NULL;
    char *fileName = "postfix.txt";

    readFile(fileName, &head);
    freeList(&head);
    
    return EXIT_SUCCESS;
}

node *newNode(double postfix){
    node *current = (node*)malloc(sizeof(node));
    if(!current){
        printf("Greska u alokaciji memorije! ");
        return NULL;
    }

    current->postfix = postfix;
    current->next = NULL;

    return current;
}

int push(node **head, node *current){
        current->next = *head;
        *head = current;

        return EXIT_SUCCESS;        
}

double pop(node **head){
    double element = (*head)->postfix;
    node *temp = *head;

    *head = (*head)->next;
    free(temp);
    
    return element;
}

int readFile(char *fileName, node **head){
    FILE *file = fopen(fileName, "r");
    if(!file){
        printf("Failed to open file! ");
        return EXIT_FAILURE;
    }
    
    char line[256];
    while(fgets(line, sizeof(line), file)!=NULL){
        char *element = strtok(line, " \n\r");
        while(element!=NULL){
            //check weather element is  character or double and depanding on that make action 
            if(isdigit(element[0]) || (element[0]=='-' && isdigit(element[1]))){
                double value = atof(element);
                node *current = newNode(value);
                push(head, current);
            } else{
                double b = pop(head);
                double a = pop(head);
                double result = calculate(a, b, element[0]);
                node *current = newNode(result);
                push(head, current);
            }
            
            element = strtok(NULL, " \n\r");
        }
    }

    return EXIT_SUCCESS;
}

double calculate(double a, double b, char operator){
    double result;

    switch(operator){
    case '+': result = a + b; break;
    case '-': result = a - b; break;
    case '*': result = a * b; break;
    case '/': result = a / b; break;
    }

    return result;
}

void freeList(node **head){
    while(*head!=NULL){
        printf("%f\n", (*head)->postfix);
        pop(head);
    }
}
