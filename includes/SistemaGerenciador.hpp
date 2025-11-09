#ifndef SISTEMA_GERENCIADOR_HPP
#define SISTEMA_GERENCIADOR_HPP

#include <string>
#include <fstream>
#include <vector>
#include "Aluno.hpp"
#include "IndicePrimario.hpp"
#include "IndiceSecundario.hpp"
#include "NoListaInvertida.hpp"
#include "Auxiliar.hpp"

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
    SistemaGerenciador(const std::string &csv, const std::string &dados, const std::string &indicePrimario,
                       const std::string &indiceSecundario, const std::string &listaInvertida)
    {
        arquivoCSV = csv;
        arquivoDados = dados;
        arquivoIndicePrimario = indicePrimario;
        arquivoIndiceCurso = indiceSecundario;
        arquivoListainvertidaCurso = listaInvertida;
    };
    SistemaGerenciador() = default;
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
    bool lerRegistro(std::ifstream &in, Aluno &aluno, long offset);
    long obterEspaçoDisponivel();
    void adicionarEspaçoDisponivel(long offset);
};
#endif

template <typename T>

void escreverRegistro(std::ofstream &out, const T &objeto)
{
    out.write(reinterpret_cast<const char *>(&objeto), sizeof(T));
}