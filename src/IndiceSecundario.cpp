#include "../includes/IndiceSecundario.hpp"

#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "IndiceSecundario.hpp"
#include "Aluno.hpp"

void IndiceSecundario::organizaIndices(std::vector<Auxiliar> &auxiliar, int tam)
{
    if (tam > 0)
    {
        quickSort_recursivo(auxiliar, 0, tam - 1);
    }
}

void IndiceSecundario::troca(std::vector<Auxiliar> &t, int a, int b)
{
    Auxiliar aux = t[a];
    t[a] = t[b];
    t[b] = aux;
}

void IndiceSecundario::quickSort_recursivo(std::vector<Auxiliar> &aux, int ini, int fim)
{
    if (ini < fim)
    {
        int q = partition(aux, ini, fim);

        quickSort_recursivo(aux, ini, q - 1); // Exclui o pivô
        quickSort_recursivo(aux, q + 1, fim); // Exclui o pivô
    }
}

int IndiceSecundario::partition(std::vector<Auxiliar> &auxiliar, int ini, int fim)
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
