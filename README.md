# Trabalho 01 - Estrutura de Dados Avançadas

## Persistência Parcial em uma Árvore Binária de Busca

O código fonte implementa persistência parcial para o caso de uma árvore binária de busca (*Binary Search Tree*, BST). Como estrutura de dados, essa pode ser implementada seguindo o modelo de computação da máquina de ponteiros, onde cada nó é apontado por nó máximo 1 ponteiro ($p = 1$). O código segue uma implementação limpa, modular e comentada de uma BST escrita em **C padrão (C11)**, com suporte a inserção, remoção, busca de sucessor numa dada versão da estrutura e impressão da árvore numa dada versão com os valores ordenados juntamente com suas respectivas profundidades (ou níveis).

---

## Sumário

- [Linguagem e versão](#linguagem-e-versão)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Como compilar e executar](#como-compilar-e-executar)
- [Formato do arquivo de entrada](#formato-do-arquivo-de-entrada)
- [Divisão dos módulos do projeto](#divisão-dos-módulos-do-projeto)
- [Exemplo de uso](#exemplo-de-uso)

---

## Linguagem e versão

| Item       | Valor                          |
|------------|--------------------------------|
| Linguagem  | C                              |
| Padrão     | C11 (`-std=c11`)               |
| Compilador | GCC = 13.2.0                   |
| Build tool | GNU Make                       |

---

## Estrutura do projeto

```text
EDA_Trabalho01/
├── bin/                     # Contém o executável final gerado após a compilação do projeto.
│   └── program.exe           
├── build/                   # Armazena os arquivos objetos produzidos durante o processo de compilação.
│   ├── main.o
│   ├── parser.o
│   └── ppersistent_bst.o
├── include/                 # Contém os arquivos de cabeçalho utilizados no projeto.
│   ├── parser.h
│   └── ppersistent_bst.h
├── input/                   # Contém os arquivos de entrada utilizados para testes e execução do programa.   
├── output/                  # Contém os arquivos de saída gerados pela execução do programa.
├── src/                     # Contém os arquivos-fonte responsáveis pela implementação do projeto.
│   ├── main.c
│   ├── parser.c
│   └── ppersistent_bst.c
├── Makefile                 # Arquivo responsável pela automação do processo de compilação do projeto.
└── README.md                # Documento contendo informações gerais sobre o projeto, compilação, execução e organização do repositório.
```

---

## Como compilar e executar

### Compilação

O `Makefile` presente nesse projeto já cuida dessa extensa etapa, bastando apenas executar:

```bash
make build
```

Isso gera os executáveis auxiliares na pasta `build\`, bem como o próprio programa final `program.exe` na pasta `bin\`.

### Execução

Com o código já compilado, basta executar:

```bash
make run
```

A qual irá rodar o programa sobre o arquivo de exemplo presente em `input\`. Caso deseje rodar sobre um outro arquivo de entrada, basta inserir o arquivo na pasta e então executar:

```bash
make run INPUT=<arquivo_com_operações_na_PPBST.txt>
```

---

## Formato do arquivo de entrada

O arquivo de entrada é um `.txt` com **uma operação por linha**:

| Operação | Formato                | Descrição                                                                                                                   | Saída                                             |
|----------|------------------------|-----------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------|
| Inserção | `INC <valor>`          | Insere o inteiro na árvore, criando uma nova versão da estrutura. Duplicatas são permitidas.                                | Nenhuma                                           |
| Remoção  | `REM <valor>`          | Remove o nó com aquele valor, criando uma nova versão da estrutura. Se inexistente, a nova versão será idêntica a anterior. | Nenhuma                                           |
| Sucessor | `SUC <valor> <versão>` | Encontra o menor valor da árvore estritamente maior que `<valor>` na versão `<versão>`.                                     | Linha 1: `SUC <valor>` / Linha 2: resultado       |
| Imprimir | `IMP` `<versão>`       | Imprime todos os elementos da estrutura na versão `<versão>` em ordem crescente com sua profundidade.                       | Linha 1: `IMP` / Linha 2: `val,prof val,prof ...` |

> **Nota sobre `SUC`:** O valor de referência **não precisa** existir na árvore. Caso não exista sucessor, é impresso `INF`.

> **Nota sobre `IMP`:** A profundidade da raiz é **0**. O separador entre elementos é um espaço simples.

---

## Divisão dos módulos do projeto

O projeto é separado em 2 módulos especiais (`parser.h` e `ppersistent_bst.h`) juntamente de um arquivo `main.c`.

### Módulo `parser.h`

Módulo responsável pela leitura e processamento das operações passadas via um arquivo de entrada localizado no diretório `input\`. Das funções presentes no módulo, tem-se:

| Função          | Assinatura                                                | Arquivo    | Descrição |
|-----------------|-----------------------------------------------------------|------------|-----------|
| `parse_and_run` | `int parse_and_run(char *filename, PPersistentBST *tree)` | `parser.c` | Abre o arquivo, lê linha a linha e despacha cada operação para o módulo `ppersistent_bst`. Retorna `0` em sucesso ou `1` em erro de abertura do arquivo de entrada. |

---

### Módulo `ppersistent_bst.h`

Módulo responsável pela criação, manuntenção e limpeza de uma árvore binária de busca com persistência parcial, além disso, permite realizar as operações de consulta e atualização sobre a estrutura.

#### `Field`

Enumerate responsável por mapear os possíveis campos que podem serem alterados em uma árvore de busca convencional.

```c
typedef enum {
    NONE,
    LEFT,
    RIGHT,
    VALUE,
    ROOT
} Field;
```

#### `Node`

Registro responsável por representar um único nó persistente da árvore.

```c
struct Node {
    Node *left;        // Filho esquerdo
    Node *right;       // Filho direito 
    int value;         // Valor do nó (chave)
    int born_version;  // Versão que o nó foi criado
    int is_root;       // Flag usada para especificar se o nó é raíz da árvore                                 
    Mod mods[2];       // Vetor de modificações
};
```

#### `PPersistentBST`

Contêiner que encapsula informações essenciais para o versionamento parcial da estrutura. Apresenta de forma rápida informações à respeito da versão atual da estrutura bem como um vetor auxiliar contendo todos os nós da estrutura, essencial para gerenciamento eficiente de memória.

```c
typedef struct {
    Node *live_root;                // Raíz ativa (versão mais recente)
    Node *node_pool[MAX_VERSIONS];  // Pool de nós utilizados pela árvore (necessário para liberação de memória)
    int node_count;                 // Quantidade de nós da estrutura
    Node *versions[MAX_VERSIONS];   // version[i] representa a raíz na versão i
    int version_count;              // Quantidade de versões criadas até o momento
    int current_version;            // Versão atual da estrutura (mais recente)
} PPersistentBST;
```

Funções disponíveis ao usuário do módulo:

| Função                | Assinatura                                                           | Arquivo             | Descrição                                                                                  |
|-----------------------|----------------------------------------------------------------------|---------------------|--------------------------------------------------------------------------------------------|
| `ppbst_init`          | `void ppbst_init(PPersistentBST *tree)`                              | `ppersistent_bst.c` | Inicializa os campos de uma PPBST vazia.                                                   |
| `ppbst_insert`        | `void ppbst_insert(PPersistentBST *tree, int value)`                 | `ppersistent_bst.c` | Cria uma nova versão da estrutura e insere o nó de valor `value`.                          |
| `ppbst_remove`        | `void ppbst_remove(PPersistentBST *tree, int value)`                 | `ppersistent_bst.c` | Cria uma nova versão da estrutura e remove o nó com o valor `value`.                       |
| `ppbst_successor`     | `void ppbst_successor(PPersistentBST *tree, int value, int version)` | `ppersistent_bst.c` | Imprime no terminal o valor do nó com o menor valor estritamente maior que `value`, ou `INF` em caso desse não existir. |
| `ppbst_print_inorder` | `void ppbst_print_inorder(PPersistentBST *tree, int version)`        | `ppersistent_bst.c` | Imprime **valor,profundidade** de cada nó da estrutura numa dada versão em ordem crescente de valor.                  |
| `ppbst_destroy`       | `void ppbst_destroy(PPersistentBST *tree)`                           | `ppersistent_bst.c` | Desaloca a memória utilizada através do pool de nós da estrutura.                                      |

Funções auxiliares (`static`, não são expostas no header!) criadas com o intuito de simplificar os algoritmos principais e propiciar maior facilidade de manuntenção do projeto:

| Função                | Arquivo             | Descrição                                                                                                       |
|-----------------------|---------------------|-----------------------------------------------------------------------------------------------------------------|
| `node_new`            | `ppersistent_bst.c` | Aloca e inicializa um novo nó.                                                                                  |
| `set_node_by_version` | `ppersistent_bst.c` | Aplica as modificações até uma certa versão no nó passado por referência.                                       |
| `insert_mod`          | `ppersistent_bst.c` | Inseri um campo de modificação no nó.                                                                           |
| `node_copying`        | `ppersistent_bst.c` | Cria um novo nó apartir de um antigo, com os campos de modificações todos aplicados no nó novo.                 |
| `ppbst_search_parent` | `ppersistent_bst.c` | Busca pelo pai do nó.                                                                                           |
| `find_min`            | `ppersistent_bst.c` | Busca pelo nó com o menor valor numa dada sub-árvore numa dada versão.                                          |
| `find_successor`      | `ppersistent_bst.c` | Busca pelo nó sucessor de um valor em uma dada versão.                                                          |
| `transplant`          | `ppersistent_bst.c` | Função auxiliar no algoritmo de remoção em uma BST convencional, adaptada pro contexto de persistência parcial. |
| `node_print_inorder`  | `ppersistent_bst.c` | Função auxiliar na impressão da árvore.                                                                         |

---

### `main.c`

Ponto de entrada do programa. Responsável por realizar a leitura do arquivo de entrada passada como argumento, inicializar uma PPBST através da chamada de `ppbst_init`, passar o controle do processamento das operações ao parser através de `parser_and_run` e, por fim, liberar os recursos alocados via `ppbst_destroy`.

---

## Exemplo de uso

Dado o arquivo `input\input_persistent_extense.txt`, a qual contém as operações sobre a PPBST:

```text
INC 50
INC 50
INC 50
IMP 2
SUC 50 2
REM 50
IMP 3
IMP 2
REM 50
IMP 4
IMP 2
REM 50
IMP 5
IMP 2
INC 10
INC 20
INC 30
INC 40
INC 50
IMP 10
SUC 35 10
SUC 50 10
REM 30
IMP 11
IMP 10
SUC 30 11
SUC 30 10
INC 100
INC 200
INC 300
REM 200
IMP 15
IMP 14
SUC 200 14
SUC 200 15
SUC 150 15
REM 100
REM 300
IMP 17
IMP 15
INC 50
INC 30
INC 70
INC 20
INC 40
INC 60
INC 80
REM 50
IMP 24
IMP 23
SUC 39 24
SUC 40 24
SUC 80 24
SUC 80 23
```

Temos como saída retornada:

```
IMP 2
50,0 50,1 50,2
SUC 50 2
INF
IMP 3
50,0 50,1
IMP 2
50,0 50,1 50,2
IMP 4
50,0
IMP 2
50,0 50,1 50,2
IMP 5

IMP 2
50,0 50,1 50,2
IMP 10
10,0 20,1 30,2 40,3 50,4
SUC 35 10
40
SUC 50 10
INF
IMP 11
10,0 20,1 40,2 50,3
IMP 10
10,0 20,1 30,2 40,3 50,4
SUC 30 11
40
SUC 30 10
40
IMP 15
10,0 20,1 40,2 50,3 100,4 300,5
IMP 14
10,0 20,1 40,2 50,3 100,4 200,5 300,6
SUC 200 14
300
SUC 200 15
300
SUC 150 15
300
IMP 17
10,0 20,1 40,2 50,3
IMP 15
10,0 20,1 40,2 50,3 100,4 300,5
IMP 24
10,0 20,1 20,4 30,3 40,2 40,4 50,3 50,4 60,6 70,5 80,6
IMP 23
10,0 20,1 20,4 30,3 40,2 40,4 50,3 50,4 60,6 70,5
SUC 39 24
40
SUC 40 24
50
SUC 80 24
INF
SUC 80 23
INF
```

> A profundidade de cada nó pode variar conforme a ordem de inserção e remoções anteriores.
