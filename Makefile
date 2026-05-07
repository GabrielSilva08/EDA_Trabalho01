# Arquivo de entrada
INPUT = input_persistent.txt

# Flags de compilação, arquivos fontes e objetos
CFLAGS  = -std=c11 -Wall -Wextra -Werror -pedantic
SRCS    = src\main.c src\ppersistent_bst.c src\parser.c
OBJS    = build\main.o build\ppersistent_bst.o build\parser.o
LIBS	= include

# Nome do executável 
TARGET  = bin\program.exe

# Compila e executa (build + run)
all: build run

# Compilação do código
build: build-single
	gcc $(CFLAGS) $(OBJS) -o $(TARGET)

build-single: $(SRCS)
	gcc $(CFLAGS) -c src\ppersistent_bst.c -I $(LIBS)  -o build\ppersistent_bst.o
	gcc $(CFLAGS) -c src\parser.c -I $(LIBS) -o build\parser.o
	gcc $(CFLAGS) -c src\main.c -I $(LIBS) -o build\main.o

# Execução do programa
# Uso: make run INPUT=meu_arquivo.txt
run: $(TARGET)
	$(TARGET) input\$(INPUT)
