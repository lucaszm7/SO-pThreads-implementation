#include <iostream>
#include <list>
#include <pthread.h>
#include "simone.h"
#include <chrono>
using namespace std::chrono;

using namespace std;

void fazCoisa(){
    for (int x = 0; x < 100000000; ++x){
        int y = x * x;
        x += 1;
    }
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

        //cout << "Fibo sync irá começar ";

        sync(t1, &r1);
        sync(t2, &r2);
        *r = *(int*)r1 + *(int*)r2;
    }
    return r;
}

int main1(){

    auto startTime = high_resolution_clock::now();

    int pvs = 12;
    int tID;
    int par = 35;
    void *r;
    start(pvs);
    //cout << "Pvs inicializados ";
    tID = spawn(NULL, fibo, &par);
    //cout << "Spawn fibo executado";
    //cout << "Sync irá ser executado";
    sync(tID, &r);
    //cout << "Sync executado";
    //cout << "Finish irá ser executado";
    cout << "Fibo de " << par << " = " << *(int*)r << endl;
    finish();
    cout << "Finish executado" << endl;

    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);
    cout << "Tempo de execução: " << duration.count() << endl;

   
}
///*
int main2(){
    int pvs = 15;
    int tID, tID2, tID3;
    void *r, *s, *t;

    start(pvs);
    spawn(NULL, funcTeste, (void*)("Gabriel"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    tID = spawn(NULL, funcTeste, (void*)("Herculanoxxxxxxxxxx"));
    sync(tID, &r);
    cout << "RESULTADO SYNC 1: " << *(int*)r << endl;
    cout << "\n";
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    tID2 = spawn(NULL, funcTeste, (void*)("AnaAmeliax"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    

    //fazCoisa();


    sync(tID2, &s);
    cout << "RESULTADO SYNC 2: " << *(int*)s << endl;
    cout << "\n";

    tID3 = spawn(NULL, funcTeste, (void*)("Gustavo"));
    sync(tID3, &t);
    cout << "RESULTADO SYNC 3: " << *(int*)t << endl;
    cout << "\n";

    cout << "ID 2 para sincronizar: " << tID2 << endl;
    cout << "ID 3 para sincronizar: " << tID3 << endl;
    cout << "\n";

    finish();
    cout << "ID 1 para sincronizar: " << tID << endl;
    return 0;
}
//*/

int main(){
    main1();
    return 0;
}
