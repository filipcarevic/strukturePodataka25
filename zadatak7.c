#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char name[64];
    struct Node* parent;
    struct Node* child;
    struct Node* sibling;
} Node;

Node* createNode(const char* name, Node* parent) {
    Node* n = (Node*)malloc(sizeof(Node));
    strcpy(n->name, name);
    n->parent = parent;
    n->child = NULL;
    n->sibling = NULL;
    return n;
}

void addDir(Node* current, const char* name) {
    Node* newDir = createNode(name, current);
    if (current->child == NULL)
        current->child = newDir;
    else {
        Node* temp = current->child;
        while (temp->sibling != NULL)
            temp = temp->sibling;
        temp->sibling = newDir;
    }
}

Node* changeDir(Node* current, const char* name) {
    Node* temp = current->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->sibling;
    }
    printf("Direktorij ne postoji\n");
    return current;
}

void listDir(Node* current) {
    Node* temp = current->child;
    if (temp == NULL) {
        printf("Direktorij je prazan\n");
        return;
    }
    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->sibling;
    }
}

int main() {
    Node* root = createNode("C:", NULL);
    Node* current = root;
    int choice;
    char name[64];

    while (1) {
        printf("\n1-md 2-cd dir 3-cd.. 4-dir 5-izlaz\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            scanf("%s", name);
            addDir(current, name);
            break;
        case 2:
            scanf("%s", name);
            current = changeDir(current, name);
            break;
        case 3:
            if (current->parent != NULL)
                current = current->parent;
            break;
        case 4:
            listDir(current);
            break;
        case 5:
            return 0;
        }
    }
}
