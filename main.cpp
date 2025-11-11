#include "SistemaGerenciador.hpp"
#include <iostream>

int main()
{
        // Escolhe o arquivo
    int opcao;
    bool valido = false;
    std::string fileCSV;
    std::string fileDisponivel;
    do
    {
        std::cout << "Escolha o Arquivo para ser lido" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        std::cout << "1. 'alunos_mil'" << std::endl;
        std::cout << "2 - 'alunos_100mil" << std::endl;
        std::cout << "3 - 'alunos_1milhao'" << std::endl;
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        switch (opcao)
        {
        case 1:
            fileCSV = "data/alunos_mil.csv";
            fileDisponivel = "data/disponivelmil";
            std::cout << std::endl;
            valido = true;
            break;
        case 2:
            fileCSV = "data/alunos_100mil.csv";
            fileDisponivel = "data/disponivel100mil";
            std::cout << std::endl;
            valido = true;
            break;
        case 3:
            fileCSV = "data/alunos_1milhao.csv";
            fileDisponivel = "data/disponivel1milhao";
            std::cout << std::endl;
            valido = true;
            break;

        default:
            std::cout << "Numero invalido" << std::endl;
            break;
        }
    } while (!valido);
    
    SistemaGerenciador sistema("data/alunos_mil.csv", "data/alunos.dat", "data/indices.dat",
                               "data/indicesSecundarios.dat", "data/ListaInvertida.dat", 
                               "data/disponiveis.dat");
    
   sistema.iniciar();
   return 0;
}