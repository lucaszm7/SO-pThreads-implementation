#include <iostream>
#include <list>
#include <pthread.h>

using namespace std;

void* OlaMundo(void* pPalavra){
    int dta = *((int*) pPalavra);

    for (int i = 0; i < dta; ++i){
        cout << "Ola mundo" << endl;
    }
    return 0;
}

class Tarefa{
public:
    int id;
    void* (*funcao) (void*);
    void* parametros;
};

list <Tarefa> tarefasProntas;
list <pthread_t> threads;

void* loop_que_itera(void*p) {
    cout << "ITERANDO" << endl;
    return 0;
}

int start(int m){
    void* (*func)(void*);
    func = loop_que_itera;
    for (int i = 0; i < m; ++i){
        pthread_t thread;
        threads.push_back(thread);
        pthread_create(&thread, NULL, func, NULL);
    }
    return 1;
}

int finish(int m){
    for (int i = 0; i < m; ++i){
        pthread_join(threads.front(), NULL);
        threads.pop_front();
    }
    return 1;
}