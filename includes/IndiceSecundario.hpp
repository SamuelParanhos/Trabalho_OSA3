#ifndef INDICE_SECUNDARIO_HPP
#define INDICE_SECUNDARIO_HPP

#include "Auxiliar.hpp"
#include <vector>
#include <iostream>

class IndiceSecundario
{
public:
    char curso[30];
    long rrn_lista_invertida;

private:
    void quickSort_recursivo(std::vector<Auxiliar> &auxiliar, int ini, int fim);
    int partition(std::vector<Auxiliar> &auxiliar, int ini, int fim);

public:
    // Função principal de interface
    void organizaIndices(std::vector<Auxiliar> &auxiliar, int tam);

    // Função auxiliar (troca) - pode ser privada, mas deixamos pública por clareza
    void troca(std::vector<Auxiliar> &auxiliar, int a, int b);

public:
};
#endif