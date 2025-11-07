#ifndef ALUNO_HPP
#define ALUNO_HPP

class Aluno
{
private:
    int matricula;
    char nome[50];
    char curso[50];
public:
    Aluno();
    ~Aluno();
    void parser(std::string linha);
    void display();
};

#endif