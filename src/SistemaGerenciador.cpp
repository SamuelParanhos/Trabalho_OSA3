#include "../includes/SistemaGerenciador.hpp"
#include "../includes/QuickSort.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>

void SistemaGerenciador::iniciar()
{
    //menu de teste
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
        fileIndice << indices[i].matricula << ' ' << indices[i].byte_offset << '\n';
    }

    // Fechamento dos arquivos
    fileIndice.close();
    fileBin.close();
}

// Definição da estrutura que será salva na Lista Invertida
struct ItemListaInvertida {
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
    std::vector<Aluno> alunos; // Vai pegar a Matricula

    QuickSort<IndiceSecundario> organizarIn;
    QuickSort<Aluno> organizaAluno; 

    //Leitura e Geração dos Índices
    while (lerRegistro(arquivoBin, aluno, offset))
    {
        IndiceSecundario indice;

        strcpy(indice.curso, aluno.curso); 
        indice.rrn_lista_invertida = offset; 

        indiceSecundario.push_back(indice);
        
        alunos.push_back(aluno);
        offset += sizeof(Aluno);
    }
    int tam = indiceSecundario.size();

    //Ordena por curso
    organizarIn.organizaIndices(indiceSecundario, tam);
    organizaAluno.organizaIndices(alunos, alunos.size()); 

    // Geração da Lista Invertida 

    long inicio = 0; // Offset no arquivo ListaInvertida
    char curso_temp[30] = ""; 
    
    // Vetor de agrupamento guarda Matrícula + RRN
    std::vector<ItemListaInvertida> itensLista; 

    // Inicializa curso_temp
    strcpy(curso_temp, indiceSecundario[0].curso);
    
    // Itera sobre o vetor ordenado
    for (size_t i = 0; i < indiceSecundario.size(); ++i)
    {
        // Se o curso mudar 
        if (strcmp(curso_temp, indiceSecundario[i].curso) != 0)
        {
            // Escreve o grupo anterior pares de Matrícula/RRN
            for (const auto& item : itensLista)
            {
                // Escreve o par Matrícula e RRN separados por espaço.
                listaInvertida << item.matricula << ":" << item.rrn << " "; 
            }
            listaInvertida << "\n";

            //Captura o novo offset e escreve o índice do grupo anterior
            long novaPosi = listaInvertida.tellp(); 
            // Salva o índice secundário: Curso | Offset_Lista_Invertida
            fileIndice << curso_temp << " | " << inicio << "\n";

            //Prepara para o próximo curso
            itensLista.clear();
            strcpy(curso_temp, indiceSecundario[i].curso);
            inicio = novaPosi;
        }

        //Adiciona o par Matrícula + RRN ao vetor de agrupamento
        ItemListaInvertida novoItem;
        
        // Pega o RRN já salvo na estrutura de índice
        novoItem.rrn = indiceSecundario[i].rrn_lista_invertida; 
        
        //  Pega a Matrícula do vetor alunos, que está na mesma posição 'i'
        novoItem.matricula = alunos[i].matricula; 
        
        itensLista.push_back(novoItem);
    }
    
    // Tratamento do ultimo curso
    
    if (!itensLista.empty()) {
        for (const auto& item : itensLista)
        {
            listaInvertida << item.matricula << ":" << item.rrn << " ";
        }
        listaInvertida << "\n";

        fileIndice << curso_temp << " | " << inicio << "\n";
    }

    fileIndice.close();
    arquivoBin.close();
    listaInvertida.close();
}
void SistemaGerenciador::escreverRegistro(std::ofstream &out, const Aluno &aluno)
{
    out.write(reinterpret_cast<const char *>(&aluno), sizeof(Aluno));
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

// Modificar o heap, para inserção e remoção
// Criar os índices secundarios
// Alterar o menu
// Implementar a politica de remoção usando lista de disponíveis.


