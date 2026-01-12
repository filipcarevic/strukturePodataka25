#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree{
    int value;
    struct tree *left;
    struct tree *right;
} tree;

typedef struct queue{
    struct tree *currentTreeNode;
    struct queue *next;
} queue;

typedef struct{
    struct queue *front;
    struct queue *rear;
} menager;

//functions I will need and almost all will use godforsaken recursions 
//creating new Node
tree *creatNewNode(int value);

//adding new element in tree
tree *addNode(tree *currentLevel, int value);

//printing out element in few ways 
//inorder -> Left, Root, Right
void inorder(tree *currentLevel);

//preorder -> Root, Left, Right
void preorder(tree *currentLevel);

//postorder -> Left, Right, Root
void postorder(tree *currentLevel);

//level order -> Visit Root (Level 0), then all children (Level 1), then all grandchildren (Level 2)
//think I will need queue for this one - hope not!
//Yep, queue it is 
//queue functions 
queue *newQueueNode(tree *newNode);
void enqueue(queue *newQNode, menager *m);
queue *dequeue(menager *m);

void levelOrder(menager *m, tree *root);

//finding element i guess by value 
tree *findElement(int value, tree *currentLevel);

//deleting tree
void deletingTree(tree *currentLevel);

int main(){
    menager m;
    m.front = NULL;
    m.rear = NULL;

    int value;

    printf("what will be root value: ");
    scanf("%d", &value);

    tree *root = NULL;
    root = addNode(root, value);  
    
    while(1){
        printf("write a number other than 0 which is to finish: ");
        scanf("%d", &value);

        if(value==0) break;

        addNode(root, value);
    }

    printf("inorder: ");
    inorder(root);

    printf("\npostorder: ");
    postorder(root);

    printf("\npreorder: ");
    preorder(root);

    printf("\nlevel order: ");
    levelOrder(&m, root);

    printf("\nwhat elemnt do you want to find: ");
    scanf("%d", &value);
    tree *element = findElement(value, root);
    if(element!=NULL){
        printf("\n%d is in list! ", value);
    } else{
        printf("\n%d is not in list! ", value);
    }
    

    return EXIT_SUCCESS;
}

tree *creatNewNode(int value){
    tree *newNode = (tree*)malloc(sizeof(tree));
    if(!newNode){
        printf("fault in allocating memory for new node! ");
        return NULL;
    }

    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

//If I am gonna use recursions I will have to check wether I currentLevel is final(NULL) one and if not keep going down
tree *addNode(tree *currentLevel, int value){
    if(currentLevel==NULL) return creatNewNode(value);

    if(value < currentLevel->value) currentLevel->left = addNode(currentLevel->left, value);

    else if(value > currentLevel->value) currentLevel->right = addNode(currentLevel->right, value);

    return currentLevel;
}

//for recursion will need to go to the leftest of left values than and only than go to midle value in level above than to the right value in level same as leftest value and so on 
void inorder(tree *currentLevel){
    if(currentLevel==NULL) return;

    inorder(currentLevel->left);
    printf("%d ", currentLevel->value);
    inorder(currentLevel->right);
}

void preorder(tree *currentLevel){
    if(currentLevel==NULL) return;

    printf("%d ", currentLevel->value);
    preorder(currentLevel->left);
    preorder(currentLevel->right);
}

void postorder(tree *currentLevel){
    if(currentLevel==NULL) return;

    preorder(currentLevel->left);
    preorder(currentLevel->right);
    printf("%d ", currentLevel->value);
}

queue *newQueueNode(tree *newNode){
    queue *newQNode = (queue*)malloc(sizeof(queue));
    if(!newNode){
        printf("fault in allocating memory for new Q node! ");
        return NULL;
    }

    newQNode->currentTreeNode = newNode;
    newQNode->next = NULL;

    return newQNode;
}

void enqueue(queue *newQNode, menager *m){
    if(m->front==NULL){
        m->front = m->rear = newQNode;
    } else{
        m->rear->next = newQNode;
        m->rear = newQNode;
    }
}

queue *dequeue(menager *m){
    queue *temp = m->front;
    m->front = m->front->next;

    if(m->front==NULL) m->rear = NULL;

    return temp;
}

void levelOrder(menager *m, tree *root){
    queue *first = newQueueNode(root);
    enqueue(first, m);

    while(m->front!=NULL){
        queue *temp = dequeue(m);
        printf("%d ", temp->currentTreeNode->value);

        queue *current = NULL;
        
        if(temp->currentTreeNode->left!=NULL){
            current = newQueueNode(temp->currentTreeNode->left);
            enqueue(current, m);
        }

        if(temp->currentTreeNode->right!=NULL){
            current = newQueueNode(temp->currentTreeNode->right);
            enqueue(current, m);
        } 
        
        free(temp);
    }
}

tree *findElement(int value, tree *currentLevel){
    if(currentLevel==NULL || currentLevel->value==value) return currentLevel;

    if(currentLevel->value > value) return findElement(value, currentLevel->left);

    return findElement(value, currentLevel->right);
}
