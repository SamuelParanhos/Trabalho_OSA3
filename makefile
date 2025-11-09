MAIN := main

# Pastas
INCLUDES := includes
SRC := src

# Objetos a compilar (Adicionando IndicePrimario, IndiceSecundario, NoListaInvertida)
OBJECTS := $(MAIN).o \
           $(SRC)/Aluno.o \
           $(SRC)/IndicePrimario.o \
           $(SRC)/IndiceSecundario.o \
           $(SRC)/NoListaInvertida.o \
           $(SRC)/SistemaGerenciador.o

# Flags do compilador
FLAGS := -Wall -Wextra -std=c++17 -pedantic-errors -I$(INCLUDES)

# Debug
DEBUG := -g

# Bibliotecas extras (Não necessário para este projeto, mas mantido)
MATH := -lm

# Compilador
CC := g++

# Ajuste para SO
ifeq ($(OS), Windows_NT)
OUTPUTMAIN := $(MAIN).exe
else
OUTPUTMAIN := $(MAIN).out
endif

# Alvo principal
all: $(OUTPUTMAIN)
	@echo Compiling 'all' complete!

# Linkagem final
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN) $(MATH)

# --- Regras de compilação dos objetos ---

# main.o
$(MAIN).o: $(MAIN).cpp \
           $(INCLUDES)/Aluno.hpp \
           $(INCLUDES)/IndicePrimario.hpp \
           $(INCLUDES)/IndiceSecundario.hpp \
           $(INCLUDES)/NoListaInvertida.hpp \
           $(INCLUDES)/SistemaGerenciador.hpp
	$(CC) $(FLAGS) -c $(MAIN).cpp -o $(MAIN).o

# Aluno.o
$(SRC)/Aluno.o: $(SRC)/Aluno.cpp $(INCLUDES)/Aluno.hpp
	$(CC) $(FLAGS) -c $(SRC)/Aluno.cpp -o $(SRC)/Aluno.o

# IndicePrimario.o
$(SRC)/IndicePrimario.o: $(SRC)/IndicePrimario.cpp $(INCLUDES)/IndicePrimario.hpp
	$(CC) $(FLAGS) -c $(SRC)/IndicePrimario.cpp -o $(SRC)/IndicePrimario.o

# IndiceSecundario.o
$(SRC)/IndiceSecundario.o: $(SRC)/IndiceSecundario.cpp $(INCLUDES)/IndiceSecundario.hpp
	$(CC) $(FLAGS) -c $(SRC)/IndiceSecundario.cpp -o $(SRC)/IndiceSecundario.o

# NoListaInvertida.o
$(SRC)/NoListaInvertida.o: $(SRC)/NoListaInvertida.cpp $(INCLUDES)/NoListaInvertida.hpp
	$(CC) $(FLAGS) -c $(SRC)/NoListaInvertida.cpp -o $(SRC)/NoListaInvertida.o

# SistemaGerenciador.o
$(SRC)/SistemaGerenciador.o: $(SRC)/SistemaGerenciador.cpp \
                            $(INCLUDES)/SistemaGerenciador.hpp \
                            $(INCLUDES)/Aluno.hpp \
                            $(INCLUDES)/IndicePrimario.hpp \
                            $(INCLUDES)/IndiceSecundario.hpp \
                            $(INCLUDES)/NoListaInvertida.hpp
	$(CC) $(FLAGS) -c $(SRC)/SistemaGerenciador.cpp -o $(SRC)/SistemaGerenciador.o

# --- Alvos de Ação ---

# Limpeza
clean:
	rm -rf $(OBJECTS)
	rm -rf $(OUTPUTMAIN)
	@echo Cleanup complete!!!

# Executar
run: all
	./$(OUTPUTMAIN)
	@echo Executing 'all' complete!!!