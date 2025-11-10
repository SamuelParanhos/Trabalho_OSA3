#include "../includes/SistemaGerenciador.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>

void SistemaGerenciador::iniciar()
{
    gerarArquivos();
    gerarArquivoIndicePrimario();
    gerarArquivoIndiceSecundario();

    // menu de teste
    int opcao;
    do
    {
        std::cout << "Sistema Gerenciador de Alunos" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "1. Inserir Aluno" << std::endl;
        std::cout << "2 - Remover Aluno por matrícula" << std::endl;
        std::cout << "3 - Buscar Aluno por matrícula" << std::endl;
        std::cout << "4 - Buscar Aluno por curso" << std::endl;
        std::cout << "0 - Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        switch (opcao)
        {
        case 1:
            void inserirAluno();
            break;

        case 2:
            removerAlunoPorMatricula();
            break;

        case 3:
        {
            int matricula;
            std::cout << "Digite a matrícula desejada" << std::endl;
            std::cin >> matricula;
            std::ifstream fileBin(arquivoDados, std::ios::binary);
            buscarAlunoPorMatricula(matricula, fileBin);
            fileBin.close();
            break;
        }
        case 4:
            char curso[30];
            std::cout << "Digite o curso desejada" << std::endl;
            std::cin >> curso;
            bucarAlunosPorCurso(curso);
            break;
        }

    } while (opcao != 0);
}
void SistemaGerenciador::inicilizaVetores()
{
    std::ifstream indicePrimario(arquivoIndicePrimario);
    std::ifstream indiceSecundario(arquivoIndiceSecundario);
    std::ifstream disponiveis(arquivoDisponiveis);

    std::vector<IndicePrimario> indicesPrimarios;
    std::vector<IndiceSecundario> indicesSecundarios;
    // Criar parte de disponíveis

    IndicePrimario primario;
    IndiceSecundario secundario;
    // Criar a parte de disponiveis

    long offset = 0;

    while (lerRegistro(indicePrimario, primario, offset))
    {
        IndicePrimario indice;
        indice.matricula = primario.matricula;
        indice.byte_offset = offset;
        offset += sizeof(IndicePrimario);
        indicesPrimarios.push_back(indice);
    }

    while (lerRegistro(indicePrimario, secundario, offset))
    {
        IndiceSecundario indice;
        strcpy(indice.curso, secundario.curso);
        indice.rrn_lista_invertida = secundario.rrn_lista_invertida;
        offset += sizeof(IndiceSecundario);
        indicesSecundarios.push_back(indice);
    }
    /*
    while (lerRegistro(indicePrimario, primario, offset))
    {
        IndicePrimario indice;
        indice.matricula = primario.matricula;
        indice.byte_offset = offset;
        offset += sizeof(IndicePrimario);
        indicesPrimarios.push_back(indice);
    }
    */
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

long SistemaGerenciador::buscarIndicePrimario(int matricula)
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
    IndiceSecundario novoIndice;

    for (int i = 0; i < indicesSecundarios.size(); i++)
    {
        if (strcmp(curso.c_str(), indicesSecundarios[i].curso) == 0)
        {
            return indicesSecundarios[i].rrn_lista_invertida;
        }
    }
    return -1;
}

void SistemaGerenciador::buscarAlunoPorMatricula(int matricula, std::ifstream &in)
{
    int matricula;
    std::cout << "Digite a matricula desejada" << std::endl;
    std::cin >> matricula;
    Aluno aluno;
    // Busca o indice Primario
    long offset = buscarIndicePrimario(matricula);

    if (offset != -1)
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
    std::ifstream fileIndice(arquivoDados, std::ios::binary);
    long rrn = buscarIndiceSecundario(nomdeDoCurso);

    if (rrn == -1)
    {
        std::cout << "Curso não encontrado";
        return;
    }

    std::ifstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary);

    // Pecorre a Lista
    while (rrn != -1)
    {
        // calcula o offset
        long offsetNoLista = rrn * sizeof(NoListaInvertida);
        listaInvertida.seekg(offsetNoLista);
        NoListaInvertida no;
        listaInvertida.read(reinterpret_cast<char *>(&no), sizeof(NoListaInvertida));

        buscarAlunoPorMatricula(no.matricula_aluno, fileIndice);
        rrn = no.proximo_rrn;
    }

    listaInvertida.close();
    fileIndice.close();
}

bool SistemaGerenciador::removerAlunoPorMatricula()
{
    int matricula;

    std::cout << "Digite a matricula a ser removida" << std::endl;
    std::cin >> matricula;

    // int indice = buscaIndicePrimario(matricula, );
}
void inserirAluno()
{
}
// Modificar o heap, para inserção e remoção
// Criar os índices secundarios
// Alterar o menu
// Implementar a politica de remoção usando lista de disponíveis.
