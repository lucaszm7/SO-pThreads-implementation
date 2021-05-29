#include <iostream>
#include <list>
#include <pthread.h>
#include "simone.h"

using namespace std;

void* fazCoisa(void* n){
    int y;
    for (int x = 0; x < 200000000; ++x){
        y = y * x;
        x += 1;
    }
    cout << "TAREFA EXECUTADA N: " << *(int*)n << endl;
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
        delete(r1);
        delete(r2);
    }

    return r;
}

int main1(){
    int pvs = 2;
    int tID;
    int par = 30;
    void *r;
    start(pvs);
    tID = spawn(NULL, fibo, &par);
    //fazCoisa();
    sync(tID, &r);
    cout << "Fibo de " << par << " = " << *(int*)r << endl;
    finish();
}

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
    //cout << "RESULTADO SYNC 2: " << *(int*)s << endl;
    // cout << "\n";

    // tID3 = spawn(NULL, funcTeste, (void*)("Gustavo"));
    // sync(tID3, &t);
    // cout << "RESULTADO SYNC 3: " << *(int*)t << endl;
    // cout << "\n";

    cout << "ID 2 para sincronizar: " << tID2 << endl;
    //cout << "ID 3 para sincronizar: " << tID3 << endl;
    //cout << "\n";

    finish();
    //cout << "ID 1 para sincronizar: " << tID << endl;
    return 0;
}

int main3(){
    int pvs = 10;
    int m = 100;
    int tiD[m];
    int par[m];
    start(pvs);

    for (int i = 0; i < m; ++i){
        par[i] = i;
        tiD[i] = spawn(NULL, fazCoisa, &par[i]);
        printf("TID %d: %d\n", i, tiD[i]);
    }
    // for (int i = 0; i < m; ++i){
    //     printf("TID %d: %d\n", i, tiD[i]);
    // }
    Sleep(5000);
    finish();
}

int main(){
    main1();
}