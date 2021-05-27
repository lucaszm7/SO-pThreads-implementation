#include <iostream>
#include <list>
#include <pthread.h>
#include "simone.h"

using namespace std;

void fazCoisa(){
    for (int x = 0; x < 1000000; ++x){
        int y = x * x;
        
        x += 1;
    }
}

int main(){
    int pvs = 10;
    int tID, tID2;
    void *r, *s;

    start(pvs);
    tID = spawn(NULL, funcTeste, (void*)("AnaAmeliax"));
    spawn(NULL, funcTeste, (void*)("Lucas"));
    spawn(NULL, funcTeste, (void*)("Gustavo"));
    spawn(NULL, funcTeste, (void*)("Gabriel"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    tID2 = spawn(NULL, funcTeste, (void*)("Herculanoxxxxxxxxxx"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    
    cout << "ID 1 para sincronizar: " << tID << endl;
    cout << "ID 2 para sincronizar: " << tID2 << endl;

    fazCoisa();

    sync(tID, &r);
    finish();
    //sync(tID2, &s);
    cout << "RESULTADO SYNC 1: " << *(int*)r << endl;
    //cout << "RESULTADO SYNC 1: " << *(int*)s << endl;
    return 0;
}