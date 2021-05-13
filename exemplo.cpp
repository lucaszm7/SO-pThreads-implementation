#include <iostream>
#include <list>
#include <string>
#include <pthread.h>
#include "simone.h"

using namespace std;

void* OlaMundo(void* pPalavra){
    int dta = *((int*) pPalavra);

    for (int i = 0; i < dta; ++i){
        cout << "Ola mundo" << endl;
    }
}

int main(){
    int paramentro_oimundo = 10;

    Tarefa tarefa;
    tarefa.funcao = OlaMundo;
    tarefa.parametros = &paramentro_oimundo

    int pvs = 5;
    start(pvs);

    tarefasProntas.push_back(tarefa);
    tarefasProntas.push_back(tarefa);
    tarefasProntas.push_back(tarefa);
    tarefasProntas.push_back(tarefa);
    tarefasProntas.push_back(tarefa);
    tarefasProntas.push_back(tarefa);
    tarefasProntas.push_back(tarefa);
    
    return 0;
}