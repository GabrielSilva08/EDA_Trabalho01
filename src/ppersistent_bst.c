#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ppersistent_bst.h>

/* Funções utilitárias do PPBST */

/*
 * Inicializa um nó com seus campos preenchidos. Faz-se uso de alocação dinâmica de memória por nó.
 *
 * Params:
 *  left (Node*): Filho esquerdo;
 *  right (Node*): Filho direito;
 *  value (int): Valor do nó (chave);
 *  version (int): Versão que o nó está sendo criado;
 *  is_root (int): Flag usada para especificar se o nó criado será do tipo raíz da árvore.
*/
static Node *node_new(Node *left, Node *right, int value, int version, int is_root);

/* 
 * Copia o estado de old_node para new_node e aplica apenas as modificações cuja versões sejam menores ou iguais a "version".
 *
 * Params:
 *  old_node (Node*): Nó antigo a qual se deseja copiar para o nó novo;
 *  new_node (Node*): Nó novo a qual se deseja aplicar o versionamento;
 *  version (int): Versão de interesse.
*/
static void  set_node_by_version(Node *old_node, Node *new_node, int version);

/*
 * Realiza uma tentativa de inserção da modificação new_mod em node. Em caso de falta de espaço, é criado um novo nó.
 *
 * Params:
 *  tree (PPersistentBST*): PPBST a qual node pertence;
 *  node (Node*): Nó a qual se deseja inserir a modificação;
 *  new_mod (Mod): Modificação nova a ser registrada.
*/
static Node *insert_mod(PPersistentBST *tree, Node *node, Mod new_mod);

/* 
 * Cria uma cópia física de node quando seus slots de modificação estão todos ocupados.
 *
 * Params:
 *  tree (PPersistentBST*): PPBST a qual node pertence;
 *  node (Node*): Nó a qual se deseja realizar a cópia;
 *  new_mod (Mod): Modificação nova a ser registrada.
 * 
 * Returns:
 *  (Node*): Nó cópia de node com os campos de mods já esvaziados.
*/
static Node *node_copying(PPersistentBST *tree, Node *node, Mod new_mod);

/* 
 * Busca pelo nó pai do nó na versão atual, NULL se não houver.
 *
 * Params:
 *  tree (PPersistentBST*): PPBST a qual se deseja procurar o nó pai;
 *  target (Node*): Nó a qual se deseja localizar o pai.
 * 
 * Returns:
 *  (Node*): Nó pai procurado (ou NULL).
*/
static Node *ppbst_search_parent(PPersistentBST *tree, Node *target);

/* 
 * Busca em tree pelo nó cujo valor seja o menor de todos. Necessário no algoritmo de remoção.
 *
 * Params:
 *  subtree (Node*): Nó a qual se deseja começar a busca pelo menor nó;
 *  version (int): Versão da estrutura procurada.
 * 
 * Returns:
 *  (Node*): Nó com menor valor em subtree.
*/
static Node *find_min(Node *subtree, int version);

/* 
 * Busca a partir de versioned_root, pelo nó cujo valor seja maior que "value" na versão "version".
 *
 * Params:
 *  versioned_root (Node*): Raíz da versão a qual se deseja procurar pelo sucessor;
 *  value (int): Valor a qual desejamos buscar o sucessor na árvore;
 *  version (int): Versão da estrutura que desejamos fazer a busca.
 * 
 * Returns:
 *  (Node*): Nó cujo valor é o primeiro maior que "value" (sucessor). Ou NULL caso esse nó não exista.
*/
static Node *find_successor(Node *versioned_root, int value, int version);

/* 
 * Substitui (transplanta) o nó u por v dentro da versão base, reconstruindo todos os ancestrais de u como cópias físicas para não contaminar versões antigas.
 * Retorna a nova raíz da árvore transplantada.
 * 
 * Params:
 * 
 *  tree (PPersistentBST): Árvore a qual está sendo feito o processo de remoção de u;
 *  versioned_root (Node*): Raíz da árvore na versão base;
 *  u (Node*): Nó a ser removido;
 *  v (Node*): Nó a ser colocado no lugar de u;
 *  base_version (int): Versão base (antes da remoção);
 *  new_version (int): Versão nova (após a remoção).
 * 
 * Returns:
 *  (Node*): Nó v já transplantado.
 */
static Node *transplant(PPersistentBST *tree, Node *versioned_root, Node *u, Node *v, int base_version, int new_version);

/* 
 * Função recursiva auxiliar responsável por imprimir o valor do nó juntamente com sua profundidade seguindo um percuso em-ordem (nós impressos ordenadamente).
 * 
 * Params:
 *  node (Node*): Nó a qual se deseja realizar a impressão;
 *  version (int): Versão de interesse;
 *  depth (int): Profundidade associada ao nó;
 *  first (int*): Flag responsável por separar o resultado da impressão dos pares "value,depth".
*/
static void node_print_inorder(Node *node, int version, int depth, int *first);

/* Funções principais da PPBST (disponíveis ao usuário da lib "ppersistent_bst.h")*/

void ppbst_init   (PPersistentBST *tree);
void ppbst_insert (PPersistentBST *tree, int value);
void ppbst_remove (PPersistentBST *tree, int value);
void ppbst_sucessor(PPersistentBST *tree, int value, int version);
void ppbst_print_inorder(PPersistentBST *tree, int version);
void ppbst_destroy(PPersistentBST *tree);

/* Implementações */

static Node *node_new(Node *left, Node *right, int value, int version, int is_root){
    Node *node = (Node*) malloc(sizeof(Node));
    if (!node){
        fprintf(stderr, "Erro: falha ao alocar memória para nó.\n");
        exit(EXIT_FAILURE); 
    }
    // Atribuição dos campos
    node->left = left;
    node->right = right;
    node->value = value;
    node->born_version = version;
    node->is_root = is_root;
    // Inicialização dos campos de modificação
    for (int i = 0; i < 2; i++) {
        node->mods[i].version   = -1;
        node->mods[i].mod_field = NONE;
        node->mods[i].mod_value   = -1;
        node->mods[i].mod_ptr   = NULL;
    }
    return node;
}

static void set_node_by_version(Node *old_node, Node *new_node, int version){
    new_node->left = old_node->left;
    new_node->right = old_node->right;
    new_node->value = old_node->value;
    new_node->is_root = old_node->is_root;

    for (int i = 0; i < 2; i++){
        if (old_node->mods[i].mod_field == NONE) continue;
        if (old_node->mods[i].version > version) continue;
        switch (old_node->mods[i].mod_field){
            case VALUE:   new_node->value = old_node->mods[i].mod_value; break;
            case LEFT:  new_node->left = old_node->mods[i].mod_ptr; break;
            case RIGHT: new_node->right = old_node->mods[i].mod_ptr; break;
            case ROOT:  new_node->is_root = old_node->mods[i].mod_value; break;
            default: break;
        }
    }
}

static Node *node_copying(PPersistentBST *tree, Node *node, Mod new_mod){
    Node *copy = node_new(node->left, node->right, node->value, node->born_version, node->is_root);
    // Aplica todas as modificações presentes em "node" em "copy".
    set_node_by_version(node, copy, -1);

    // Aplicação da nova modificação diretamente em copy
    switch (new_mod.mod_field) {
        case VALUE: copy->value = new_mod.mod_value; break;
        case LEFT: copy->left = new_mod.mod_ptr; break;
        case RIGHT: copy->right = new_mod.mod_ptr; break;
        case ROOT: copy->is_root = new_mod.mod_value; if (new_mod.mod_value) tree->live_root = copy; break;
        default: break;
    }
    copy->born_version = new_mod.version;

    // Atualização do filho do pai do nó node, pois agora seu filho é copy! (se houver)
    Node *parent = ppbst_search_parent(tree, node);
    if (parent){
        Field side = (copy->value < parent->value) ? LEFT : RIGHT;
        Mod up_mod = {new_mod.version, side, -1, copy};
        insert_mod(tree, parent, up_mod);
    }
    tree->node_pool[tree->node_count++] = copy;
    return copy;
}

static Node *insert_mod(PPersistentBST *tree, Node *node, Mod new_mod){
    if (!node) return NULL;

    // Caso 1: Versão a ser editada corresponde ao nó -> Nenhuma modificação precisa ser salva!
    if (node->born_version == new_mod.version){
        // Mudança diretamente no campo do nó original
        switch (new_mod.mod_field){
            case LEFT: node->left = new_mod.mod_ptr; break;
            case RIGHT: node->right = new_mod.mod_ptr; break;
            case ROOT: node->is_root = new_mod.mod_value; if (new_mod.mod_value) tree->live_root = node; break;
            default: break;
        }
        return node;
    }

    // Caso 2: Há espaço no campo de mods -> Modificação salva no mesmo nó.
    for (int i = 0; i < 2; i++){
        if (node->mods[i].mod_field == NONE) {
            node->mods[i] = new_mod;
            if (new_mod.mod_field == ROOT && new_mod.mod_value) tree->live_root = node;
            return node;
        }
    }

    // Caso 3: Não há espaço no campo de mods -> Necessário criar novo nó e aplicar as modificações nele.
    return node_copying(tree, node, new_mod);
}

static Node *ppbst_search_parent(PPersistentBST *tree, Node *target){
    if (tree->current_version < 0 || !tree->live_root) return NULL;

    Node *current = tree->live_root;
    while(current){
        Node tmp;
        set_node_by_version(current, &tmp, tree->current_version);
        if (tmp.left == target || tmp.right == target) return current;
        current = (target->value < tmp.value) ? tmp.left : tmp.right;
    }
    return NULL;
}

static Node *find_min(Node *tree, int version){
    Node *cur = tree;
    // Busca do filho mais à esquerda de tree na versão version.
    while(1){
        Node tmp;
        set_node_by_version(cur, &tmp, version);
        if (!tmp.left) break;
        cur = tmp.left;
    }
    return cur;
}

static Node *find_successor(Node *versioned_root, int value, int version){
    Node *current = versioned_root;
    Node *candidate = NULL;
    while(current){
        Node tmp;
        set_node_by_version(current, &tmp, version);
        if (tmp.value > value){ 
            candidate = current; 
            current = tmp.left;
        } else {
            current = tmp.right;
        }
    }
    return candidate;
}

static Node *transplant(PPersistentBST *tree, Node *versioned_root, Node *u, Node *v, int base_version, int new_version){
    // u era raíz -> v troca de lugar com u
    if (versioned_root == u) return v;

    // Caminho da raíz até u
    Node *path[MAX_VERSIONS];
    int path_len = 0;
    Node *current = versioned_root;

    while (current && current != u) {
        path[path_len++] = current;
        Node tmp;
        set_node_by_version(current, &tmp, base_version);
        current = (u->value < tmp.value) ? tmp.left : tmp.right;
    }
    if (!current) return versioned_root;

    Node *child_substitute = v;
    // Reconstrução do caminho
    for (int i = path_len - 1; i >= 0; i--) {
        Node *orig = path[i];
        Node  tmp_orig;
        set_node_by_version(orig, &tmp_orig, base_version);

        // Qual filho estava no caminho para u?
        Node *child_in_path = (i + 1 < path_len) ? path[i + 1] : u;
        Node *copy = node_new(tmp_orig.left, tmp_orig.right, tmp_orig.value, new_version, 0);
        tree->node_pool[tree->node_count++] = copy;

        if (tmp_orig.left == child_in_path) copy->left = child_substitute;
        else copy->right = child_substitute;

        child_substitute = copy;
    }
    return child_substitute;
}

static void node_print_inorder(Node *node, int version, int depth, int *first){
    if (!node) return;
    
    Node tmp;
    set_node_by_version(node, &tmp, version);
    node_print_inorder(tmp.left, version, depth + 1, first);

    // Imprime espaço antes de cada elemento par "valor,profundidade" (exceto o primeiro) 
    if (!(*first)){
        printf(" ");
    }
    *first = 0;
    printf("%d,%d", tmp.value, depth);

    node_print_inorder(tmp.right, version, depth + 1, first);
}

void ppbst_init(PPersistentBST *tree){
    tree->live_root = NULL;
    tree->node_count   = 0;
    tree->version_count = 1;
    tree->current_version  = 0;
    memset(tree->node_pool, 0, sizeof(tree->node_pool));
    memset(tree->versions,  0, sizeof(tree->versions));
}

void ppbst_insert(PPersistentBST *tree, int value){
    int new_version = tree->current_version + 1;
    // Nó a ser inserido na nova versão da estrutura
    Node *new_node = node_new(NULL, NULL, value, new_version, 0);
    tree->node_pool[tree->node_count++] = new_node;

    // Primeiro nó inserido na esturutra (raíz)
    if (tree->current_version < 0 || tree->versions[tree->current_version] == NULL){
        new_node->is_root = 1;
        tree->live_root = new_node;
        tree->versions[tree->version_count++] = new_node;
        tree->current_version = new_version;
        return;
    }

    // Localização da posição correta de inserção do nó
    // Raíz atual
    Node *current = tree->versions[tree->current_version]; 
    Node *parent = NULL;

    while (current){
        parent = current;
        Node tmp;
        set_node_by_version(current, &tmp, tree->current_version);
        current = (value < tmp.value) ? tmp.left : tmp.right;
    }

    // Registra a modificação no pai
    Node tmp_parent;
    set_node_by_version(parent, &tmp_parent, tree->current_version);
    Field side = (value < tmp_parent.value) ? LEFT : RIGHT;

    Mod m = {new_version, side, -1, new_node};
    insert_mod(tree, parent, m);

    tree->versions[tree->version_count++] = tree->live_root;
    tree->current_version = new_version;
}

void ppbst_remove(PPersistentBST *tree, int value){
    int base_version = tree->current_version;

    // Estrutura vazia -> Cópia da versão nula
    if (base_version < 0) {
        tree->current_version++;
        tree->versions[tree->version_count++] = NULL;
        tree->live_root = NULL;
        return;
    }

    // Busca pelo nó a ser removido
    Node *versioned_root = tree->versions[base_version];
    Node *target = versioned_root;

    while(target){
        Node tmp;
        set_node_by_version(target, &tmp, base_version);
        if (value == tmp.value) break;
        target = (value < tmp.value) ? tmp.left : tmp.right;
    }

    int new_version = ++tree->current_version;

    // Nó não encontrado -> Nova versão idêntica à anterior
    if (!target) {
        tree->versions[tree->version_count++] = versioned_root;
        tree->live_root = versioned_root;
        return;
    }

    Node *new_root;
    Node tmp_target;
    set_node_by_version(target, &tmp_target, base_version);

    // Caso 1/2: Sem filho direito ou sem filho esquerdo. 
    if (!tmp_target.right){
        new_root = transplant(tree, versioned_root, target, tmp_target.left, base_version, new_version);
    } else if (!tmp_target.left){
        new_root = transplant(tree, versioned_root, target, tmp_target.right, base_version, new_version);
    } else {
        // Caso 3: Nó com dois filhos -> Substituição pelo menor nó da subárvore direita (sucessor).
        Node *successor = find_min(tmp_target.right, base_version);
        Node  tmp_successor;
        set_node_by_version(successor, &tmp_successor, base_version);

        // Remove o sucessor de sua posição original na subárvore direita.
        Node *new_right = transplant(tree, tmp_target.right, successor, tmp_successor.right, base_version, new_version);

        // Cria um novo nó com o valor igual ao do sucessor, assumindo o lugar do nó target
        Node *replacement = node_new(tmp_target.left, new_right, tmp_successor.value, new_version, 0);
        tree->node_pool[tree->node_count++] = replacement;

        new_root = transplant(tree, versioned_root, target, replacement, base_version, new_version);
    }
    tree->versions[tree->version_count++] = new_root;
    tree->live_root = new_root;
}

void ppbst_successor(PPersistentBST *tree, int value, int version){
    if(version > tree->current_version){
        version = tree->current_version;
    }
    Node* successor = find_successor(tree->versions[version], value, version);
    if(successor) printf("%d\n", successor->value);
    else printf("INF\n");
}

void ppbst_print_inorder(PPersistentBST *tree, int version) {
    // Flag para evitar espaço antes do primeiro elemento
    int first = 1; 
    if(version >= tree->current_version){
        node_print_inorder(tree->versions[tree->current_version], tree->current_version, 0, &first);
    }else{
        node_print_inorder(tree->versions[version], version, 0, &first);
    }
    printf("\n");
}

void ppbst_destroy(PPersistentBST *tree){
    // Desalocando os do pool (todos os nós)
    for(int i = 0; i < tree->node_count; i++){
        free(tree->node_pool[i]);
    }
}
