#include "../includes/SistemaGerenciador.hpp"
#include "../includes/Aluno.hpp"
#include "../includes/IndicePrimario.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

void SistemaGerenciador::gerarArquivos()
{
    //Abertura dos arquivos e criação de variáveis auxiliares
    Aluno aluno;
    std::ifstream fileCSV(arquivoCSV);
    std::ofstream fileBin(arquivoDados, std::ios::binary);
    std::string linha;

    //Verificações de abertura de arquivo
    if (!fileCSV)
    {
        std::cout << "Erro ao abrir o arquivo CSV" << std::endl;
    }

    if (!fileBin)
    {
        std::cout << "Erro ao abiri o arquivo binario" << std::endl;
    }
    //Pula o cabeçalho
    getline(fileCSV, linha);

    //Faz o parser dos campos 
    while (std::getline(fileCSV, linha))
    {
        aluno.parser(linha);

        // Salva no arquivo binario
        escreverRegistro(fileBin, aluno);
    }
    //Fechamento dos arquivos
    fileCSV.close();
    fileBin.close();
}

void SistemaGerenciador::escreverRegistro(std::ofstream &out, const Aluno &aluno)
{
    out.write(reinterpret_cast<const char *>(&aluno), sizeof(Aluno));
}

// Essa função vai servir para os indices
bool SistemaGerenciador::lerRegistro(std::ifstream &in, Aluno &aluno, long offset)
{
    //Posiciona o curso de leitura no inicio de cada registro de aluno
    in.seekg(offset);

    //Verifica se o curso do arquivo acessa uma zona válida
    if (!in)
        return false;

    // Tenta ler o tamanho do próximo registro. Se não conseguir retorna false.
    if (!in.read(reinterpret_cast<char *>(&aluno), sizeof(aluno)))
    {
        return false;
    }

    return true;
}

void buscarAlunoPorMatricula(int matricula, std::ifstream &in, std::vector<IndicePrimario> &indices)
{
    //Declara as variáveis auxiliares
    int ini = 0;
    int fim = indices.size() - 1;
    int meio;
    Aluno aluno;

    //Faz a busca enquanto a posição for válida
    while (ini <= fim)
    {
        meio = (ini + fim) / 2;

        //Se encontra a matrícula, imprime as informações do aluno
        if (indices[meio].matricula == matricula)
        {
            if (lerRegistro(in, aluno, indices[meio].byte_offset))
            {
                aluno.display();
            }
            return;
        }
        //Compara se a matricula esta na segunda metade do vetor
        else if (indices[meio].matricula < matricula)
        {
            ini = meio + 1;
        }
        //Compara se a matricula esta na primeira metade do vetor
        else
        {
            fim = meio - 1;
        }
    }

    std::cout << "Matricula não encontrada!!!" << std::endl;
}