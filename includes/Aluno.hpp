#ifndef ALUNO_HPP
#define ALUNO_HPP

class Aluno
{
public:
    int matricula;
    char nome[50];
    char curso[50];
    void parser(std::string linha);
    void display();
};

#endif