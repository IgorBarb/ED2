#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de um nó da árvore AVL
struct Node {
    int id;               // ID do cliente (chave de busca)
    char name[50];        // Nome do cliente
    struct Node *left;
    struct Node *right;
    int height;
};

// Função para criar um novo nó
struct Node* newNode(int id, char name[]) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->id = id;
    strcpy(node->name, name);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // novo nó é inicialmente uma folha
    return node;
}

// Função para obter a altura de um nó
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Função para obter o valor máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Rotação à direita
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Rotação à esquerda
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Obter o fator de balanceamento do nó
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Função para inserir um novo nó na árvore AVL
struct Node* insert(struct Node* node, int id, char name[]) {
    if (node == NULL)
        return newNode(id, name);

    if (id < node->id)
        node->left = insert(node->left, id, name);
    else if (id > node->id)
        node->right = insert(node->right, id, name);
    else  // IDs duplicados não são permitidos na árvore AVL
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && id < node->left->id)
        return rightRotate(node);

    if (balance < -1 && id > node->right->id)
        return leftRotate(node);

    if (balance > 1 && id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Função para encontrar o menor valor na subárvore de um nó
struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Função para remover um nó da árvore AVL
struct Node* removeNode(struct Node* root, int id) {
    if (root == NULL)
        return root;

    if (id < root->id)
        root->left = removeNode(root->left, id);
    else if (id > root->id)
        root->right = removeNode(root->right, id);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            struct Node* temp = minValueNode(root->right);
            root->id = temp->id;
            strcpy(root->name, temp->name);
            root->right = removeNode(root->right, temp->id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Função para exibir a árvore AVL em ordem
void inOrder(struct Node *root) {
    if(root != NULL) {
        inOrder(root->left);
        printf("ID: %d, Nome: %s\n", root->id, root->name);
        inOrder(root->right);
    }
}

// Função auxiliar para imprimir a árvore AVL em formato indentado (pré-ordem)
void printTree(struct Node *root, int space) {
    if (root == NULL)
        return;

    space += 10;
    printTree(root->right, space);
    
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->id);
    
    printTree(root->left, space);
}

// Função para encontrar o menor valor na árvore
struct Node* findMin(struct Node* root) {
    struct Node* current = root;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Função para encontrar o maior valor na árvore
struct Node* findMax(struct Node* root) {
    struct Node* current = root;
    while (current->right != NULL)
        current = current->right;
    return current;
}

// Função principal
int main() {
    struct Node *root = NULL;

    // Inserção de nós como exemplo
    root = insert(root, 10, "Alice");
    root = insert(root, 70, "Grace");
    root = insert(root, 20, "Bob");
    root = insert(root, 50, "Eve");    
    root = insert(root, 30, "Charlie");
    root = insert(root, 40, "Diana");
    root = insert(root, 60, "Frank");
    root = insert(root, 90, "Igor");
    root = insert(root, 85, "Paulo");

    printf("Árvore AVL em ordem:\n");
    inOrder(root);

    printf("\nRepresentação da árvore AVL:\n");
    printTree(root, 0);

    printf("\nRemovendo o nó com ID 20:\n");
    root = removeNode(root, 20);
    inOrder(root);
    printf("\nRepresentação da árvore AVL após remoção:\n");
    printTree(root, 0);
    
    printf("\nRemovendo o nó com ID 85:\n");
    root = removeNode(root, 85);
    inOrder(root);
    printf("\nRepresentação da árvore AVL após remoção:\n");
    printTree(root, 0);


    struct Node *minNode = findMin(root);
    struct Node *maxNode = findMax(root);
    printf("\nMenor ID: %d, Nome: %s\n", minNode->id, minNode->name);
    printf("Maior ID: %d, Nome: %s\n", maxNode->id, maxNode->name);

    return 0;
}
