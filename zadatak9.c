#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree{
    int element;
    struct tree *left;
    struct tree *right;
} tree;

// a), c)
tree *creatNewNode(int element);
tree *insert(tree *current, int element);

// b)
int replace(tree *current);
void clearTree(tree *current);

// c)
void inorder(tree *current, FILE *file);
void printFileOut(char *fileName);

int main(){
    tree *root = NULL;

    int array[10] = {2, 5, 7, 8, 11, 1, 4, 2, 3, 7};
    for(int i=0; i<9; i++){
        root = insert(root, array[i]);
    }

    char *fileName = "9thTask.txt";
    FILE *file = fopen(fileName, "w");
    if(file==NULL){
        printf("fault in opening file for writing! ");
        return EXIT_FAILURE;
    }

    inorder(root, file);
    replace(root);
    fprintf(file, "\n");
    inorder(root, file);

    fclose(file);

    printFileOut(fileName);

    clearTree(root);
    return EXIT_SUCCESS;
}

tree *creatNewNode(int element){
    tree *newNode = (tree*)malloc(sizeof(tree));
    if(newNode==NULL){
        printf("fault in allocating memory for new node! ");
        return NULL;
    }

    newNode->element = element;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

tree *insert(tree *current, int element){
    if(current==NULL) return creatNewNode(element);

    if(current->element >= element) current->right = insert(current->right, element);
    if(current->element <= element) current->left = insert(current->left, element);

    return current;
}

int replace(tree *current){
    if(current==NULL) return 0;

    int leftSum = replace(current->left);
    int rightSum = replace(current->right);
    int tempValue = current->element;

    current->element = leftSum + rightSum;

    return current->element + tempValue;
}

void clearTree(tree *current){
    if(current==NULL) return;

    clearTree(current->left);
    clearTree(current->right);

    free(current);
}

void inorder(tree *current, FILE *file){
    if(current==NULL) return;

    inorder(current->left, file);
    fprintf(file, "%d ", current->element);
    inorder(current->right, file);
}

void printFileOut(char *fileName){
    FILE *file = fopen(fileName, "r");
    if(file==NULL){
        printf("fault in opening of the file for reading! ");
        return;
    }

    char buffer[6969];

    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    fclose(file);
}
