# Arquivo de entrada
INPUT = input.txt

# Flags de compilação, arquivos fontes e objetos
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic
SRCS    = src\main.c src\bst.c src\parser.c
OBJS    = build\main.o build\bst.o build\parser.o
LIBS	= include

# Nome do executável 
TARGET  = program.exe

# Compila e executa (build + run)
all: build run

# Compilação do código
build: build-single
	gcc $(CFLAGS) $(OBJS) -o $(TARGET)

build-single: $(SRCS)
	gcc $(CFLAGS) -c src\bst.c -I $(LIBS)  -o build\bst.o
	gcc $(CFLAGS) -c src\parser.c -I $(LIBS) -o build\parser.o
	gcc $(CFLAGS) -c src\main.c -I $(LIBS) -o build\main.o

# Execução do programa
# Uso: make run INPUT=meu_arquivo.txt
run: bin\$(TARGET)
	bin\$(TARGET) input\$(INPUT)
