#ifndef SISTEMA_GERENCIADOR_HPP
#define SISTEMA_GERENCIADOR_HPP

#include <string>

class SistemaGerenciador
{
private:
    std::string arquivoCSV;
    std::string arquivoDados;
    std::string arquivoIndicePrimario;
    std::string arquivoIndiceCurso;
    std::string arquivoListainvertidaCurso;
    std::string arquivioDisponiveis;
    
public:
    SistemaGerenciador(/* args */);
    ~SistemaGerenciador();
};

SistemaGerenciador::SistemaGerenciador(/* args */)
{
}




#endif