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
};
#endif

template <typename T>

 // Função principal de interface
void organizaIndices(std::vector<T> &auxiliar, int tam)
{
    if (tam > 0)
    {
        quickSort_recursivo(auxiliar, 0, tam - 1);
    }
}
template <typename T>
void troca(std::vector<T> &t, int a, int b)
{
    T aux = t[a];
    t[a] = t[b];
    t[b] = aux;
}
template <typename T>
void quickSort_recursivo(std::vector<T> &aux, int ini, int fim)
{
    if (ini < fim)
    {
        int q = partition(aux, ini, fim);

        quickSort_recursivo(aux, ini, q - 1); // Exclui o pivô
        quickSort_recursivo(aux, q + 1, fim); // Exclui o pivô
    }
}
template <typename T>
int partition(std::vector<T> &auxiliar, int ini, int fim)
{

    const char *pivo = auxiliar[fim].curso;

    int i = ini - 1;

    for (int j = ini; j < fim; j++)
    {

        if (strcmp(auxiliar[j].curso, pivo) <= 0)
        {
            i++;
            troca(auxiliar, i, j);
        }
    }

    troca(auxiliar, i + 1, fim);
    return i + 1;
}