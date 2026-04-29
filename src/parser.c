#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "bst.h"

// Tamanho máximo de uma linha no arquivo de entrada
#define MAX_LINE 256

int parse_and_run(const char *filename, BST *tree){
/* Realiza o parsing das operações passadas no arquivo de entrada sobre a árvore passada. Realiza as seguintes operações:
    1. Abre o arquivo de entrada;
    2. lê linha; 
    3. executa operação reconhecida sobre a BST fornecida;
    4. Repete os passos 2 e 3 até chegar no fim de arquivo.

Params:
    filename (const char*): Caminho do arquivo de entrada;
    tree (BST*): Ponteiro para a BST.

Returns:
    (int): 0 caso o arquivo tenha sido lido com sucesso, 1 caso contrário.

*/
    char line[MAX_LINE];
    FILE *fp = fopen(filename, "r");

    if (!fp){
        fprintf(stderr, "Erro: não foi possível abrir o arquivo \"%s\".\n", filename);
        return 1;
    }

    while (fgets(line, sizeof(line), fp)){
        // Remoção do caractere '\n', se houver
        line[strcspn(line, "\n")] = '\0';

        // INC <valor>
        if (strncmp(line, "INC ", 4) == 0){
            int value = atoi(line + 4);
            bst_insert(tree, value);

        // REM <valor>
        } else if (strncmp(line, "REM ", 4) == 0){
            int value = atoi(line + 4);
            bst_remove(tree, value);

        // SUC <valor>
        } else if (strncmp(line, "SUC ", 4) == 0){
            int value = atoi(line + 4);

            // Impressão do nome da operação
            printf("SUC %d\n", value);

            Node *suc = bst_successor(tree, value);

            if (suc != NULL){
                printf("%d\n", suc->value);
            } else {
                // Nenhum elemento é estritamente maior que 'value'
                printf("INF\n");
            }

        // IMP
        } else if (strcmp(line, "IMP") == 0) {
            printf("IMP\n");
            bst_print_inorder(tree);
        }
        // Operações não reconhecidas são ignoradas
    }
    fclose(fp);
    return 0;
}
