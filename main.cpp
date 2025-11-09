#include "SistemaGerenciador.hpp"
#include <iostream>

int main()
{
    SistemaGerenciador sistema("data/alunos_mil.csv", "data/alunos.dat", "data/indices.dat",
                               "data/indicesSecundarios", "data/ListaInvertida");
    
   sistema.iniciar();
   return 0;
}