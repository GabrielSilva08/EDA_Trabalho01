# ── Compilador e flags ────────────────────────────────────────
CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g

# ── Arquivos fonte e objeto ───────────────────────────────────
SRCS    = main.c bst.c parser.c
OBJS    = $(SRCS:.c=.o)

# ── Nome do executável ────────────────────────────────────────
TARGET  = bst

# ── Arquivo de entrada padrão (usado em `make run`) ──────────
INPUT   ?= input.txt

# ══════════════════════════════════════════════════════════════
#  Regras
# ══════════════════════════════════════════════════════════════

## Alvo padrão: compila tudo
all: $(TARGET)

## Linka os objetos e gera o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

## Compila cada .c em seu respectivo .o
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

## Executa o programa com o arquivo de entrada definido em INPUT
## Uso: make run INPUT=meu_arquivo.txt
run: $(TARGET)
	./$(TARGET) $(INPUT)

## Remove arquivos gerados pela compilação
clean:
	rm -f $(OBJS) $(TARGET)

## Exibe ajuda resumida
help:
	@echo "Alvos disponíveis:"
	@echo "  make           Compila o projeto (padrão)"
	@echo "  make run       Compila e executa com INPUT=<arquivo>"
	@echo "  make clean     Remove objetos e executável"
	@echo "  make help      Exibe esta mensagem"

# ── Declara alvos que não são arquivos ───────────────────────
.PHONY: all run clean help
