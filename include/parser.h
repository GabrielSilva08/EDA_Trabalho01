#ifndef PARSER_H
#define PARSER_H

#include "bst.h"

int parse_and_run(const char *filename, BST *tree);
/* Lê o arquivo de operações e as executa sobre a árvore. Cada linha do arquivo representa uma operação no formato:

- INC <valor> -> Insere o valor na árvore;
- REM <valor> -> Remove o nó com aquele valor na árvore;
- SUC <valor> -> Imprime o sucessor do valor;
- IMP -> Imprime todos os elementos em ordem crescente.

Linhas não reconhecidas são ignoradas.

Params:
    filename (cosnt char*): Caminho para o arquivo de entrada (.txt);
    tree (BST*): Ponteiro para a BST sobre a qual as operações serão aplicadas.
    
Returns:
    (int): Resultado da execução. 0 em sucesso, 1 em caso de erro ao abrir o arquivo.*/

#endif /* PARSER_H */
