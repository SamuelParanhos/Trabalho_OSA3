#ifndef SISTEMA_GERENCIADOR_HPP
#define SISTEMA_GERENCIADOR_HPP

#include <string>
#include <fstream>
#include "Aluno.hpp"
#include "IndicePrimario.hpp"

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
    void iniciar();
    void gerarArquivos();
    void gerarArquivoIndicePrimario();
    void inserirAluno();
    void buscarAlunoPorMatricula(int matricula, std::ifstream &in, std::vector<IndicePrimario> &indices);
    void bucarAlunosPorCurso();
    bool removerAlunoPorMatricula();

private:
    long buscarIndicePrimario(int matricula, const std::vector<IndicePrimario> &indices);
    long buscarIndiceSecundario(const std::string & curso);
    void escreverRegistro(std::ofstream &out, const Aluno &aluno);
    bool lerRegistro(std::ifstream &in, Aluno &aluno, long offset);
    long obterEspaçoDisponivel();
    void adicionarEspaçoDisponivel(long offset);
};
#endif