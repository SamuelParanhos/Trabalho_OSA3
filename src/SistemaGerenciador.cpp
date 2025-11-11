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
    inicilizaVetores();
    IndicePrimario in;

    // menu de teste
    int opcao;
    do
    {
        std::cout << "Sistema Gerenciador de Alunos" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "1 - Inserir Aluno" << std::endl;
        std::cout << "2 - Remover Aluno por matrícula" << std::endl;
        std::cout << "3 - Buscar Aluno por matrícula" << std::endl;
        std::cout << "4 - Buscar Aluno por curso" << std::endl;
        std::cout << "0 - Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        switch (opcao)
        {
        case 1:
            int continuar;
            do
            {
                void inserirAluno();
                std::cout << "Deseja insereir mais um aluno? " << "\n"
                          << "1 - Sim" << "\n"
                          << "2 - Não";
                std::cin >> continuar;
            } while (continuar != 2);
            in.organizar(indices);
            int tam = sizeof(indicesSecundarios);
            organizaIndices(indicesSecundarios, tam);
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
    long offset = 0;
    
    std::ifstream fileDisponiveis(arquivoDisponiveis);

    Disponiveis disponivel;

    while (lerRegistro(fileDisponiveis, disponivel, offset))
    {
        disponiveis.push_back(disponivel);
        
    }

    fileDisponiveis.close();
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
    std::vector<IndicePrimario> indicePrimario;
    IndicePrimario in;

    // Faz a leitura e popula o vetor de indices
    while (lerRegistro(fileBin, aluno, offset))
    {
        IndicePrimario indice;
        indice.matricula = aluno.matricula;
        indice.byte_offset = offset;
        offset += sizeof(Aluno);
        indicePrimario.push_back(indice);
    }

    // Ordena o vetor
    in.organizar(indicePrimario); // Talvez tenha que mudar

    int tam = indices.size();

    // Grava cada registro do vetor em um arquivo
    for (int i = 0; i < tam; i++)
    {
        escreverRegistro(fileIndice, indices[i]);
    }

    // Fechamento dos arquivos
    fileIndice.close();
    fileBin.close();
    indices = indicePrimario;
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
    organizaIndices(aux, aux.size());

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

long SistemaGerenciador::buscarIndicePrimario(int matricula, int retorno)
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
            if(retorno == 1)
            return indices[meio].byte_offset;
            else
            return meio;
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
    long offset = buscarIndicePrimario(matricula, 1);

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
    std::ifstream fileBin(arquivoDados);
    std::ifstream listaInvertida(arquivoListainvertidaCurso);
    int matricula;

    std::cout << "Digite a matricula a ser removida" << std::endl;
    std::cin >> matricula;

    int offset = buscarIndicePrimario(matricula, 1);

    if (offset == -1)
    {
        std::cout << "O aluno buscado não existe!" << std::endl;
        return false;
    }

    adicionarEspaçoDisponivel(offset);

    int indice = buscarIndicePrimario(matricula, 2);
    
    indices.erase(indices.begin() + indice);

    Aluno aluno;
    lerRegistro(fileBin, aluno, offset);

    int indiceSec = buscarIndiceSecundario(aluno.curso);
    IndiceSecundario indiceSecundario = indicesSecundarios[indiceSec];
    NoListaInvertida no;

    if(!lerRegistro(listaInvertida, no, indiceSecundario.rrn_lista_invertida))
    return false;

    if(no.proximo_rrn == -1 && no.matricula_aluno == matricula)
    {
        indicesSecundarios[indice].rrn_lista_invertida = -1;
        return true;
    }
    
    if(no.proximo_rrn != -1 && no.matricula_aluno == matricula)
    {
        indicesSecundarios[indice].rrn_lista_invertida = no.proximo_rrn;
        return true;
    }

    NoListaInvertida noProximo;

    if(!lerRegistro(listaInvertida, noProximo, no.proximo_rrn))
    return false;
    
    while(noProximo.proximo_rrn != -1 && noProximo.matricula_aluno != matricula)
    {
        no = noProximo;
        
        if(!lerRegistro(listaInvertida, noProximo, noProximo.proximo_rrn))
        return false;
    }

    no.proximo_rrn = noProximo.proximo_rrn;
    return true;
}

void SistemaGerenciador::inserirAluno()
{
    std::ofstream fileBin(arquivoDados, std::ios::binary);

    // Dados do Aluno
    char nome[50];
    char curso[30];
    int matricula;
    long offset;

    std::cout << "Digite o nome do aluno: ";
    std::cin >> nome;
    std::cout << "Digite o curso do aluno: ";
    std::cin >> curso;
    std::cout << "Digite a matricula do aluno: ";
    std::cin >> matricula;

    // Verifica se a Matricula Existe
    long matriculaExiste;
    buscarIndicePrimario(matriculaExiste, 1);
    if (matricula != -1)
    {
        std::cout << "Matricula ja Existe";
        return;
    }

    long offset = obterEspaçoDisponivel();
    fileBin.seekp(offset);

    Aluno aluno(nome, curso, matricula);
    // Indice Primario

    if (offset == -1)
    {
        fileBin.seekp(std::ios::end);
    }
    IndicePrimario indicePrimario;
    indicePrimario.matricula = aluno.matricula;
    indicePrimario.byte_offset = offset;
    indices.push_back(indicePrimario);

    escreverRegistro(fileBin, aluno);
    fileBin.close();

    insereIndiceSecundario(aluno);
}

void SistemaGerenciador::insereIndiceSecundario(const Aluno &aluno)
{
    std::ofstream fileIndiceSecundario(arquivoIndiceSecundario, std::ios::binary);
    std::ofstream fileListaInvertida(arquivoListainvertidaCurso, std::ios::binary);

    long cursoExistente;
    IndiceSecundario indiceSecundario;

    cursoExistente = buscarIndiceSecundario(aluno.curso);

    NoListaInvertida novoNo;
    novoNo.matricula_aluno = aluno.matricula;
    novoNo.proximo_rrn = indicesSecundarios[cursoExistente].rrn_lista_invertida;

    if (cursoExistente == -1)
    {
        strcpy(indiceSecundario.curso, aluno.curso);
        indiceSecundario.rrn_lista_invertida = fileListaInvertida.tellp();

        indicesSecundarios.push_back(indiceSecundario);
    }
    else
    {
        indicesSecundarios[cursoExistente].rrn_lista_invertida = fileListaInvertida.tellp();
    }

    escreverRegistro(fileListaInvertida, novoNo);
}

long SistemaGerenciador::obterEspaçoDisponivel()
{
    for (int i = 0; i < disponiveis.size(); i++)
    {
        if (disponiveis[i].valido)
        {
            disponiveis[i].valido = false;
            return disponiveis[i].offset;
        }
    }
    return -1;
}
void SistemaGerenciador::adicionarEspaçoDisponivel(long offset)
{
    std::ofstream fileDisponiveis(arquivoDisponiveis, std::ios::binary);
    Disponiveis disponivel;
    disponivel.offset = offset;
    disponivel.valido = true;
    disponiveis.push_back(disponivel);
    escreverRegistro(fileDisponiveis, disponivel);
}
