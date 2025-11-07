#ifndef SISTEMA_GERENCIADOR_HPP
#define SISTEMA_GERENCIADOR_HPP

#include <string>
#include "Aluno.hpp"

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
    void iniciar();
    void gerarArquivos();
    void inserirAluno();
    void buscarAlunoPorMatricula(int matricula, std::ifstream &in, std::vector<IndicePrimario> &indices);
    void bucarAlunosPorCurso();
    bool removerAlunoPorMatricula();

private:
    long buscarIndicePrimario(int matricula);
    long buscarIndiceSecundario(const std::string & curso);
    void escreverRegistro(std::ofstream &out, const Aluno &aluno);
    bool lerRegistro(std::ifstream &in, Aluno &aluno, long offset);
    long obterEspaçoDisponivel();
    void adicionarEspaçoDisponivel(long offset);
};

SistemaGerenciador::SistemaGerenciador(/* args */)
{
}

#endif