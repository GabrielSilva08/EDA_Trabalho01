#ifndef PPERSISTENT_BST_H
#define PPERSISTENT_BST_H
#define MAX_VERSIONS 100

/* Estruturas necessárias para a implementação */ 

/*
 * Enumeração dos possíveis campos em que um nó pode ser alterado ao longo das operações.
*/
typedef enum {
    NONE,
    LEFT,
    RIGHT,
    VALUE,
    ROOT
} Field;

typedef struct Node Node;

/* 
 * Registro de uma modificação aplicada a um nó em determinada versão.
 *
 * Params:
 *  version (int): Versão da modificação;
 *  mod_field (Field): Campo em que a modificação é feita;
 *  mod_value (int): Valor da modificação;
 *  mod_ptr (Node*): Ponteiro da modificação.
*/
typedef struct {
    int version;
    Field mod_field;
    int mod_value;
    Node *mod_ptr;
} Mod;

/*
 * Nó constituente da árvore binária de busca parcialmente persistente (PPBST).
 * 
 * Params:
 *  left (Node*): Filho esquerdo;
 *  right (Node*): Filho direito;
 *  value (int): Valor do nó (chave);
 *  born_version (int): Versão que o nó foi criado;
 *  is_root (int): Flag usada para especificar se o nó é raíz da árvore.
 *  mods (Mod[2]): Vetor de modificações.
*/
struct Node {
    Node *left;
    Node *right;
    int value;
    int born_version;
    int is_root;                                        
    Mod mods[2];
};

/* 
 * Árvore binária de busca parcialmente persistente.
 * 
 * Params:
 * 
 *  live_root (Node*): Raíz ativa (versão mais recente);
 *  node_pool (Node*): Pool de nós utilizados pela árvore (necessário para liberação de memória);
 *  node_count (int): Quantidade de nós da estrutura;
 *  versions (Node*): version[i] representa a raíz na versão i;
 *  version_count (int): Quantidade de versões criadas até o momento;
 *  current_version (int): Versão atual da estrutura (mais recente).
*/
typedef struct {
    Node *live_root;
    Node *node_pool[MAX_VERSIONS];
    int node_count;
    Node *versions[MAX_VERSIONS];
    int version_count;       
    int current_version;         
} PPersistentBST;

/* Operações disponíveis sobre a PPBST (públicos ao parser) */

/*
 * Realiza a inicialização da PPBST passada por referência.
 * 
 * Params:
 *  tree (PPersistentBST*): Árvore a qual se deseja fazer uso.
*/
void ppbst_init(PPersistentBST *tree);

/*
 * Cria uma nova versão da estrutura com a inserção do nó cuja chave é igual a value na PPBST tree.
 * 
 * Params:
 *  tree (PPersistentBST*): Árvore a qual se deseja realizar a inserção;
 *  value (int): Chave do nó a ser inserido.
*/
void ppbst_insert(PPersistentBST *tree, int value);

/*
 * Cria uma nova versão da estrutura com a remoção do nó cuja chave é igual a value na PPBST tree.
 * 
 * Params:
 *  tree (PPersistentBST*): Árvore a qual se deseja realizar a remoção;
 *  value (int): Chave do nó a ser removido.
*/
void ppbst_remove(PPersistentBST *tree, int value);

/*
 * Imprime no terminal o menor inteiro maior que "value" em "tree" na versão "version". Caso esse nó não exista, é imprimido "INF" (infinito).
 * 
 * Params:
 *  tree (PPersistentBST*): PPBST a qual se deseja procurar pelo sucessor;
 *  value (int): Valor pelo qual buscamos seu sucessor (não necessariamente um nó com esse valor existe em "tree"!)
 *  version (int): Versão de interesse.
*/
void ppbst_successor(PPersistentBST *tree, int value, int version);

/* 
 * Percorre a PPBST tree na versão "version" seguindo o percuso em-ordem.
 *
 * Params:
 *  tree (PPersistentBST*): PPBST a qual se deseja imprimir no terminal;
 *  version (int): Versão de interesse.
*/
void ppbst_print_inorder(PPersistentBST *tree, int version);

/*
 * Realiza a desalocação da PPBST passada por referência.
 * 
 * Params:
 *  tree (PPersistentBST*): Árvore a qual se deseja liberar da memória.
*/
void ppbst_destroy(PPersistentBST *tree);

#endif
