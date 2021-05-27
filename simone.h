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
        dta = strdup("JOSEARLINDODACRUZEVARISTO");
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
pthread_mutex_t m_cout = PTHREAD_MUTEX_INITIALIZER;
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
        //Garante a exclusividade ao verificar a lista de tarefas prontas
        pthread_mutex_lock(&(m_tarefas_prontas));
        //Verifica se a lista está vazia
        if(!tarefasProntas.empty()){
            //Caso a lista não esteja vazia pega uma tarefa e libera o mutex
            *tarefaAtual = (tarefasProntas.front());
            tarefasProntas.pop_front();
            pthread_mutex_unlock(&(m_tarefas_prontas));
            //Executa a função
            tarefaAtual->retorno = tarefaAtual->funcao(tarefaAtual->parametros);
            //Coloca o resultado na lista de tarefas terminadas
            pthread_mutex_lock(&m_tarefas_terminadas);
            tarefasTerminadas.push_back(*tarefaAtual);
            pthread_mutex_unlock(&m_tarefas_terminadas);

        }
        //Se a lista estiver vazia libera a mutex
        else {
            pthread_mutex_unlock(&(m_tarefas_prontas));
        }
    
    } while(!isFinished);

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
    pthread_mutex_lock(&m_countID);
    countID += 1;
    int returnID = countID;
    Tarefa novaTarefa;
    novaTarefa.funcao = func;
    novaTarefa.parametros = dta;
    novaTarefa.id = countID;
    pthread_mutex_unlock(&m_countID);
    pthread_mutex_lock(&(m_tarefas_prontas));
    tarefasProntas.push_back(novaTarefa);
    pthread_mutex_unlock(&(m_tarefas_prontas));

    return returnID;
}

int sync(int idTarefa, void** retornoTarefa){
    Tarefa *tarefaAtual = new Tarefa;
    list<Tarefa>::iterator it1, it2;
    bool achou = false;

    //Ocasião numero 1
    //A tarefa a ser sincronizada ainda não foi executada
    //Executaremos a tarefa e já retornaremos o resultado
    // e retiraremos a tarefa da lista de tarefas prontas

    pthread_mutex_lock(&m_tarefas_prontas);
    for (it1 = tarefasProntas.begin(); it1 != tarefasProntas.end(); ++it1){
        if(it1->id == idTarefa){
            achou = true;
            cout << "TAREFA NAO FOI EXECUTADA AINDA" << endl;

            cout << "TAMANHO TAREFAS PRONTAS: " << tarefasProntas.size() << endl;
            tarefasProntas.erase(it1);
            cout << "TAMANHO TAREFAS PRONTAS: " << tarefasProntas.size() << endl;

            if(!pthread_mutex_unlock(&m_tarefas_prontas)){
                cout << "LIBEROU MUTEX TAREFAS PRONTAS 1" << endl;
            }

            *retornoTarefa = it1->funcao(it1->parametros);
            cout << "RESULTADO PRE SYNC: " << *(int*)(*retornoTarefa) << endl;


            break;
        }
    }
    if(!achou){
        if(!pthread_mutex_unlock(&m_tarefas_prontas)){
            cout << "LIBEROU MUTEX TAREFAS PRONTAS 2" << endl;
        }
    }
    //Ocasião numero 2
    //A tarefa não está na lista de tarefas prontas
    //Então iremos procurar na lista de tarefas terminadas
    //Caso ela esteja aqui, retornaremos o resultado e tiraremos
    // a tarefa da lista de tarefas terminadas
    if(!achou){
        pthread_mutex_lock(&m_tarefas_terminadas);
        for (it2 = tarefasTerminadas.begin(); it2 != tarefasTerminadas.end(); ++it2){
            if(it2->id == idTarefa){
                achou = true;
                cout << "TAREFA JA FOI EXECUTADA" << endl;

                cout << "TAMANHO TAREFAS TERMINADAS: " << tarefasTerminadas.size() << endl;
                tarefasTerminadas.erase(it2);
                cout << "TAMANHO TAREFAS TERMINADAS: " << tarefasTerminadas.size() << endl;

                if(!pthread_mutex_unlock(&m_tarefas_terminadas)){
                    cout << "LIBEROU MUTEX TAREFAS TERMINADAS 1" << endl;
                }
                
                *retornoTarefa = it2->retorno;
                cout << "RESULTADO PRE SYNC: " << *(int*)(*retornoTarefa) << endl;
                break;
            }
        }
        if(!achou){
            if(!pthread_mutex_unlock(&m_tarefas_terminadas)){
                    cout << "LIBEROU MUTEX TAREFAS TERMINADAS 2" << endl;
                }
        }
        //Ocasiao numero 3
        //A tarefa está em execução
        //Esperar que ela termine

        // if(!achou){
        //     cout << "TAREFA SENDO EXECUTADA" << endl;
        //     sleep(1);
        //     pthread_mutex_lock(&m_tarefas_terminadas);
        //     for (it1 = tarefasTerminadas.begin(); it1 != tarefasTerminadas.end(); ++it1){
        //         if(it1->id == idTarefa){
        //             cout << "TAREFA TERMINOU" << endl;
        //             tarefasTerminadas.erase(it1);
        //             pthread_mutex_unlock(&m_tarefas_terminadas);
        //             *retornoTarefa = it1->retorno;
        //             achou = true;
        //             break;
        //         }
        //     }
        // }
    }

    //Printa o resultado
    //=== PARA DEBUG ===
    pthread_mutex_lock(&m_tarefas_terminadas);
    for (it2 = tarefasTerminadas.begin(); it2 != tarefasTerminadas.end(); ++it2){
        cout << "Retorno: " << *((int*)(it2->retorno)) << endl;
    }
    pthread_mutex_unlock(&m_tarefas_terminadas);
    return 1;
}

void finish(){
    cout << "=== SEGMENTATION FAULT 1 ===" << endl;
    isFinished = true;
    while (!threads.empty()){
        cout << "=== SEGMENTATION FAULT 2 === " << threads.size() << endl;
        pthread_join(threads.front(), NULL);
        cout << "=== SEGMENTATION FAULT 3 ===" << endl;
        threads.pop_front();
    }
}