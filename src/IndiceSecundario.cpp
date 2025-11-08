#include "../includes/IndiceSecundario.hpp"
#include "../includes/SistemaGerenciador.hpp"
#include <cstring>

void IndiceSecundario::organizaIndices(std::vector<IndiceSecundario> &indice, int tam)
{
    quickSort(indice, 0, tam);
}
void IndiceSecundario::troca(std::vector<IndiceSecundario> &indice, int a, int b)
{
    IndiceSecundario aux = indice[a];
    indice[a] = indice[b];
    indice[b] = aux;
}
void IndiceSecundario::quickSort(std::vector<IndiceSecundario> &indice, int ini, int fim)
{
    if (ini < fim - 1)
    {
        int q = random(indice, ini, fim);

        quickSort(indice, ini, q);
        quickSort(indice, q + 1, fim);
    }
}

int IndiceSecundario::random(std::vector<IndiceSecundario> &indice, int ini, int fim)
{
    int q = ini + (rand() % (fim - ini) + 1);

    troca(indice, fim, q);

    partition(indice, ini, fim);
}

int IndiceSecundario::partition(std::vector<IndiceSecundario> &indice, int ini, int fim)
{
    char pivo[30];
    strcpy(pivo, indice[fim].chave_curso);

    int i = ini - 1;

    for (int j = 0; j < indice.size() - 1; j++)
    {
        if (strcmp(pivo, indice[j].chave_curso) <= 0)
        {
            troca(indice, i, j);
        }
    }

    troca(indice, i + 1, fim);
    return i + 1;
}