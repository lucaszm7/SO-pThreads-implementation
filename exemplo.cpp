#include <iostream>
#include <list>
#include <pthread.h>
#include <chrono>
#include <fstream>
#include "simone.h"
using namespace std::chrono;

using namespace std;

// void* funcTeste(void* pTeste){
//     //cout << "ERRO 1" << endl;
//     char* dta = (char*) pTeste;
//     //cout << "STRING REFERENCE: " << *dta <<  endl;
//     int *r = new int;
//     *r = 0;
//     //cout << "ERRO 2" << endl;
//     if(pTeste == NULL){
//         dta = strdup("JOSEARLINDODACRUZEVARISTO");
//         //cout << "ERRO 3" << endl;
//     }
//     for (*r = 0; dta[*r] != NULL; ++(*r));
//     return (void*) r;
// }

void* funcTeste2(void* t){
    cout << "Ola mundo" << endl;
    int *r = new int;
    *r = 0;
    return (void*) r;
}

void* fazCoisa(void* n){
    int *r = new int;
    int *y = new int;
    *r = 1;
    for (int x = 0; x < 20000000; ++x){
        *y = *y * x;
        *y = *y - (2 * x);
    }
    *r = *y;
    //cout << "Tarefa " << *(int*)n << " executada" << endl;
    return r;
}

void* fibo(void *dta){
    int n = *(int*)dta;
    int *n1, *n2, 
        *r = new int,
        t1, t2;
    void *r1, *r2;
    if(n <= 2) *r = 1;
    else {
        n1 = new int;
        *n1 = n - 1;
        t1 = spawn(NULL, fibo, (void*) n1);
        n2 = new int;
        *n2 = n - 2;
        t2 = spawn(NULL, fibo, (void*) n2);

        sync(t1, &r1);
        sync(t2, &r2);
        *r = *(int*)r1 + *(int*)r2;
        delete(n1);
        delete(n2);
        delete((int*)r1);
        delete((int*)r2);
    }

    return r;
}

int main1(int pvs, int par){

    auto startTime = high_resolution_clock::now();

    //int pvs = 0;
    int tID;
    //int par = 40;
    void *r;
    start(pvs);
    tID = spawn(NULL, fibo, &par);
    sync(tID, &r);
    cout << "Fibo de " << par << " = " << *(int*)r << endl;
    cout << "PV's       = "<< pvs << endl;
    finish();
    //cout << "Finish executado" << endl;

    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);
    cout << "Time       = " << (float)duration.count()/1000000 << endl;
    cout << endl;
    return 0;

}
///*
// int main2(){
//     int pvs = 15;
//     int tID, tID2, tID3;
//     void *r, *s, *t;
//     start(pvs);
//     spawn(NULL, funcTeste, (void*)("Gabriel"));
//     spawn(NULL, funcTeste, (void*)("Herculano"));
//     tID = spawn(NULL, funcTeste, (void*)("Herculanoxxxxxxxxxx"));
//     sync(tID, &r);
//     cout << "RESULTADO SYNC 1: " << *(int*)r << endl;
//     cout << "\n";
//     spawn(NULL, funcTeste, (void*)("Herculano"));
//     spawn(NULL, funcTeste, (void*)("Herculano"));
//     tID2 = spawn(NULL, funcTeste, (void*)("AnaAmeliax"));
//     spawn(NULL, funcTeste, (void*)("Herculano"));
//     spawn(NULL, funcTeste, (void*)("Herculano"));
//     spawn(NULL, funcTeste, (void*)("Herculano")); 
//     //fazCoisa();
//     sync(tID2, &s);
//     cout << "RESULTADO SYNC 2: " << *(int*)s << endl;
//     cout << "\n";
//     tID3 = spawn(NULL, funcTeste, (void*)("Gustavo"));
//     sync(tID3, &t);
//     cout << "RESULTADO SYNC 3: " << *(int*)t << endl;
//     cout << "\n";
//     cout << "ID 2 para sincronizar: " << tID2 << endl;
//     cout << "ID 3 para sincronizar: " << tID3 << endl;
//     cout << "\n";
//     finish();
//     cout << "ID 1 para sincronizar: " << tID << endl;
//     return 0;
// }

float main3(int pvs, int m){
    auto startTime = high_resolution_clock::now();
    int tiD[m];
    int par[m];
    void* r[m];
    start(pvs);

    for (int i = 0; i < m; ++i){
        par[i] = i;
        tiD[i] = spawn(NULL, fazCoisa, &par[i]);
    }

    for (int i = 0; i < m; ++i){
        sync(tiD[i], &r[i]);
    }
    // for(int i = 0; i < m; ++i){
    //     cout << "Retorno da tarefa " << i << ": " << (int)r[i] << endl;
    // }
    for (int i = 0; i < m; ++i){
        delete((int*)r[i]);
    }
    finish();
    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);
    // cout << "Tempo de execucao: " << (float)duration.count()/1000000 << endl;

    return (float)(duration.count());
}

int main(){
    int pvs = 1;
    int m = 1;
    int nfibo = 1;
    float time[8][20];
    ofstream results;

    cout << "=== Generic Task ===" << endl;
    for (pvs = 1; pvs <= 8; pvs += 2){
        for (m = 1; m <= 20; m += 4){
            time[pvs][m] = main3(pvs, m)/1000000;
            cout << "Time for " << pvs << " pvs and " << m << " tasks equal: " <<time[pvs][m]<<endl;
        }
    }
    cout << endl;
    cout << "=== Fibonacci ===" << endl;
    for (pvs = 0; pvs < 10; pvs += 2){
        main1(pvs, 30);
    }
    cout << endl;

    return 0;
}