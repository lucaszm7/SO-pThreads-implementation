#include <iostream>
#include <list>
#include <string>
#include <pthread.h>

using namespace std;

class Tarefa{
public:
    int id;
    void* (*funcao) (void*);
    list <void*> parametros;
};

list <Tarefa> tarefasProntas;
list <pthread_t> threads;

int start(int m){
    for (int i = 0; i < m; ++i){
        pthread_t thread;
        pthread_create(*thread, NULL, loop_que_itera, NULL);
    }
}

void* loop_que_itera(void*p) {
    
}