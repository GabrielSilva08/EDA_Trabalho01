#ifndef BST_H
#define BST_H

/* Estruturas */

typedef struct Node {
    int value;          
    struct Node *left; 
    struct Node *right;
} Node;
/* Nó individual da BST.
 
Attrs:
    value (int): Valor de chave do nó
    left (Node*): Ponteiro para o filho esquerdo
    right (Node*): Ponteiro para o filho direito
*/


typedef struct {
    Node *root; 
} BST;
/*Estrutura definidora da BST.

Attrs:
    root (Node*): Nó raiz da BST.
*/


/* Funções públicas disponíveis*/

void bst_init(BST *tree);
/* Inicializa uma BST vazia.

Params:
    tree (BST*): Ponteiro para a BST a ser inicializada.
*/


void bst_insert(BST *tree, int value);
/* Insere um valor na BST.

Params:
    tree (BST*): Ponteiro para a BST;
    value (int): Valor inteiro a inserir.
*/


void bst_remove(BST *tree, int value);
/* Remove um nó com o valor especificado da BST. Se o valor não existir, a árvore permanece inalterada.

Params:
    tree (BST*): Ponteiro para a BST;
    value (int): Valor do nó a ser removido.
*/


Node *bst_successor(const BST *tree, int value);
/* Encontra o sucesso do valor dado na BST.

Params:
    tree (BST*): Ponteiro para a BST;
    value (int): Valor de referência.

Returns:
    (Node*): Ponteiro para o nó sucessor, ou NULL se não existir.
*/


void bst_print_inorder(const BST *tree);
/* Imprime todos os elementos em ordem crescente com suas profundidades seguindo o padrão (valor,profundidade).

Params:
    tree (const BST*): Ponteiro para a BST.
*/

void bst_destroy(BST *tree);
/* Libera toda a memória alocada pela BST

Params:
    tree (BST*): Ponteiro para a BST a ser destruída.
*/

#endif
