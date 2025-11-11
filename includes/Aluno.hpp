#ifndef ALUNO_HPP
#define ALUNO_HPP

class Aluno
{
public:
  int matricula;
  char nome[50];
  char curso[30];

public:
  void parser(std::string linha);
  void display();
  Aluno() = default;
  Aluno(char nomeNovo[50], char cursoNovo[30], int matriculaNova)
  {
    strcpy(nome, nomeNovo);
    strcpy(curso, cursoNovo);
    matricula = matriculaNova;
  }
};

#endif