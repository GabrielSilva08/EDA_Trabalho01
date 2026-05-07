#include <stdio.h>
#include <stdlib.h>
#include "ppersistent_bst.h"
#include "parser.h"

int main(int argc, char *argv[]){
    /* Ponto de partida do programa. Realiza as seguintes etapas:
     * 1. Valida os argumentos de linha de comando;
     * 2. Inicializa a PPBST (Partial Persistent Binary Search Tree);
     * 3. Delega a leitura e execução das operações ao módulo parser;
     * 4. Libera os recursos ao final. 
    */
    // Verifica se o caminho do arquivo de entrada (operações) foi fornecido
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_operacoes.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Inicializa a PPBST
    PPersistentBST tree;
    ppbst_init(&tree);
    // Lê e executa todas as operações do arquivo de entrada
    int status = parse_and_run(argv[1], &tree);
    // Libera toda a memória alocada pela árvore
    ppbst_destroy(&tree);
    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
