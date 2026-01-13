#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//general functions for BST tree

typedef struct tree{
    int element;
    struct tree *left;
    struct tree *right;
} tree;

tree *creatNewTreeNode(int element);
tree *addElement(tree *current, int element);
void inorder(tree *current);
void preorder(tree *current);
void postorder(tree *current);
void deleteTree(tree *current);
tree *findElement(tree *current, int element);

//functions I need to make enqueue/dequeue for level order 

typedef struct node{
    struct tree *currentTreeNode;
    struct node *next;
} node; //forgot to call it queue but it is queue called node :)

typedef struct{
    struct node *front;
    struct node *rear;
} manager;

node *creatNewNode(tree *currentElement);
void enqueue(manager *m, node *current);
node *dequeue(manager *m);
void levelOrder(manager *m, tree *root); 
void freeQueue(manager *m);

int main(){
    manager m; m.front = NULL; m.rear = NULL;
    tree *root = NULL;
    int element;

    while(1){
        printf("input the element: ");
        scanf("%d", &element);
        if(element==-1) break;

        if(root==NULL){
            root = addElement(root, element);
        } else{
            addElement(root, element);
        }
    }

    printf("\n____inorder____\n");
    inorder(root);

    printf("\n____prerder____\n");
    preorder(root);

    printf("\n____postorder____\n");
    postorder(root);

    printf("\n____level order____\n");
    levelOrder(&m, root);

    printf("\n\nwhat element do you want to find: ");
    scanf("%d", &element);
    tree *found = findElement(root, element);
    if(found!=NULL){
        printf("%d is in tree!\n", element);
    } else{
        printf("%d is not in tree!\n", element);
    }

    deleteTree(root);

    return EXIT_FAILURE;
}

tree *creatNewTreeNode(int element){
    tree *newTreeNode = (tree*)malloc(sizeof(tree));
    if(newTreeNode==NULL){
        printf("Fault in allocating memory for new tree node! ");
        return NULL;
    }

    newTreeNode->element = element;
    newTreeNode->left = NULL;
    newTreeNode->right = NULL;

    return newTreeNode;
}

//left when current value is smaller than element
tree *addElement(tree *current, int element){
    if(current==NULL) return creatNewTreeNode(element);
    if(current->element > element) current->left = addElement(current->left, element);
    if(current->element < element) current->right = addElement(current->right, element);

    return current;

}

//Left, Root, Right
void inorder(tree *current){
    if(current==NULL) return;

    inorder(current->left);
    printf("%d ", current->element);
    inorder(current->right);
}

//Root, Left, Right
void preorder(tree *current){
    if(current==NULL) return;

    printf("%d ", current->element);
    inorder(current->left);
    inorder(current->right);
}

//Left, Right, Root
void postorder(tree *current){
    if(current==NULL) return;

    inorder(current->left);
    inorder(current->right);
    printf("%d ", current->element);

}

void deleteTree(tree *current){
    if(current==NULL) return;

    deleteTree(current->left);
    deleteTree(current->right);

    free(current);
}

tree *findElement(tree *current, int element){
    if(current==NULL || current->element==element) return current;

    if(current->element > element) current = findElement(current->left, element);
    else current = findElement(current->right, element);

    return current;
}

node *creatNewNode(tree *currentElement){
    node *newQueueNode = (node*)malloc(sizeof(node));
    if(newQueueNode==NULL){
        printf("fault in allocatiing memory for the new queue node! ");
        return NULL;
    }

    newQueueNode->currentTreeNode = currentElement;
    newQueueNode->next = NULL;

    return newQueueNode;
}

void enqueue(manager *m, node *current){
    if(m->front==NULL){
        m->front = m->rear = current;
    }else{
        m->rear->next = current;
        m->rear = current;
    }
}

node *dequeue(manager *m){
    if(m->front==NULL){
        m->rear=NULL;
        return NULL;
    }

    node *temp = m->front;
    m->front = m->front->next;

    return temp;
}

//Visit Root (Level 0), then all children (Level 1), then all grandchildren (Level 2)
void levelOrder(manager *m, tree *root){
    node *first = creatNewNode(root);
    enqueue(m, first);
    
    while(m->front!=NULL){
        node *current = dequeue(m);
        printf("%d ", current->currentTreeNode->element);

        if(current->currentTreeNode->left!=NULL) enqueue(m, creatNewNode(current->currentTreeNode->left));

        if(current->currentTreeNode->right!=NULL) enqueue(m, creatNewNode(current->currentTreeNode->right));

        free(current);
    }
}