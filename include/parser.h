#ifndef PARSER_H
#define PARSER_H
#include "ppersistent_bst.h"

int parse_and_run(char *filename, PPersistentBST *tree);
/* Lê o arquivo de operações e as executa sobre a árvore. Cada linha do arquivo representa uma operação no formato:

- INC <valor> -> Insere o valor na árvore e cria uma nova versão;
- REM <valor> -> Remove o nó com aquele valor na árvore e cria uma nova versão;
- SUC <valor> <versão> -> Imprime o sucessor de valor em uma dada versão (ou mais recente);
- IMP <versão> -> Imprime todos os elementos em ordem crescente em uma dada versão (ou mais recente).

Linhas não reconhecidas são ignoradas.

Params:
    filename (char*): Caminho para o arquivo de entrada (.txt);
    tree (PPersistentBST*): Ponteiro para a PPBST sobre a qual as operações serão aplicadas.
    
Returns:
    (int): Resultado da execução. 0 em sucesso, 1 em caso de erro ao abrir o arquivo.*/

#endif
