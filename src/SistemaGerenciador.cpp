#include "../includes/SistemaGerenciador.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>

void SistemaGerenciador::iniciar()
{
    std::cout<<"teste"<<std::endl;
    inicilizaVetores();
     std::cout<<"teste2"<<std::endl;
    // menu de teste
    int opcao;
    do
    {
        std::cout << "Sistema Gerenciador de Alunos" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "1 - Gerar Arquivo de Dados" << std::endl;
        std::cout << "2 - Gerar Arquivo de Indices Primários" << std::endl;
        std::cout << "3 - Gerar Arquivo de Indices Secundários" << std::endl;
        std::cout << "4 - Gerar Arquivo de Disponíveis" << std::endl;
        std::cout << "5 - Inserir Aluno" << std::endl;
        std::cout << "6 - Remover Aluno por matrícula" << std::endl;
        std::cout << "7 - Buscar Aluno por matrícula" << std::endl;
        std::cout << "8 - Buscar Aluno por curso" << std::endl;
        std::cout << "0 - Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        if (!(std::cin >> opcao)) {
            std::cin.clear();
            opcao = -1; // Para evitar loop infinito em caso de input inválido
            std::cout << "Opção inválida, tente novamente." << std::endl;
            continue;
        }

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

        case 4:
            gerarAquivoDisponiveis();
            break;

        case 5:
        {
            int continuar;
            IndicePrimario in;
            do
            {
                inserirAluno();
                std::cout << "Deseja insereir mais um aluno? " << "\n"
                          << "1 - Sim" << "\n"
                          << "2 - Não" << "\n";
                std::cin >> continuar;
            } while (continuar != 2);
            in.organizar(indices);
            int tam = indicesSecundarios.size();
            organizaIndices(indicesSecundarios, tam);
            break;
        }
        case 6:
            removerAlunoPorMatricula();
            break;

        case 7:
        {
            int matricula;
            std::cout << "Digite a matrícula desejada" << std::endl;
            std::cin >> matricula;
            std::ifstream fileBin(arquivoDados, std::ios::binary);
            buscarAlunoPorMatricula(matricula, fileBin);
            fileBin.close();
            break;
        }
        case 8:
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
    disponiveis.clear();
    indices.clear();
    indicesSecundarios.clear();
   std::cout<<"Apos limpeza"<<std::endl;
    long offset = 0;
    std::ifstream fileDisponiveis(arquivoDisponiveis);
    std::ifstream fileIndicePrimario(arquivoIndicePrimario);
    std::ifstream fileIndiceSecundario(arquivoIndiceSecundario);

    Disponiveis disponivel;

    while (lerRegistro(fileDisponiveis, disponivel, offset))
    {
        disponiveis.push_back(disponivel);
    }
    std::cout<<"Apos leitura de Disponiveis"<<std::endl;

    offset = 0;
    IndicePrimario indice_primario;

    while (lerRegistro(fileIndicePrimario, indice_primario, offset))
    {
        indices.push_back(indice_primario);
    }
    std::cout<<"Apos leitura de IndicesPri"<<std::endl;

    offset = 0;
    IndiceSecundario indice_secundario;

    while (lerRegistro(fileIndiceSecundario, indice_secundario, offset))
    {
        indicesSecundarios.push_back(indice_secundario);
    }
    std::cout<<"Apos leitura de IndiceSec"<<std::endl;

    fileDisponiveis.close();
    fileIndicePrimario.close();
    fileIndiceSecundario.close();
    std::cout<<"Fim da funçaõ"<<std::endl;
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
    indices.clear();
    // Abertura dos arquivos e criação das variáveis auxiliares
    std::ifstream fileBin(arquivoDados, std::ios::binary);
    std::ofstream fileIndice(arquivoIndicePrimario);
    long offset = 0;
    Aluno aluno;
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
/*struct ItemListaInvertida
{
????????
    std::string matricula;
    long rrn;
};*/

void SistemaGerenciador::gerarArquivoIndiceSecundario()
{
    indicesSecundarios.clear();
    std::cout<<"Apos Limpeza"<<std::endl;

    std::ofstream fileIndice(arquivoIndiceSecundario, std::ios::binary);
    std::ifstream arquivoBin(arquivoDados, std::ios::binary);
    std::ofstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary);

    if (!arquivoBin.is_open())
    {
        std::cout << "Erro ao abrir o arquivo de dados para geração do índice secundário." << std::endl;
        return;
    }
    if (!fileIndice.is_open())
    {
        std::cout << "Erro ao abrir o arquivo de índice secundário para escrita." << std::endl;
        return;
    }
    if (!listaInvertida.is_open())
    {
        std::cout << "Erro ao abrir o arquivo de lista invertida para escrita." << std::endl;
        return;
    }

    long offset = 0;
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
    std::cout<<"Apos Geração"<<std::endl;

    // Ordena por curso
    organizaIndices(aux, aux.size());
    std::cout<<"Apos Organizar"<<std::endl;

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
                listaInvertida.write(reinterpret_cast<char *>(&corrigir), sizeof(long));

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
            indicesSecundarios.push_back(novoIndice);
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
        listaInvertida.write(reinterpret_cast<char *>(&corrigir), sizeof(long));

        // Atualizar o cursor para o final do arquivo
        listaInvertida.seekp(0, std::ios::end);
    }
    std::cout<<"Apos Lista"<<std::endl;
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
            if (retorno == 1)
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

long SistemaGerenciador::buscarIndiceSecundario(const std::string &curso, int opcao)
{

    for (size_t i = 0; i < indicesSecundarios.size(); i++)
    {
        if (strcmp(curso.c_str(), indicesSecundarios[i].curso) == 0)
        {
            if (opcao == 1)
                return indicesSecundarios[i].rrn_lista_invertida;
            else
                return i;
        }
    }
    return -1;
}

void SistemaGerenciador::buscarAlunoPorMatricula(int matricula, std::ifstream &in)
{
    Aluno aluno;
    // Busca o indice Primario
    long offset = buscarIndicePrimario(matricula, 1);

    if (offset != -1)
    {
        // Lê o registro no arquivo de dados
        if (lerRegistro(in, aluno, offset))
        {
            std::cout << "Aluno encontrado:" << std::endl;
            aluno.display();
            return;
        }
    }

    std::cout << "Matricula não encontrada!!!" << std::endl;
}

void SistemaGerenciador::bucarAlunosPorCurso(std::string nomdeDoCurso)
{
    std::ifstream fileDados(arquivoDados, std::ios::binary);

    if (!fileDados.is_open())
    {
        std::cout << "Erro: Arquivo de dados não encontrado/aberto para busca por curso." << std::endl;
        return;
    }

    long rrn = buscarIndiceSecundario(nomdeDoCurso, 1);

    if (rrn == -1)
    {
        std::cout << "Curso não encontrado";
        return;
    }

    std::cout << "Alunos encontrados no curso '" << nomdeDoCurso << "':" << std::endl;
    std::ifstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary);

    if (!listaInvertida.is_open())
    {
        std::cout << "Erro: Arquivo de lista invertida não encontrado/aberto." << std::endl;
        fileDados.close();
        return;
    }

    // Pecorre a Lista
    while (rrn != -1)
    {
        // calcula o offset
        long offsetNoLista = rrn * sizeof(NoListaInvertida);
        listaInvertida.seekg(offsetNoLista);
        NoListaInvertida no;
        listaInvertida.read(reinterpret_cast<char *>(&no), sizeof(NoListaInvertida));

        fileDados.clear();
        // volta para o inicio
        fileDados.seekg(0, std::ios::beg);
        buscarAlunoPorMatricula(no.matricula_aluno, fileDados);
        rrn = no.proximo_rrn;
    }

    listaInvertida.close();
    fileDados.close();
}

bool SistemaGerenciador::removerAlunoPorMatricula()
{
    std::fstream fileBin(arquivoDados, std::ios::binary | std::ios::in | std::ios::out);
    std::fstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary | std::ios::in | std::ios::out);
    int matricula;

    std::cout << "Digite a matricula a ser removida" << std::endl;
    std::cin >> matricula;

    int offset = buscarIndicePrimario(matricula, 1);
    int indicePrimarioVector = buscarIndicePrimario(matricula, 2);

    if (offset == -1)
    {
        std::cout << "O aluno buscado não existe!" << std::endl;
        return false;
    }

    adicionarEspaçoDisponivel(offset);

    indices.erase(indices.begin() + indicePrimarioVector);

    Aluno aluno;
    fileBin.clear();
    lerRegistro(fileBin, aluno, offset);
    fileBin.close();

    int indiceSec = buscarIndiceSecundario(aluno.curso, 2);
    if (indiceSec == -1)
    {
        // Isso não deveria acontecer se o aluno existia.
        std::cout << "Erro lógico: Curso não encontrado no índice secundário." << std::endl;
        listaInvertida.close();
        return false;
    }

    long rrnCabecaLista = indicesSecundarios[indiceSec].rrn_lista_invertida;

    // Caso 1: O nó a ser removido é a cabeça da lista
    NoListaInvertida noCabeca;
    listaInvertida.clear();
    listaInvertida.seekg(rrnCabecaLista * sizeof(NoListaInvertida));
    listaInvertida.read(reinterpret_cast<char *>(&noCabeca), sizeof(NoListaInvertida));

    if (noCabeca.matricula_aluno == matricula)
    {
        // A nova cabeça da lista é o próximo nó
        long novoRrnCabeca = noCabeca.proximo_rrn;
        indicesSecundarios[indiceSec].rrn_lista_invertida = novoRrnCabeca;
        listaInvertida.close();
        return true;
    }

    // Caso 2: O nó a ser removido está no meio ou no final da lista
    NoListaInvertida noAnterior;
    long rrnAnterior = rrnCabecaLista;

    // Pecorre a lista para encontrar o nó anterior ao nó que será removido
    while (rrnAnterior != -1)
    {
        // Lê o nó anterior (começa na cabeça)
        listaInvertida.clear();
        listaInvertida.seekg(rrnAnterior * sizeof(NoListaInvertida));
        listaInvertida.read(reinterpret_cast<char *>(&noAnterior), sizeof(NoListaInvertida));

        long rrnProximo = noAnterior.proximo_rrn;

        if (rrnProximo == -1)
        {
            std::cout << "Erro: Nó não encontrado na lista invertida (deveria ter sido a cabeça)." << std::endl;
            listaInvertida.close();
            return false;
        }

        // Lê o próximo nó
        NoListaInvertida noProximo;
        listaInvertida.clear();
        listaInvertida.seekg(rrnProximo * sizeof(NoListaInvertida));
        listaInvertida.read(reinterpret_cast<char *>(&noProximo), sizeof(NoListaInvertida));

        // Se o próximo nó é o que queremos remover
        if (noProximo.matricula_aluno == matricula)
        {
            // "Pula" o nó removido: Anterior.proximo_rrn = Removido.proximo_rrn
            noAnterior.proximo_rrn = noProximo.proximo_rrn;

            // Re-escreve o nó anterior com o novo ponteiro
            listaInvertida.clear();
            listaInvertida.seekp(rrnAnterior * sizeof(NoListaInvertida));
            listaInvertida.write(reinterpret_cast<char *>(&noAnterior), sizeof(NoListaInvertida));

            listaInvertida.close();
            return true;
        }

        rrnAnterior = rrnProximo; // Continua para o próximo nó
    }

    std::cout << "Erro: Não foi possível remover o aluno da lista invertida." << std::endl;
    listaInvertida.close();
    return false;
}

void SistemaGerenciador::inserirAluno()
{
    std::fstream fileBin(arquivoDados, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

    // Dados do Aluno
    char nome[50];
    char curso[30];
    int matricula;
    long offset;
    long matriculaExiste;

    std::cout << "Digite o nome do aluno: ";
    std::cin >> nome;
    std::cout << "Digite o curso do aluno: ";
    std::cin >> curso;
    std::cout << "Digite a matricula do aluno: ";
    std::cin >> matricula;

    // Verifica se a Matricula Existe
    matriculaExiste = buscarIndicePrimario(matricula, 1);
    if (matriculaExiste != -1)
    {
        std::cout << "Matricula ja Existe";
        return;
    }

    offset = obterEspaçoDisponivel();

    if (offset == -1)
    {
        fileBin.seekp(0, std::ios::end);
        offset = fileBin.tellp();
    }
    else
    {
        fileBin.seekp(offset);
    }

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
    std::cout << "Aluno inserido com sucesso na posição " << offset << std::endl;
}

void SistemaGerenciador::insereIndiceSecundario(const Aluno &aluno)
{
    std::fstream fileListaInvertida(arquivoListainvertidaCurso, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

    if (!fileListaInvertida.is_open())
    {
        std::cout << "Erro ao abrir o arquivo da Lista Invertida." << std::endl;
        return;
    }

    // long cursoExistente;
    int indice;
    IndiceSecundario indiceSecundario;

    // O RRN onde o NOVO nó da lista invertida será escrito
    long novo_rrn_lista_invertida = fileListaInvertida.tellp() / sizeof(NoListaInvertida);

    // cursoExistente = buscarIndiceSecundario(aluno.curso, 1);
    indice = buscarIndiceSecundario(aluno.curso, 2);

    long rrn_cabeca_anterior;

    if (indice == -1)
    {
        rrn_cabeca_anterior = -1;
        strcpy(indiceSecundario.curso, aluno.curso);
        indiceSecundario.rrn_lista_invertida = novo_rrn_lista_invertida;
        indicesSecundarios.push_back(indiceSecundario);
        indice = indicesSecundarios.size() - 1;
    }
    else
    {
        rrn_cabeca_anterior = indicesSecundarios[indice].rrn_lista_invertida;
        indicesSecundarios[indice].rrn_lista_invertida = novo_rrn_lista_invertida;
    }

    NoListaInvertida novoNo;
    novoNo.matricula_aluno = aluno.matricula;
    novoNo.proximo_rrn = rrn_cabeca_anterior;
    escreverRegistro(fileListaInvertida, novoNo);

    fileListaInvertida.close();
}

long SistemaGerenciador::obterEspaçoDisponivel()
{
    for (size_t i = 0; i < disponiveis.size(); i++)
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
    std::fstream fileDisponiveis(arquivoDisponiveis, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);
    Disponiveis disponivel;
    disponivel.offset = offset;
    disponivel.valido = true;
    disponiveis.push_back(disponivel);
    escreverRegistro(fileDisponiveis, disponivel);
}
void SistemaGerenciador::gerarAquivoDisponiveis()
{
    // Cria/sobrescreve o arquivo
    std::ofstream fileDisponiveis(arquivoDisponiveis);

    if (!fileDisponiveis.is_open())
    {
        std::cout << "Erro ao criar o arquivo de Disponíveis." << std::endl;
        return;
    }

    for (const auto &disp : disponiveis)
    {
        escreverRegistro(fileDisponiveis, disp);
    }

    fileDisponiveis.close();
}
