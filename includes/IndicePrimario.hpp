#ifndef INDICE_PRIMARIO_HPP
#define INDICE_PRIMARIO_HPP

#include <vector>
#include "Aluno.hpp"

class IndicePrimario
{
public:
    int matricula;
    long byte_offset; // Posição do registro no arquivo de dados
    // HeapSort
    void organizar(std::vector<IndicePrimario> &indices);

private:
    void desce(std::vector<IndicePrimario> &indices, int tamanho, int no);
    void sobe (std::vector<IndicePrimario> &indices, int no);
    void troca(std::vector<IndicePrimario> &indices, int i, int j);
    
};
#endif