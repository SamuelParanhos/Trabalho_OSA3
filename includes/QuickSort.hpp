#ifndef QUICK_SORT
#define QUICK_SORT

#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "IndiceSecundario.hpp"
#include "Aluno.hpp"

template <typename T>
class QuickSort
{
private:
    void quickSort_recursivo(std::vector<T> &t, int ini, int fim);
    int random_pivot(std::vector<T> &t, int ini, int fim);
    int partition(std::vector<T> &t, int ini, int fim);
    
public:
    // Função principal de interface
    void organizaIndices(std::vector<T> &t, int tam);
    
    // Função auxiliar (troca) - pode ser privada, mas deixamos pública por clareza
    void troca(std::vector<T> &t, int a, int b);
};


template <typename T>
void QuickSort<T>::organizaIndices(std::vector<T> &t, int tam)
{
    if (tam > 0) {
        quickSort_recursivo(t, 0, tam - 1);
    }
}

template <typename T>
void QuickSort<T>::troca(std::vector<T> &t, int a, int b)
{
    T aux = t[a];
    t[a] = t[b];
    t[b] = aux;
}

template <typename T>
void QuickSort<T>::quickSort_recursivo(std::vector<T> &t, int ini, int fim)
{
    if (ini < fim)
    {
        int q = random_pivot(t, ini, fim);

        quickSort_recursivo(t, ini, q - 1); // Exclui o pivô
        quickSort_recursivo(t, q + 1, fim); // Exclui o pivô
    }
}

template <typename T>
int QuickSort<T>::random_pivot(std::vector<T> &t, int ini, int fim)
{
    // Garante que o rand seja inicializado apenas uma vez
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    
    int range = fim - ini + 1;
    if (range <= 1) return fim; 
    
    int q = ini + (rand() % range); 
    
    // Coloca o pivô escolhido (q) na posição final (fim)
    troca(t, q, fim); 
    
    return partition(t, ini, fim);
}

template <typename T>
int QuickSort<T>::partition(std::vector<T> &t, int ini, int fim)
{

    const char *pivo = t[fim].curso; 

    int i = ini - 1;

    for (int j = ini; j < fim; j++) 
    {
      
        if (strcmp(t[j].curso, pivo) <= 0) 
        {
            i++;
            troca(t, i, j);
        }
    }


    troca(t, i + 1, fim); 
    return i + 1;
}

#endif