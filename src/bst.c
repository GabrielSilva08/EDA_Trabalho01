#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/* Funções auxiliares internas (static) */

static Node *node_create(int value){
/* Aloca e inicializa um novo nó com valor de chave igual a value.

Params:
    value (int): Valor a armazenar no nó.

Returns:
    (Node*): Ponteiro de nó com valor de chave igual a value.
*/
    Node *node = (Node*) malloc(sizeof(Node));
    if (!node){
        fprintf(stderr, "Erro: falha ao alocar memória para nó.\n");
        return NULL;
    }
    node->value = value;
    node->left  = NULL;
    node->right = NULL;
    return node;
}


static Node *node_insert(Node *node, int value){
/* Insere recursivamente nó de valor value a partir da subárvore raizada em node.

Params:
    node (Node*): Raiz da subárvore atual;
    value (int): Valor a ser inserido.

Returns:
    (Node*): Raiz da subárvore após a inserção.

*/
    // Caso base: Inserção na raíz da BST
    if (node == NULL){
        return node_create(value);
    }

    if (value < node->value){
        // Inserção na subárvore esquerda
        node->left = node_insert(node->left, value);
    } else if (value >= node->value) {
        // Inserção na subárvore direita
        node->right = node_insert(node->right, value);
    }

    return node;
}


static Node *node_min(Node *node){
/* Encontra o nó com o menor valor em uma subárvore. método auxiliar utilizado internamente na remoção para localizar o sucessor quando o nó removido possui filhos.

Params:
    node (Node*): Raíz da subárvore que se deseja encontrar o nó de menor valor (não pode ser NULL);

Returns:
    (Node*): Ponteiro para o nó de menor valor.

*/
    // Retorno do valor localizado mais à esquerda da subárvore
    while (node->left != NULL){
        node = node->left;
    }
    return node;
}

/**
 * @brief Remove recursivamente um nó com o valor especificado.
 *
 * Casos tratados:
 *  1. Nó folha         → removido diretamente.
 *  2. Um filho         → substituído pelo filho existente.
 *  3. Dois filhos      → substituído pelo sucessor in-order.
 *
 * @param node  Raiz da subárvore atual.
 * @param value Valor do nó a remover.
 * @return      Raiz da subárvore após a remoção.
 */
static Node *node_remove(Node *node, int value){
/* Remove recursivamente um nó com o valor especificado.

Params:
    node (Node*): Raíz da subárvore atual;
    value (int): Valor do nó a ser removido.

Returns:
    (Node*): Raiz da subárvore após a remoção.
*/
    if (node == NULL){
        // Valor não encontrado
        return NULL;
    }

    if (value < node->value){
        // Remoção na subárvore esquerda
        node->left = node_remove(node->left, value);

    } else if (value > node->value){
        // Remoção na subárvore direita
        node->right = node_remove(node->right, value);

    } else{
        // Primeira ocorrência do valor encontrado
        if (node->left == NULL && node->right == NULL){
            // Caso 1: nó folha. Simplesmente libera
            free(node);
            return NULL;

        } else if (node->left == NULL){
            // Caso 2a: Apenas filho direito
            Node *temp = node->right;
            free(node);
            return temp;

        } else if (node->right == NULL){
            // Caso 2b: Apenas filho esquerdo 
            Node *temp = node->left;
            free(node);
            return temp;

        } else{
            // Caso 3: Dois filhos. Substitui o valor pelo sucessor e remove esse sucessor da direita.
            Node *successor = node_min(node->right);
            node->value     = successor->value;
            node->right     = node_remove(node->right, successor->value);
        }
    }
    return node;
}


static void node_print_inorder(const Node *node, int depth, int *first){
/* Percorre a subárvore em-ordem (in-order) imprimindo cada nó. Ou seja, percorremos em ordem crescente por valor.

Params:
    node (Node*): Raíz da subárvore atual;
    depth (int): Profundidade do nó atual (raíz = 0);
    first (int*): Ponteiro para flag que controla o separador de espaço.
*/
    if (node == NULL){
        return;
    }

    node_print_inorder(node->left, depth + 1, first);

    /* Imprime separador antes de cada elemento (exceto o primeiro) */
    if (!(*first)){
        printf(" ");
    }
    *first = 0;
    // Imprime "valor,profundidade"
    printf("%d,%d", node->value, depth);

    node_print_inorder(node->right, depth + 1, first);
}


static Node *node_successor(Node *node, int value, Node *candidate){
/* Busca o sucessor de um valor percorrendo a árvore. Mantém um ponteiro para o candidato mais recente a sucessor (menor valor encontrado ainda maior que o buscado).

Params:
    node (Node*): Raíz da subárvore atual;
    value (int): Valor de referência (que se deseja encontrar o sucessor);
    candidate (Node*): Melhor candidato a sucessor encontrado até agora.

Returns:
    (Node*): Melhor candidato atualizado.
*/
    if (node == NULL){
        return candidate;
    }

    if (node->value > value){
        // Este nó pode ser o sucessor atualiza candidato
        candidate = node;
        // Pode haver um valor ainda menor que seja maior que 'value'
        return node_successor(node->left, value, candidate);
    } else{
        // o sucessor deve estar à direita
        return node_successor(node->right, value, candidate);
    }
}


static void node_destroy(Node *node){
/* Libera recursivamente todos os nós de uma subárvore. Os nós são liberados seguindo um percurso em pós-ordem.

Params:
    node (Node*):Raíz da subárvore a destruir.
*/
    if (node == NULL) {
        return;
    }
    node_destroy(node->left);
    node_destroy(node->right);
    free(node);
}

/* Funções públicas (disponíveis ao parser) */

void bst_init(BST *tree) {
    tree->root = NULL;
}

void bst_insert(BST *tree, int value) {
    tree->root = node_insert(tree->root, value);
}

void bst_remove(BST *tree, int value) {
    tree->root = node_remove(tree->root, value);
}

Node *bst_successor(const BST *tree, int value) {
    // Inicia a busca sem nenhum candidato
    return node_successor(tree->root, value, NULL); // Atualizar para inf
}

void bst_print_inorder(const BST *tree) {
    int first = 1; // Flag para evitar espaço antes do primeiro elemento
    node_print_inorder(tree->root, 0, &first);
    printf("\n");
}

void bst_destroy(BST *tree) {
    node_destroy(tree->root);
    tree->root = NULL;
}
