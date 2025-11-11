#include "../includes/SistemaGerenciador.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>

void SistemaGerenciador::iniciar()
{
    inicilizaVetores();
    // menu de teste
    int opcao;
    do
    {
        std::cout << "Sistema Gerenciador de Alunos" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "1 - Gerar arquivos" << std::endl;
        std::cout << "2 - Inserir Aluno" << std::endl;
        std::cout << "3 - Remover Aluno por matrícula" << std::endl;
        std::cout << "4 - Buscar Aluno por matrícula" << std::endl;
        std::cout << "5 - Buscar Aluno por curso" << std::endl;
        std::cout << "0 - Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        if (!(std::cin >> opcao))
        {
            std::cin.clear();
            opcao = -1; // Para evitar loop infinito em caso de input inválido
            std::cout << "Opção inválida, tente novamente." << std::endl;
            continue;
        }

        switch (opcao)
        {
        case 1:
            // Gera os arquivos necessários para o programa rodar
            gerarArquivos();
            gerarArquivoIndicePrimario();
            gerarArquivoIndiceSecundario();
            gerarAquivoDisponiveis();
            break;

        case 2:
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
                in.organizar(indices);
            } while (continuar != 2);
            int tam = indicesSecundarios.size();
            organizaIndices(indicesSecundarios, tam);
            break;
        }
        case 3:
            removerAlunoPorMatricula();
            break;

        case 4:
        {
            int matricula;
            std::cout << "Digite a matrícula desejada" << std::endl;
            std::cin >> matricula;
            std::ifstream fileBin(arquivoDados, std::ios::binary);
            buscarAlunoPorMatricula(matricula, fileBin);
            fileBin.close();
            break;
        }
        case 5:
            char curso[30];
            std::cout << "Digite o curso desejada" << std::endl;
            std::cin >> curso;
            bucarAlunosPorCurso(curso);
            break;
        }

    } while (opcao != 0);
    atualizar();
}
void SistemaGerenciador::inicilizaVetores()
{
    // Limpa os vetores
    disponiveis.clear();
    indices.clear();
    indicesSecundarios.clear();

    // Variável usada para percorrer os arquivos
    long offset = 0;

    // Abertura dos arquivos
    std::ifstream fileDisponiveis(arquivoDisponiveis, std::ios::binary);
    std::ifstream fileIndicePrimario(arquivoIndicePrimario, std::ios::binary);
    std::ifstream fileIndiceSecundario(arquivoIndiceSecundario, std::ios::binary);

    Disponiveis disponivel;

    // Populando o vetor de disponíveis
    while (lerRegistro(fileDisponiveis, disponivel, offset))
    {
        disponiveis.push_back(disponivel);
        offset += sizeof(Disponiveis);
    }

    offset = 0;
    IndicePrimario indice_primario;

    // Populando o vetor de indices primário
    while (lerRegistro(fileIndicePrimario, indice_primario, offset))
    {
        indices.push_back(indice_primario);
        offset += sizeof(IndicePrimario);
    }

    offset = 0;
    IndiceSecundario indice_secundario;

    // Populando o vetor de indices secundários
    while (lerRegistro(fileIndiceSecundario, indice_secundario, offset))
    {
        indicesSecundarios.push_back(indice_secundario);
        offset += sizeof(IndiceSecundario);
    }
    std::cout << "Apos leitura de IndiceSec" << std::endl;

    // Fechamento dos arquivos
    fileDisponiveis.close();
    fileIndicePrimario.close();
    fileIndiceSecundario.close();
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

void SistemaGerenciador::gerarArquivoIndiceSecundario()
{
    // Limpa o vetor de indíces secundarios
    indicesSecundarios.clear();

    // Abertura do arquivo
    std::ofstream fileIndice(arquivoIndiceSecundario, std::ios::binary);
    std::ifstream arquivoBin(arquivoDados, std::ios::binary);
    std::ofstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary);

    // Verificando se os arquivos foram abertos
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

    // Ordena por curso
    organizaIndices(aux, aux.size());
    std::cout << "Apos Organizar" << std::endl;

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
    std::cout << "Apos Lista" << std::endl;
    fileIndice.close();
    listaInvertida.close();
}

void SistemaGerenciador::atualizar()
{
    //Atualiza o arquivo de indice primario
    std::ofstream fileIndice(arquivoIndicePrimario, std::ios::binary);
    fileIndice.clear();
    for (size_t i = 0; i < indices.size(); i++)
    {
        escreverRegistro(fileIndice, indices[i]);
    }

    //Atualiza o arquivo de indice secundario
    std::ofstream fileIndiceSec(arquivoIndiceSecundario, std::ios::binary);
    fileIndiceSec.clear();

    for (size_t i = 0; i < indicesSecundarios.size(); i++)
    {
        escreverRegistro(fileIndiceSec, indicesSecundarios[i]);
    }

    //Atualizar o arquivo de Disponiveis
    std::ofstream fileDisponiveis(arquivoDisponiveis, std::ios::binary);
    fileDisponiveis.clear();
    
    for(size_t i = 0; i < disponiveis.size(); i++)
    {
        escreverRegistro(fileDisponiveis, disponiveis[i]);
    }
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
    // Declara as variáveis auxiliares
    int ini = 0;
    int fim = indicesSecundarios.size() - 1;
    int meio;

    // Faz a busca binária enquanto a posição for válida
    while (ini <= fim)
    {
        meio = (ini + fim) / 2;
        
        int comparacao = strcmp(curso.c_str(), indicesSecundarios[meio].curso);
        
        // Se encontra o curso (comparacao == 0)
        if (comparacao == 0)
        {
            if (opcao == 1)
                return indicesSecundarios[meio].rrn_lista_invertida; // Retorna o RRN
            else
                return meio; // Retorna o índice do vetor
        }
        // Se o curso buscado é "maior" (vem depois do 'meio')
        else if (comparacao > 0)
        {
            ini = meio + 1;
        }
        // Se o curso buscado é "menor" (vem antes do 'meio')
        else
        {
            fim = meio - 1;
        }
    }

    return -1; // Curso não encontrado
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
    // Abre o arquivo
    std::ifstream fileDados(arquivoDados, std::ios::binary);

    // Verifica a abertura do arquivo
    if (!fileDados.is_open())
    {
        std::cout << "Erro: Arquivo de dados não encontrado/aberto para busca por curso." << std::endl;
        return;
    }

    // busca a cabeça da lista do curso buscado
    long rrn = buscarIndiceSecundario(nomdeDoCurso, 1);

    // Verifica se o curso existe
    if (rrn == -1)
    {
        std::cout << "Curso não encontrado";
        return;
    }

    // Abertura do arquivo
    std::ifstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary);

    // Verifica abertura do arquivo
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

        // Passa pelo nó de cada lista e le o nó
        listaInvertida.seekg(offsetNoLista);
        NoListaInvertida no;
        listaInvertida.read(reinterpret_cast<char *>(&no), sizeof(NoListaInvertida));

        fileDados.clear();
        // volta para o inicio
        fileDados.seekg(0, std::ios::beg);

        // Chama a função de busca para encontrar o aluno e imprimir ele na tela
        buscarAlunoPorMatricula(no.matricula_aluno, fileDados);

        // Atualiza o rrn
        rrn = no.proximo_rrn;
    }

    listaInvertida.close();
    fileDados.close();
}

bool SistemaGerenciador::removerAlunoPorMatricula()
{
    // Abertura do arquivo
    std::fstream fileBin(arquivoDados, std::ios::binary | std::ios::in | std::ios::out);
    std::fstream listaInvertida(arquivoListainvertidaCurso, std::ios::binary | std::ios::in | std::ios::out);
    int matricula;

    std::cout << "Digite a matricula a ser removida" << std::endl;
    std::cin >> matricula;

    // Busca o offset da matricula a ser removida
    int offset = buscarIndicePrimario(matricula, 1);

    // Busca o índice da matricula a ser removida
    int indicePrimarioVector = buscarIndicePrimario(matricula, 2);

    // Verifica se a matrícula existe
    if (offset == -1)
    {
        std::cout << "O aluno buscado não existe!" << std::endl;
        return false;
    }

    // Adiciona um espaço disponível no vetor de espaço
    adicionarEspaçoDisponivel(offset);

    // Apaga o índice que contém a matricula, do vetor de índices

    indices.erase(indices.begin() + indicePrimarioVector);

    // Limpa e fecha o arquivo de dados
    Aluno aluno;
    fileBin.clear();
    //Usa o aluno para encontra o índice no vetor de indices secundários
    lerRegistro(fileBin, aluno, offset);
    fileBin.close();

    //Busca o elemento do vetor de indices secundarios correspondente ao curso do 
    //aluno a ser removido
    
    int indiceSec = buscarIndiceSecundario(aluno.curso, 2);
    
    //Verifica se o curso existe
    if (indiceSec == -1)
    {
        // Isso não deveria acontecer se o aluno existia.
        std::cout << "Erro lógico: Curso não encontrado no índice secundário." << std::endl;
        listaInvertida.close();
        return false;
    }
    
    //Recebe a cabeça da lista invertida
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

    //Recebe o offset do espaço disponivel no vetor de dados,
    //caso ele exista, se não existir insere no final do arquivo
    offset = obterEspaçoDisponivel();

    if (offset == -1)
    {
        fileBin.seekp(0, std::ios::end);
        offset = fileBin.tellp();
    }
    else{

        fileBin.seekp(offset);
    }

    Aluno aluno(nome, curso, matricula);
    // Indice Primario

    if (offset == -1)
    {
        fileBin.seekp(std::ios::end);
    }

    //Cria um novo indice primario e insere no vetor
    IndicePrimario indicePrimario;
    indicePrimario.matricula = aluno.matricula;
    indicePrimario.byte_offset = offset;
    indices.push_back(indicePrimario);

    //Escreve o aluno no arquivo de dados
    escreverRegistro(fileBin, aluno);
    fileBin.close();

    insereIndiceSecundario(aluno);
    std::cout << "Aluno inserido com sucesso na posição " << offset << std::endl;
}

void SistemaGerenciador::insereIndiceSecundario(const Aluno &aluno)
{
    //Abertura do arquivo
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

    //Se não houver curso, cria um posição no vetor de indices secundario e aponta para o final da lista
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
        //Se houver, atualiza a cabeça da lista
        rrn_cabeca_anterior = indicesSecundarios[indice].rrn_lista_invertida;
        indicesSecundarios[indice].rrn_lista_invertida = novo_rrn_lista_invertida;
    }

    //Cria um novo nó da lista
    NoListaInvertida novoNo;
    novoNo.matricula_aluno = aluno.matricula;
    novoNo.proximo_rrn = rrn_cabeca_anterior;
    escreverRegistro(fileListaInvertida, novoNo);

    fileListaInvertida.close();
}

long SistemaGerenciador::obterEspaçoDisponivel()
{
    //Encontra a primeira posição da lista de disponíveis
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
    //Adiciona um novo espaço(posição) a lista de disponíveis
    Disponiveis disponivel;
    disponivel.offset = offset;
    disponivel.valido = true;
    disponiveis.push_back(disponivel);
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
