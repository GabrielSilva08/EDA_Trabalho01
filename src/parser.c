#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ppersistent_bst.h"

// Tamanho máximo de uma linha no arquivo de entrada
#define MAX_LINE 256
 
int parse_and_run(char *filename, PPersistentBST *tree){
    char line[MAX_LINE];
    FILE *fp = fopen(filename, "r");
 
    if (!fp){
        fprintf(stderr, "Erro: não foi possível abrir o arquivo \"%s\".\n", filename);
        return EXIT_FAILURE;
    }
 
    while (fgets(line, sizeof(line), fp)){
        // Remoção do caractere '\n', se houver
        line[strcspn(line, "\n")] = '\0';
        
        // INC <valor>
        if (strncmp(line, "INC", 3) == 0){
            int value = atoi(line + 4);
            ppbst_insert(tree, value);

        // REM <valor>
        } else if (strncmp(line, "REM", 3) == 0){
            int value = atoi(line + 4);
            ppbst_remove(tree, value);

        // SUC <valor> <versão>
        } else if (strncmp(line, "SUC", 3) == 0){
            int value, version;
            sscanf(line+4, "%d %d", &value, &version);
            // Impressão do nome da operação
            printf("SUC %d %d\n", value, version);
            ppbst_successor(tree, value, version);

        // IMP <versão>
        } else if (strncmp(line, "IMP", 3) == 0) {
            int version = atoi(line + 4);
            printf("IMP %d\n", version);
            ppbst_print_inorder(tree, version);
        }
        // Operações não reconhecidas são ignoradas
    }
    fclose(fp);
    return EXIT_SUCCESS;
 }
