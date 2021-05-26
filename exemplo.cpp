#include <iostream>
#include <list>
#include <pthread.h>
#include "simone.h"

using namespace std;

int main(){
    int pvs = 10;
    Tarefa teste1, teste2, teste3, teste4;
    teste1.funcao = funcTeste;
    teste1.id = 1;
    teste1.parametros = (void*)("Lucas");
    teste2.funcao = funcTeste;
    teste2.id = 2;
    teste2.parametros = (void*)("Joao");
    teste3.funcao = funcTeste;
    teste3.id = 3;
    teste3.parametros = (void*)("Matheus");
    teste4.funcao = funcTeste;
    teste4.id = 4;
    teste4.parametros = (void*)("Guilherme");
    tarefasProntas.push_back(teste1);
    tarefasProntas.push_back(teste2);
    tarefasProntas.push_back(teste3);
    tarefasProntas.push_back(teste3);
    tarefasProntas.push_back(teste4);
    tarefasProntas.push_back(teste4);
    tarefasProntas.push_back(teste4);
    tarefasProntas.push_back(teste4);
    tarefasProntas.push_back(teste4);
    tarefasProntas.push_back(teste4);
    start(pvs);
    finish();
    return 0;
}