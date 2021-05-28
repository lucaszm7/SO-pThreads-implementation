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
        dta = ("JOSEARLINDODACRUZEVARISTO");
        //cout << "ERRO 3" << endl;
    }

    for (*r = 0; dta[*r] != NULL; ++(*r));

    return (void*) r;
}

void* funcTeste2(void* t){
    cout << "Ola mundo" << endl;
    int *r = new int;
    *r = 0;
    return (void*) r;
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
pthread_mutex_t m_tarefas_terminadas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_threads = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_countID = PTHREAD_MUTEX_INITIALIZER;
list <Tarefa> tarefasProntas;
list <Tarefa> tarefasTerminadas;
list <pthread_t> threads;

int countID = 0;
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
        
        pthread_mutex_lock(&(m_tarefas_prontas));   // lock: m_tarefas_prontas; Garante a exclusividade ao verificar a lista de tarefas prontas

        //Verifica se a lista está vazia
        if(!tarefasProntas.empty()){
            //Caso a lista não esteja vazia pega uma tarefa e libera o mutex
            *tarefaAtual = (tarefasProntas.front());
            tarefasProntas.pop_front();

            pthread_mutex_unlock(&(m_tarefas_prontas)); // unlock caso if=True: m_tarefas_prontas;

            //Executa a função
            tarefaAtual->retorno = tarefaAtual->funcao(tarefaAtual->parametros);
            //Coloca o resultado na lista de tarefas terminadas

            pthread_mutex_lock(&m_tarefas_terminadas);  // lock: m_tarefas_terminadas;
            tarefasTerminadas.push_back(*tarefaAtual);
            pthread_mutex_unlock(&m_tarefas_terminadas);    // unlock: m_tarefas_terminadas;

        }
        //Se a lista estiver vazia libera a mutex
        else {
            pthread_mutex_unlock(&(m_tarefas_prontas)); // unlock caso if=False: m_tarefas_prontas;
        }
    
    } while(!isFinished);

    return 0;
}

int start(int m){
    pthread_mutex_lock(&(m_threads));   // lock: m_threads
    for (int i = 0; i < m; ++i){
        pthread_t thread;
        threads.push_back(thread);
        pthread_create(&thread, NULL, loop_que_itera, NULL);
    }
    pthread_mutex_unlock(&(m_threads)); // unlock: m_threads
    return 1;
}

int spawn(Attrib *attr = NULL, void* (*func) (void*) = NULL, void* dta = NULL){

    pthread_mutex_lock(&m_countID); // lock: m_countID
    countID += 1;
    int returnID = countID;
    Tarefa novaTarefa;
    novaTarefa.funcao = func;
    novaTarefa.parametros = dta;
    novaTarefa.id = countID;
    pthread_mutex_unlock(&m_countID);   // unlock: m_countID

    pthread_mutex_lock(&(m_tarefas_prontas));   // lock: m_tarefas_prontas
    tarefasProntas.push_back(novaTarefa);
    pthread_mutex_unlock(&(m_tarefas_prontas)); // unlock: m_tarefas_prontas

    // cout << "RETURN ID: " << returnID << endl;
    // cout << "\n";
    return returnID;
}

int sync(int idTarefa, void** retornoTarefa){
    Tarefa *tarefaSync = new Tarefa;
    list<Tarefa>::iterator iterator_t;

    while(true){

        pthread_mutex_lock(&(m_tarefas_prontas));   // lock: m_tarefas_prontas
        pthread_mutex_lock(&(m_tarefas_terminadas));    // lock: m_tarefas_terminadas

        // ------------------------------------------------------------------------------------------------------
        // Checa caso 1: Tarefa está na lista de tarefas_prontas
        // ------------------------------------------------------------------------------------------------------

        for(iterator_t = tarefasProntas.begin(); iterator_t != tarefasProntas.end(); iterator_t){

            if(idTarefa == iterator_t->id){    // caso 1: Tarefa está na tarefasProntas

                *tarefaSync = *iterator_t;
                tarefasProntas.erase(iterator_t);

                pthread_mutex_unlock(&(m_tarefas_prontas));   // unlock caso if=True: m_tarefas_prontas
                pthread_mutex_unlock(&(m_tarefas_terminadas)); // unlock caso if=True: m_tarefas_terminadas

                *retornoTarefa = tarefaSync->funcao(tarefaSync->parametros);

                return 1;
            }
        }
        // ------------------------------------------------------------------------------------------------------
        // Checa caso 2: Tarefa está na lista de tarefas_terminadas
        // ------------------------------------------------------------------------------------------------------

        for(iterator_t = tarefasTerminadas.begin(); iterator_t != tarefasTerminadas.end(); iterator_t){

            if(idTarefa == iterator_t->id){    // caso 2: Tarefa está na tarefasTerminadas

                *tarefaSync = *iterator_t;
                tarefasTerminadas.erase(iterator_t);

                pthread_mutex_unlock(&(m_tarefas_prontas));   // unlock caso if=True: m_tarefas_prontas
                pthread_mutex_unlock(&(m_tarefas_terminadas)); // unlock caso if=True: m_tarefas_terminadas

                *retornoTarefa = tarefaSync->retorno;
                return 1;
            }
        }

        //pthread_mutex_unlock(&(m_tarefas_prontas));   // unlock caso if=False: m_tarefas_prontas
        pthread_mutex_unlock(&(m_tarefas_terminadas)); // unlock caso if=False: m_tarefas_terminadas

        // ------------------------------------------------------------------------------------------------------
        // caso 3: Tarefa não está na lista de tarefas_prontas e também não está na lista de tarefas_terminadas
        // ------------------------------------------------------------------------------------------------------

        if(!tarefasProntas.empty()){    // checa se há uma tarefa para finalizar enquanto espera pela tarefaSync terminar
                
            // tarefas_prontas já está com lock
            *tarefaSync = (tarefasProntas.front());
            tarefasProntas.pop_front();

            pthread_mutex_unlock(&(m_tarefas_prontas)); // unlock caso if=True: m_tarefas_prontas;

            //Executa a função
            tarefaSync->retorno = tarefaSync->funcao(tarefaSync->parametros);   // Coloca o resultado na lista de tarefas terminadas

            pthread_mutex_lock(&m_tarefas_terminadas);  // lock: m_tarefas_terminadas;
            tarefasTerminadas.push_back(*tarefaSync);
            pthread_mutex_unlock(&m_tarefas_terminadas);    // unlock: m_tarefas_terminadas;
        }
        //Se a lista estiver vazia libera a mutex
        else {
            pthread_mutex_unlock(&(m_tarefas_prontas)); // unlock caso if=False: m_tarefas_prontas;
        }
    }
}

void finish(){
    cout << "Finalizando threads... /n" << endl;
    isFinished = true;
    while (!threads.empty()){
        //cout << "=== SEGMENTATION FAULT 2 === " << threads.size() << endl;
        pthread_join(threads.front(), NULL);
        //cout << "=== SEGMENTATION FAULT 3 ===" << endl;
        threads.pop_front();
    }
}