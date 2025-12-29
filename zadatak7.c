#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//from the begging 
//I need to write code that allowes infinite directories that are connected in tree structure
//first one is root(first/og parent), than comes first child that can either point to the next sibling that is in same level as him 
//or can point to his first child which will now make him a parent -> firstChild is for depth, nextSibling is for width
//that sequence can repete idenfiinitely 
//stack  is used to remeber last directory(parent or previouse child)
//task can also be done by having parent node inside of struct

typedef struct directory{
    char name[69];
    struct directory *firstChild;
    struct directory *nextSibling;
} dir;

typedef struct stack{
    dir *dirAddress;
    struct stack *next;
} stack;

dir *creatNewNode(char *name);

//making new directory inside of current one
void md(dir *newNode, dir *currentDir);

//moving to next directory inside of current if there is one 
dir *cd(dir *currentDir, char *name);

void printTree(dir* current, int level);

//stack functoins
//for push I need current directory and somewhere I will save that directory by name
stack *makeNewStackNode(dir *currentDirectory);
void push(stack *current, stack **head);
dir *pop(stack **head);

void cleanDirectory(dir *parent, stack **head);

int main(){
    dir *root = creatNewNode("c");
    dir *currentDir = root;
    char name[69];
    char opereation[5];
    stack *head = NULL;

    while(1){
        //seting up input
        printTree(currentDir, 0);

        printf("\n$ ");
        gets(name);
        sscanf(name, "%s %s", opereation, name);

        name[strcspn(name, "\r\n")] = '\0';
        opereation[strcspn(opereation, "\r\n")] = '\0';

        //______________________________
        if(strcmp(opereation, "break")==0){
            cleanDirectory(currentDir, &head);

            return EXIT_SUCCESS;
        }
    
        if(strcmp(opereation, "md")==0){
            dir *newNode = creatNewNode(name);
            md(newNode, currentDir);
        }
        
        if(strcmp(opereation, "cd") == 0) {
            dir *target = cd(currentDir, name);

            if(target != currentDir) {
                stack *newStackNode = makeNewStackNode(currentDir); 
                push(newStackNode, &head);
                currentDir = target;
            }
        }
        
        if(strcmp(opereation, "cd..") == 0){
        dir *previous = pop(&head);

        if(previous!=NULL) currentDir = previous;
        }
    }
}

dir *creatNewNode(char *name){
    dir *newNode = (dir*)malloc(sizeof(dir));
    if(!newNode){
        printf("fault in alocating memory for new node! ");
        return NULL;
    }
    strcpy(newNode->name, name);
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    return newNode;
}

void md(dir *newNode, dir *currentDir){
    if(currentDir->firstChild==NULL){
        currentDir->firstChild = newNode;
    } else{
        dir *temp = currentDir->firstChild;

        while(temp->nextSibling!=NULL){
            temp = temp->nextSibling;
        }
        temp->nextSibling = newNode;
    }
}

dir *cd(dir *currentDir, char *name){
    dir *temp = currentDir->firstChild;

    while(temp!=NULL){
        if(strcmp(temp->name, name)==0){
            return temp;
        }
        temp = temp->nextSibling;
    }
    printf("cd: %s: No such file or directory\n", name);
    return currentDir;
}

void printTree(dir* current, int level) {
    if (current == NULL) return;

    for (int i = 0; i < level; i++) printf("  ");
    printf("%s\n", current->name);

    printTree(current->firstChild, level + 1);

    printTree(current->nextSibling, level);
}

stack *makeNewStackNode(dir *currentDirectory){
    stack *current = (stack*)malloc(sizeof(stack));
    if(!current){
        printf("fault in alocating memory for current! ");
        return NULL;
    }

    current->dirAddress = currentDirectory;
    current->next = NULL;

    return current;
}

void push(stack *current, stack **head){
    current->next = *(head);
    *(head) = current;
}

dir *pop(stack **head){
    if (*head == NULL) return NULL;

    stack *temp = *head;
    dir *address = temp->dirAddress; 
    
    *head = temp->next;
    free(temp);

    return address;
}

void cleanDirectory(dir *parent, stack **head){
    while(parent!=NULL){
        dir *prev = pop(head);
        free(prev);
    }
}
