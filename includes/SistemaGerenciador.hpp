#ifndef SISTEMA_GERENCIADOR_HPP
#define SISTEMA_GERENCIADOR_HPP

#include <string>
#include <fstream>
#include <vector>
#include "Aluno.hpp"
#include "IndicePrimario.hpp"
#include "IndiceSecundario.hpp"
#include "NoListaInvertida.hpp"
#include "Disponiveis.hpp"
#include "Auxiliar.hpp"
class SistemaGerenciador
{
private:
    std::string arquivoCSV;
    std::string arquivoDados;
    std::string arquivoIndicePrimario;
    std::string arquivoIndiceSecundario;
    std::string arquivoListainvertidaCurso;
    std::string arquivoDisponiveis;
    std::vector<IndicePrimario> indices;
    std::vector<IndiceSecundario> indicesSecundarios;
    std::vector<Disponiveis> disponiveis;

public:
    SistemaGerenciador(const std::string &csv, const std::string &dados, const std::string &indicePrimario,
                       const std::string &indiceSecundario, const std::string &listaInvertida,
                       const std::string &disponiveis)
    {
        arquivoCSV = csv;
        arquivoDados = dados;
        arquivoIndicePrimario = indicePrimario;
        arquivoIndiceSecundario = indiceSecundario;
        arquivoListainvertidaCurso = listaInvertida;
        arquivoDisponiveis = disponiveis;
    };
    SistemaGerenciador() = default;
    void iniciar();
    void gerarArquivos();
    void gerarArquivoIndicePrimario();
    void gerarArquivoIndiceSecundario();
    void inserirAluno();
    void buscarAlunoPorMatricula(int matricula, std::ifstream &in);
    void bucarAlunosPorCurso(std::string nomdeDoCurso);
    bool removerAlunoPorMatricula();
    void inicilizaVetores();
    void gerarAquivoDisponiveis();

private:
    long buscarIndicePrimario(int matricula, int retorno);
    long buscarIndiceSecundario(const std::string &curso, int opcao);
    long obterEspaçoDisponivel();
    void adicionarEspaçoDisponivel(long offset);
    void insereIndiceSecundario(const Aluno &aluno);
    void atualizar();
};

template <typename T>

void escreverRegistro(std::ostream &out, const T &objeto)
{
    out.write(reinterpret_cast<const char *>(&objeto), sizeof(T));
}

template <typename T>

bool lerRegistro(std::istream &in, T &t, long offset)
{
    // Posiciona o curso de leitura no inicio de cada registro de aluno
    in.seekg(offset);

    // Verifica se o curso do arquivo acessa uma zona válida
    if (!in)
        return false;

    // Tenta ler o tamanho do próximo registro. Se não conseguir retorna false.
    if (!in.read(reinterpret_cast<char *>(&t), sizeof(T)))
    {
        return false;
    }

    return true;
}

#endif