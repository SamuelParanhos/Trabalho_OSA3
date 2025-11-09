#ifndef NO_LISTA_INVERTIDA
#define NO_LISTA_INVERTIDA
#include <fstream>
#include "Aluno.hpp"

class NoListaInvertida
{
private:
public:
    int matricula_aluno;
    long proximo_rrn;

public:
    void insereMatricula(std::ofstream &listaInvertida, Aluno &aluno);
};

#endif
