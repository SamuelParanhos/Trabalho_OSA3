#ifndef SISTEMA_GERENCIADOR_HPP
#define SISTEMA_GERENCIADOR_HPP

#include <string>
#include <fstream>
#include <vector>
#include "Aluno.hpp"
#include "IndicePrimario.hpp"
#include "IndiceSecundario.hpp"
#include "NoListaInvertida.hpp"

class SistemaGerenciador
{
private:
    std::string arquivoCSV;
    std::string arquivoDados;
    std::string arquivoIndicePrimario;
    std::string arquivoIndiceCurso;
    std::string arquivoListainvertidaCurso;
    std::string arquivioDisponiveis;
    std::string arquivoIndiceSecundario;
    std::string ListaInvertida;

public:
    void iniciar();
    void gerarArquivos();
    void gerarArquivoIndicePrimario();
    void gerarArquivoIndiceSecundario();
    void inserirAluno();
    void buscarAlunoPorMatricula(int matricula, std::ifstream &in, std::vector<IndicePrimario> &indices);
    void bucarAlunosPorCurso();
    bool removerAlunoPorMatricula();
    std::vector<std::string> encontraCursos(std::vector<IndiceSecundario> indices);

private:
    long buscarIndicePrimario(int matricula, const std::vector<IndicePrimario> &indices);
    long buscarIndiceSecundario(const std::string &curso);
    void escreverRegistro(std::ofstream &out, const Aluno &aluno);
    bool lerRegistro(std::ifstream &in, Aluno &aluno, long offset);
    long obterEspaçoDisponivel();
    void adicionarEspaçoDisponivel(long offset);
};
#endif