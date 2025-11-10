#include "../includes/SistemaGerenciador.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>

void SistemaGerenciador::iniciar()
{
    // menu de teste
    int opcao;
    do
    {
        std::cout << "Sistema Gerenciador de Alunos" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "1. Gerar Arquivo de Dados a partir do CSV" << std::endl;
        std::cout << "2 - Gerar Arquivo de indice" << std::endl;
        std::cout << "3 - Gerar Arquivo de indice Secundario" << std::endl;
        std::cout << "0 - Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        switch (opcao)
        {
        case 1:
            gerarArquivos();
            break;
        case 2:
            gerarArquivoIndicePrimario();
            break;
        case 3:
            gerarArquivoIndiceSecundario();
            break;
        }
    } while (opcao != 0);
}

void SistemaGerenciador::gerarArquivos()
{
    // Abertura dos arquivos e criação de variáveis auxiliares
    Aluno aluno;
    std::ifstream fileCSV(arquivoCSV);
    std::ofstream fileBin(arquivoDados, std::ios::binary);
    std::string linha;

    // Verificações de abertura de arquivo
    if (!fileCSV)
    {
        std::cout << "Erro ao abrir o arquivo CSV" << std::endl;
    }

    if (!fileBin)
    {
        std::cout << "Erro ao abiri o arquivo binario" << std::endl;
    }
    // Pula o cabeçalho
    getline(fileCSV, linha);

    // Faz o parser dos campos
    while (std::getline(fileCSV, linha))
    {
        aluno.parser(linha);

        // Salva no arquivo binario
        escreverRegistro(fileBin, aluno);
    }
    // Fechamento dos arquivos
    fileCSV.close();
    fileBin.close();
}

void SistemaGerenciador::gerarArquivoIndicePrimario()
{
    // Abertura dos arquivos e criação das variáveis auxiliares
    std::ifstream fileBin(arquivoDados, std::ios::binary);
    std::ofstream fileIndice(arquivoIndicePrimario);
    long offset = 0;
    Aluno aluno;
    std::vector<IndicePrimario> indices;
    IndicePrimario in;

    // Faz a leitura e popula o vetor de indices
    while (lerRegistro(fileBin, aluno, offset))
    {
        IndicePrimario indice;
        indice.matricula = aluno.matricula;
        indice.byte_offset = offset;
        offset += sizeof(Aluno);
        indices.push_back(indice);
    }

    // Ordena o vetor
    in.organizar(indices); // Talvez tenha que mudar

    int tam = indices.size();

    // Grava cada registro do vetor em um arquivo
    for (int i = 0; i < tam; i++)
    {
        escreverRegistro(fileIndice, indices[i]);
    }

    // Fechamento dos arquivos
    fileIndice.close();
    fileBin.close();
}

// Definição da estrutura que será salva na Lista Invertida
struct ItemListaInvertida
{
    std::string matricula;
    long rrn;
};

void SistemaGerenciador::gerarArquivoIndiceSecundario()
{
    std::ofstream fileIndice(arquivoIndiceSecundario);
    std::ifstream arquivoBin(arquivoDados, std::ios::binary);
    std::ofstream listaInvertida(ListaInvertida);

    long offset = 0;
    std::vector<IndiceSecundario> indiceSecundario;
    Aluno aluno;
    std::vector<Auxiliar> aux; // Vai pegar a Matricula

    // Leitura e Geração dos Índices
    while (lerRegistro(arquivoBin, aluno, offset))
    {
        Auxiliar temp;

        strcpy(temp.curso, aluno.curso);
        temp.matricula = aluno.matricula;
        temp.byte_offset = offset;

        aux.push_back(temp);

        offset += sizeof(Aluno);
    }
    IndiceSecundario in;

    // Ordena por curso
    in.organizaIndices(aux, aux.size());

    // Geração da Lista Invertida

    char curso_temp[30] = ""; // Guarda o curso atual
    int rrnAtual = 0;         // Guarda o offset da no atual da lista invertida
    int rrnAnterior = -1;     // Guarda o offset da no atual da lista invertida

    arquivoBin.close();

    // Itera sobre o vetor ordenado
    for (size_t i = 0; i < aux.size(); ++i)
    {
        // Se o curso mudar
        if (strcmp(curso_temp, aux[i].curso) != 0)
        {
            // Verificação para atualizar o último nó da lista invertida. Em vez dele apontar para o offset
            // de um outro curso ele aponta para -1.
            if (rrnAnterior != 0)
            {
                // Encontra o rrn que deve ser alterado
                long posParaCorrigir = (rrnAnterior * sizeof(NoListaInvertida)) + sizeof(int);

                long corrigir = -1;

                // Atualiza o curso
                listaInvertida.seekp(posParaCorrigir);

                // Encontra -1
                listaInvertida.write((char *)&corrigir, sizeof(long));

                // Atualizar o cursor para o final do arquivo
                listaInvertida.seekp(0, std::ios::end);
            }

            // Muda o curso_temp para armazenar um novo curso
            strcpy(curso_temp, aux[i].curso);

            // Preenche um índice secundário
            IndiceSecundario novoIndice;
            strcpy(novoIndice.curso, curso_temp);
            novoIndice.rrn_lista_invertida = rrnAtual;

            // Grava no arquivo
            escreverRegistro(fileIndice, novoIndice);
        }
        // Cria e preenche um novo nó
        NoListaInvertida no;
        no.matricula_aluno = aux[i].matricula;
        no.proximo_rrn = rrnAtual + 1;

        // Escreve o nó no arquivo ListaInvertida
        escreverRegistro(listaInvertida, no);

        // Atualiza as variáveis auxiliares
        rrnAnterior = rrnAtual;
        rrnAtual++;
    }
    if (rrnAnterior != -1)
    {
        // Encontra o rrn que deve ser alterado
        long posParaCorrigir = (rrnAnterior * sizeof(NoListaInvertida)) + sizeof(int);

        long corrigir = -1;

        // Atualiza o curso
        listaInvertida.seekp(posParaCorrigir);

        // Encontra -1
        listaInvertida.write((char *)&corrigir, sizeof(long));

        // Atualizar o cursor para o final do arquivo
        listaInvertida.seekp(0, std::ios::end);
    }

    fileIndice.close();
    listaInvertida.close();
}

// Essa função vai servir para os indices
bool SistemaGerenciador::lerRegistro(std::ifstream &in, Aluno &aluno, long offset)
{
    // Posiciona o curso de leitura no inicio de cada registro de aluno
    in.seekg(offset);

    // Verifica se o curso do arquivo acessa uma zona válida
    if (!in)
        return false;

    // Tenta ler o tamanho do próximo registro. Se não conseguir retorna false.
    if (!in.read(reinterpret_cast<char *>(&aluno), sizeof(aluno)))
    {
        return false;
    }

    return true;
}

long SistemaGerenciador::buscarIndicePrimario(int matricula, const std::vector<IndicePrimario> &indices)
{

    // Declara as variáveis auxiliares
    int ini = 0;
    int fim = indices.size() - 1;
    int meio;

    // Faz a busca enquanto a posição for válida
    while (ini <= fim)
    {
        meio = (ini + fim) / 2;

        // Se encontra a matrícula, imprime as informações do aluno
        if (indices[meio].matricula == matricula)
        {
            return indices[meio].byte_offset;
        }
        // Compara se a matricula esta na segunda metade do vetor
        else if (indices[meio].matricula < matricula)
        {
            ini = meio + 1;
        }
        // Compara se a matricula esta na primeira metade do vetor
        else
        {
            fim = meio - 1;
        }
    }

    return -1;
}

long SistemaGerenciador::buscarIndiceSecundario(const std::string &curso)
{
    std::ifstream fileIndice(arquivoIndiceSecundario, std::ios::binary);
    IndiceSecundario novoIndice;

    // fileIndice.read() tenta ler sizeof(IndiceSecundario) bytes para o novoIndice, se der certo retorna true
    while (fileIndice.read(reinterpret_cast<char *>(&novoIndice), sizeof(IndiceSecundario)))
    {
        if (strcmp(novoIndice.curso, curso.c_str()) == 0)
        {
            return novoIndice.rrn_lista_invertida;
        }
    }
    fileIndice.close();
    return -1;
}

void SistemaGerenciador::buscarAlunoPorMatricula(int matricula, std::ifstream &in, std::vector<IndicePrimario> &indices)
{
    Aluno aluno;
    // Busca o indice Primario
    long offset = buscarIndicePrimario(matricula, indices);

    if (offset != 1)
    {
        // Lê o registro no arquivo de dados
        if (lerRegistro(in, aluno, offset))
        {
            aluno.display();
            return;
        }
    }

    std::cout << "Matricula não encontrada!!!" << std::endl;
}

void SistemaGerenciador::bucarAlunosPorCurso(std::string nomdeDoCurso)
{
    long rrn = buscarIndiceSecundario(nomdeDoCurso);

    if (rrn == -1)
    {
        std::cout <<"Curso não encontrado";
        return;
    }

    std::ifstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary);

    //Pecorre a Lista
    while(rrn != -1){
        //calcula o offset
        long offsetNoLista = rrn * sizeof(NoListaInvertida);
        listaInvertida.seekg(offsetNoLista);
        NoListaInvertida no;
        listaInvertida.read(reinterpret_cast<char*>(&no), sizeof(NoListaInvertida));

        //tem que pensar em alguma forma de usar essa informação

        rrn = no.proximo_rrn;
    }

    listaInvertida.close();

}

bool SistemaGerenciador::removerAlunoPorMatricula()
{
    int matricula;

    std::cout << "Digite a matricula a ser removida" << std::endl;
    std::cin >> matricula;

    // int indice = buscaIndicePrimario(matricula, );
}
// Modificar o heap, para inserção e remoção
// Criar os índices secundarios
// Alterar o menu
// Implementar a politica de remoção usando lista de disponíveis.
