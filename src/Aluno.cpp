#include "../includes/Aluno.hpp"
#include <iostream>
#include <sstream>
#include <vector>

void Aluno::parser(std::string linha)
{
    std::stringstream ss(linha);
    std::string campo;

    // Se o campo da matricula não for vazia, salva
    std::getline(ss, campo, ',');
    if (!campo.empty())
    {
        matricula = stoi(campo);
    }

    //Mome
    std::getline(ss, campo, ',');
    //Copia o conteudo de "campo" para "nome",respeitando o tamanho do campo "nome"
    std::snprintf(nome, sizeof(nome), "%s", campo.c_str());

    //Curso
    std::getline(ss, campo, ',');
    std::snprintf(curso, sizeof(curso), "%s", campo.c_str());
}
//Imprime todas as informações do aluno
void Aluno::display()
{
    std::cout << matricula << " - " << nome << " - " << curso << std::endl;
}