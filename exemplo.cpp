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
    int pvs = 3;
    int tID, tID2, tID3;
    void *r, *s, *t;

    start(pvs);
    spawn(NULL, funcTeste, (void*)("Lucas"));
    spawn(NULL, funcTeste, (void*)("Gabriel"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    tID = spawn(NULL, funcTeste, (void*)("Herculanoxxxxxxxxxx"));
    sync(tID, &r);
    cout << "RESULTADO SYNC 1: " << *(int*)r << endl;
    cout << "\n";
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    spawn(NULL, funcTeste, (void*)("Herculano"));
    

    //fazCoisa();


    tID2 = spawn(NULL, funcTeste, (void*)("AnaAmeliax"));
    sync(tID2, &s);
    cout << "RESULTADO SYNC 2: " << *(int*)s << endl;
    cout << "\n";

    tID3 = spawn(NULL, funcTeste, (void*)("Gustavo"));
    sync(tID3, &t);
    cout << "RESULTADO SYNC 3: " << *(int*)t << endl;
    cout << "\n";

    cout << "ID 1 para sincronizar: " << tID << endl;
    cout << "ID 2 para sincronizar: " << tID2 << endl;
    cout << "ID 3 para sincronizar: " << tID3 << endl;
    cout << "\n";

    finish();
    return 0;
}