#ifndef INDICE_SECUNDARIO_HPP
#define INDICE_SECUNDARIO_HPP
#include <iostream>
#include "IndiceSecundario.hpp"
#include <vector>

class IndiceSecundario
{
public:
    char chave_curso[30];
    long rrn_lista_invertida;

public:
    IndiceSecundario(/* args */);
    ~IndiceSecundario();

    IndiceSecundario::IndiceSecundario(/* args */)
    {
    }

    IndiceSecundario::~IndiceSecundario()
    {
    }

    void organizaIndices(std::vector<IndiceSecundario> &indice, int tam);

    void troca(std::vector<IndiceSecundario> &indice, int a, int b);

    void quickSort(std::vector<IndiceSecundario> &indice, int ini, int fim);

    int random(std::vector<IndiceSecundario> &indice, int ini, int fim);

    int partition(std::vector<IndiceSecundario> &indice, int ini, int fim);
};

#endif