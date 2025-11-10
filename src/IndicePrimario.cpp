#include "../includes/IndicePrimario.hpp"
#include "../includes/SistemaGerenciador.hpp"

// Ordena o vetor de índices usando HeapSort
void IndicePrimario::organizar(std::vector<IndicePrimario> &indices)
{
    int n = indices.size();

    // Constrói o heap máximo
    for (int i = n / 2 - 1; i >= 0; i--)
        desce(indices, n, i);

    // Extrai elementos do heap um por um
    for (int i = n - 1; i >= 1; i--)
    {
        troca(indices, 0, i); // Move o maior para o fim
        desce(indices, i, 0); // Restaura heap nas posições restantes
    }
}

void IndicePrimario::desce(std::vector<IndicePrimario> &indices, int tamanho, int no)
{
    int maior = no;      // Índice do maior elemento
    int fe = 2 * no + 1; // Filho esquerdo
    int fd = 2 * no + 2; // Filho direito

    // Se o filho esquerdo existe e é maior que o pai, atualiza "maior"
    if (fe < tamanho && indices[fe].matricula > indices[maior].matricula)
        maior = fe;
    // Se o filho direito existe e é maior que o atual "maior", atualiza novamente
    if (fd < tamanho && indices[fd].matricula > indices[maior].matricula)
        maior = fd;

    // Se o maior não for o próprio nó, faz a troca e continua descendo recursivamente
    if (maior != no)
    {
        troca(indices, no, maior);
        desce(indices, tamanho, maior); // Continua descendo na nova posição
    }
}

void IndicePrimario::sobe(std::vector<IndicePrimario> &indices, int no)
{
    // A raiz está no índice 0, então para se o nó for a raiz
    if (no == 0)
    {
         return;
    }
    // Calcula o índice do nó pai
    int pai = (no - 1) / 2;

    // Se o elemento no nó atual filho for maior que o pai
    if (indices[no].matricula > indices[pai].matricula)
    {
        troca(indices, no, pai); // Troca o filho com o pai
        sobe(indices, pai);      // Chama recursivamente para continuar subindo
    }
}

// troca dois elementos de posição
void IndicePrimario::troca(std::vector<IndicePrimario> &indices, int i, int j)
{
    IndicePrimario temp = indices[i];
    indices[i] = indices[j];
    indices[j] = temp;
}