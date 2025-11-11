#include "SistemaGerenciador.hpp"
#include <iostream>
#include <string>
// A dependência de <limits> foi removida.

int main()
{
    // Escolhe o arquivo
    int opcao;
    bool valido = false;
    std::string fileCSV;
    
    // Variáveis que vão montar os caminhos dos arquivos gerados
    std::string baseName;
    std::string arquivoDados;
    std::string arquivoIndicePrimario;
    std::string arquivoIndiceSecundario;
    std::string arquivoListaInvertida;
    std::string fileDisponivel; // O nome do arquivo de disponíveis

    // Define a nova pasta para todos os arquivos gerados/binários:
    const std::string pasta = "indices/"; 

    do
    {
        std::cout << "Escolha o Arquivo CSV para ser lido (deve estar em 'data/')" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "1. 'alunos_mil'" << std::endl;
        std::cout << "2. 'alunos_100mil'" << std::endl;
        std::cout << "3. 'alunos_1milhao'" << std::endl;
        std::cout << "Escolha uma opcao: ";
        
        if (!(std::cin >> opcao)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n'); 
            opcao = -1;
            std::cout << "Opção inválida, tente novamente." << std::endl;
            continue;
        }

        // Limpa o buffer após ler o inteiro
        std::cin.ignore(1000, '\n'); 
        
        switch (opcao)
        {
        case 1:
            fileCSV = "data/alunos_mil.csv";
            baseName = "alunos_mil";
            valido = true;
            break;
        case 2:
            fileCSV = "data/alunos_100mil.csv";
            baseName = "alunos_100mil";
            valido = true;
            break;
        case 3:
            fileCSV = "data/alunos_1milhao.csv";
            baseName = "alunos_1milhao";
            valido = true;
            break;

        default:
            std::cout << "Número inválido" << std::endl;
            break;
        }
    } while (!valido);

    // Define nomes de arquivos gerados dentro da nova pasta 'indices/'
    arquivoDados = pasta + baseName + ".dat";
    arquivoIndicePrimario = pasta + baseName + "_ip.dat";
    arquivoIndiceSecundario = pasta + baseName + "_is.dat";
    arquivoListaInvertida = pasta + baseName + "_li.dat";
    fileDisponivel = pasta + "disp_" + baseName + ".dat";
    
    // Instancia o sistema com os nomes de arquivos corretos
    SistemaGerenciador sistema(fileCSV, arquivoDados, arquivoIndicePrimario,
                              arquivoIndiceSecundario, arquivoListaInvertida, 
                              fileDisponivel);
    
    sistema.iniciar();
    return 0;
}