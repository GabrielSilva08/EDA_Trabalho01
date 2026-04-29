# Trabalho 01 - Estrutura de Dados Avançadas

## Persistência Parcial em uma Árvore Binária de Busca

O código fonte implementa persistência parcial para o caso de uma árvore binária de busca (*Binary Search Tree*, BST). Como estrutura de dados, essa pode ser implementada seguindo o modelo de computação da máquina de ponteiros, onde cada nó é apontado por nó máximo 1 ponteiro ($p = 1$). O código segue uma implementação limpa, modular e comentada de uma BST escrita em **C padrão (C11)**, com suporte a inserção, remoção, busca de sucessor numa dada versão da estrutura e impressão da árvore numa dada versão com os valores ordenados juntamente com suas respectivas profundidades (ou níveis).

---

## Sumário

- [Linguagem e versão](#linguagem-e-versão)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Como compilar e executar](#como-compilar-e-executar)
- [Formato do arquivo de entrada](#formato-do-arquivo-de-entrada)
- [Estruturas de dados](#estruturas-de-dados)
- [Funções](#funções)
- [Exemplo de uso](#exemplo-de-uso)

---

## Linguagem e versão (atualizar)

| Item       | Valor                          |
|------------|-------------------------------|
| Linguagem  | C                              |
| Padrão     | C11 (`-std=c11`)               |
| Compilador | GCC ≥ 9.0 (ou Clang ≥ 10.0)   |
| Build tool | GNU Make                       |

---

## Estrutura do projeto (atualizar)

```
bst/
├── main.c       # Ponto de entrada — valida argumentos e orquestra o fluxo
├── bst.h        # Interface pública da BST (estruturas + assinaturas)
├── bst.c        # Implementação completa da BST
├── parser.h     # Interface do módulo de leitura de operações
├── parser.c     # Leitura do arquivo e despacho de operações
├── Makefile     # Automação de compilação e execução
├── input.txt    # Arquivo de entrada de exemplo
└── README.md    # Este arquivo
```

---

## Como compilar e executar (atualizar)

### Compilar

```bash
gcc -c src\main.c -I include\*.h -o build\main.o
gcc -c src\bst.c -I include\*.h -o build\bst.o
gcc -c src\parser.c -I include\*.h -o build\parser.o
```

Isso gera os executáveis auxiliares pro programa final na pasta `build\`.

```bash
gcc build\*.o -o bin\program.exe
```

### Executar

```bash
bin\program.exe <arquivo_com_operações_na_BST.txt>
```

Exemplo com o arquivo de entrada incluído:

```bash
bin\program.exe input\input.txt
```

### Compilar e executar em um único comando (atualizar)

```bash
make run                        # usa input.txt por padrão
make run INPUT=outro_arquivo.txt
```

### Limpar os arquivos gerados

```bash
make clean
```

---

## Formato do arquivo de entrada (atualizar)

O arquivo de entrada é um `.txt` com **uma operação por linha**:

| Operação | Formato        | Descrição                                                             | Saída                                              |
|----------|----------------|-----------------------------------------------------------------------|----------------------------------------------------|
| Inserção | `INC <valor>`  | Insere o inteiro na árvore. Duplicatas são permitidas.                | Nenhuma                                            |
| Remoção  | `REM <valor>`  | Remove o nó com aquele valor. Se inexistente, nada acontece.          | Nenhuma                                            |
| Sucessor | `SUC <valor>`  | Encontra o menor valor da árvore estritamente maior que `<valor>`.    | Linha 1: `SUC <valor>` / Linha 2: resultado        |
| Imprimir | `IMP`          | Imprime todos os elementos em ordem crescente com sua profundidade.   | Linha 1: `IMP` / Linha 2: `val,prof val,prof ...` |

> **Nota sobre `SUC`:** O valor de referência **não precisa** existir na árvore. Caso não exista sucessor, é impresso `NULL`. (atualizar para INF)

> **Nota sobre `IMP`:** A profundidade da raiz é **0**. O separador entre elementos é um espaço simples.

---

## Estruturas de dados

### `Node` — `bst.h`

Representa um único nó da árvore.

```c
typedef struct Node {
    int value;          // Valor inteiro armazenado
    struct Node *left;  // Filho esquerdo (valores menores)
    struct Node *right; // Filho direito  (valores maiores)
} Node;
```

### `BST` — `bst.h`

Contêiner que encapsula o ponteiro para a raiz. Permite que funções modifiquem a raiz sem precisar retornar ponteiros adicionais.

```c
typedef struct {
    Node *root; // Ponteiro para o nó raiz
} BST;
```

---

## Funções (atualizar)

### Módulo `bst.c` / `bst.h`

| Função | Assinatura | Arquivo | Descrição |
|--------|-----------|---------|-----------|
| `bst_init` | `void bst_init(BST *tree)` | `bst.c` | Inicializa uma BST vazia (`root = NULL`). |
| `bst_insert` | `void bst_insert(BST *tree, int value)` | `bst.c` | Insere um valor respeitando a propriedade BST. Duplicatas são ignoradas. |
| `bst_remove` | `void bst_remove(BST *tree, int value)` | `bst.c` | Remove o nó com o valor dado. Trata os três casos clássicos (folha, um filho, dois filhos). |
| `bst_successor` | `Node *bst_successor(const BST *tree, int value)` | `bst.c` | Retorna o nó com o menor valor estritamente maior que `value`, ou `NULL`. |
| `bst_print_inorder` | `void bst_print_inorder(const BST *tree)` | `bst.c` | Imprime `valor,profundidade` de cada nó em ordem crescente. |
| `bst_destroy` | `void bst_destroy(BST *tree)` | `bst.c` | Libera toda a memória da árvore (travessia pós-ordem). |

#### Funções internas (static — não expostas no header)

| Função | Arquivo | Descrição |
|--------|---------|-----------|
| `node_create` | `bst.c` | Aloca e inicializa um novo nó. |
| `node_insert` | `bst.c` | Inserção recursiva em subárvore. |
| `node_min` | `bst.c` | Encontra o nó de menor valor (extremo esquerdo). |
| `node_remove` | `bst.c` | Remoção recursiva em subárvore. |
| `node_print_inorder` | `bst.c` | Travessia in-order recursiva com impressão. |
| `node_successor` | `bst.c` | Busca recursiva do sucessor com rastreamento de candidato. |
| `node_destroy` | `bst.c` | Liberação recursiva de subárvore (pós-ordem). |

---

### Módulo `parser.c` / `parser.h`

| Função | Assinatura | Arquivo | Descrição |
|--------|-----------|---------|-----------|
| `parse_and_run` | `int parse_and_run(const char *filename, BST *tree)` | `parser.c` | Abre o arquivo, lê linha a linha e despacha cada operação para o módulo `bst`. Retorna `0` em sucesso ou `1` em erro de abertura. |

---

### `main.c`

Contém apenas a função `main`, que:
1. Valida que exatamente um argumento foi passado (caminho do arquivo).
2. Inicializa a BST com `bst_init`.
3. Chama `parse_and_run` para processar o arquivo.
4. Libera os recursos com `bst_destroy`.
5. Retorna `EXIT_SUCCESS` ou `EXIT_FAILURE` conforme o resultado.

---

## Exemplo de uso

Dado o arquivo `input.txt`:

```
INC 50
INC 42
INC 65
INC 13
INC 52
IMP
SUC 50
REM 42
IMP
SUC 60
REM 50
IMP
```

Saída esperada:

```
IMP
13,2 42,1 50,0 52,2 65,1
SUC 50
52
IMP
13,2 50,0 52,1 65,1
SUC 60
65
IMP
13,1 52,0 65,1
```

> A profundidade de cada nó pode variar conforme a ordem de inserção e remoções anteriores.
