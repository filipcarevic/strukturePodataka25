#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int value) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node* insert(Node* root, int value) {
    if (root == NULL)
        return createNode(value);
    if (value < root->value)
        root->left = insert(root->left, value);
    else if (value > root->value)
        root->right = insert(root->right, value);
    return root;
}

Node* search(Node* root, int value) {
    if (root == NULL || root->value == value)
        return root;
    if (value < root->value)
        return search(root->left, value);
    return search(root->right, value);
}

Node* findMin(Node* root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

Node* deleteNode(Node* root, int value) {
    if (root == NULL)
        return root;

    if (value < root->value)
        root->left = deleteNode(root->left, value);
    else if (value > root->value)
        root->right = deleteNode(root->right, value);
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = findMin(root->right);
        root->value = temp->value;
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}

void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);
}

void preorder(Node* root) {
    if (root == NULL) return;
    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node* root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);
}

void levelOrder(Node* root) {
    if (root == NULL) return;
    Node* q[100];
    int f = 0, r = 0;
    q[r++] = root;

    while (f < r) {
        Node* c = q[f++];
        printf("%d ", c->value);
        if (c->left) q[r++] = c->left;
        if (c->right) q[r++] = c->right;
    }
}

int main() {
    Node* root = NULL;
    int choice, value;

    while (1) {
        printf("\n1-insert 2-search 3-delete 4-inorder 5-preorder 6-postorder 7-level 8-exit\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            scanf("%d", &value);
            root = insert(root, value);
            break;
        case 2:
            scanf("%d", &value);
            printf(search(root, value) ? "Postoji\n" : "Ne postoji\n");
            break;
        case 3:
            scanf("%d", &value);
            root = deleteNode(root, value);
            break;
        case 4:
            inorder(root);
            break;
        case 5:
            preorder(root);
            break;
        case 6:
            postorder(root);
            break;
        case 7:
            levelOrder(root);
            break;
        case 8:
            return 0;
        }
    }
}
