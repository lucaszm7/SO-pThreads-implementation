#include <iostream>
#include <list>
#include <string>
#include <pthread.h>

using namespace std;

void* funcTeste(void* pTeste){
    //cout << "ERRO 1" << endl;
    char* dta = (char*) pTeste;
    //cout << "STRING REFERENCE: " << *dta <<  endl;
    int *r = new int;
    *r = 0;
    //cout << "ERRO 2" << endl;
    if(pTeste == NULL){
        dta = strdup("Lucas");
        //cout << "ERRO 3" << endl;
    }

    for (*r = 0; dta[*r] != NULL; ++(*r));

    return (void*) r;
}

void* funcTeste2(void* t){
    cout << "Ola mundo" << endl;
    return 0;
}

class Tarefa{
public:
    int id = 0;
    void* (*funcao) (void* t) = NULL;
    void* parametros = NULL;
    void* retorno = NULL;
};

class Attrib {
public:
    int p; //Prioridade da tarefa
    int c; //Custo computacional
};

pthread_mutex_t m_tarefas_prontas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_cout = PTHREAD_MUTEX_INITIALIZER;
list <Tarefa> tarefasProntas;
list <Tarefa> tarefasTerminadas;
list <pthread_t> threads;

bool isFinished = false;

    //Mutex para verificar a lista
    //Se tiver uma tarefa pega ela e retira da lista
    //Libera a lista
    //Chama tarefaAtual->funcao(tarefaAtual->parametros)
    //Agora como pegar o retorno?
    //tarefaAtual->retorno = tarefaAtual->funcao(tarefaAtual->parametros)
    //Coloca tarefa concluida na lista de tarefasTerminadas
    //Agora como eu saio desse loop?

void* loop_que_itera(void*p) {
    Tarefa *tarefaAtual = new Tarefa;

    do {
        pthread_mutex_lock(&(m_tarefas_prontas));
        if(!tarefasProntas.empty()){
            //cout << "SIZE OF LIST: " << tarefasProntas.size() << endl;
            *tarefaAtual = (tarefasProntas.front());
            tarefasProntas.pop_front();
        }
        //cout << "FINAL SIZE OF LIST: " << tarefasProntas.size() << endl;
        //cout << "ID OF TAREFA: " << tarefaAtual->id << endl;
        pthread_mutex_unlock(&(m_tarefas_prontas));

        //cout << "ERRO 1" << endl;
        if (tarefaAtual->funcao != NULL){
            tarefaAtual->retorno = tarefaAtual->funcao(tarefaAtual->parametros);
            tarefasTerminadas.push_back(*tarefaAtual);
            int* nome = (int*)(tarefaAtual->retorno);
            pthread_mutex_lock(&m_cout);
            cout << "RETORNO: " << *((int*)(tarefaAtual->retorno)) << endl;
            pthread_mutex_unlock(&m_cout);
        } 

        tarefaAtual->id = 0;
        tarefaAtual->funcao = NULL;
        tarefaAtual->parametros = NULL;
        tarefaAtual->retorno = NULL;
    
    } while(!isFinished);

    //cout << "ERRO 2" << endl;
    //funcTeste(NULL);
    //funcTeste2();
    return 0;
}

int start(int m){
    for (int i = 0; i < m; ++i){
        pthread_t thread;
        threads.push_back(thread);
        pthread_create(&thread, NULL, loop_que_itera, NULL);
    }
    return 1;
}

int spawn(Attrib *attr = NULL, void* (*func) (void*) = NULL, void* dta = NULL){
    return 1;
}

int sync(int idTarefa, void** retornoTarefa){
    return 1;
}

void finish(){
    isFinished = true;
    for (int i = 0; i < threads.size(); ++i){
        pthread_join(threads.front(), NULL);
        threads.pop_front();
    }
}