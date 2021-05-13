#include <iostream>
#include <list>
#include <pthread.h>
#include "simone.h"

using namespace std;



int main(){
    // int paramentro_oimundo = 10;

    // Tarefa tarefa;
    // tarefa.funcao = OlaMundo;
    // tarefa.parametros = (void*)&paramentro_oimundo;

    int pvs = 5;
    start(pvs);
    finish(pvs);
    return 0;
}