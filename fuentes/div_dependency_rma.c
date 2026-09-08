

///////////////////////////////////////////////////
/// CORE BOUND RANDOM MEMORY ACCESS
///////////////////////////////////////////////////



#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {

    long long iterations = 10000000000; // 10^10
    long long data = 0xFFFFFFFFFFFFFFFF; 
    long long sumando = 1;
    long long divisor = 1;

    // L3 de cada nodo MN5: 105 MB
    
    //int size = 16 * 1024 * 1024; // 64 MB = int*16*1024*1024
    int size = 256 * 1024; // 1 MB = int*256*1024 
    int *noise = malloc(size * sizeof(int));
    for(int i=0; i<size; i++) noise[i] = i;

    //volatile para que el compilador no optimice lecturas
    volatile int sink; 

    for (long long i = 0; i < iterations; i++) {

        // indice que no pueda predecir el prefetcher
        int index = (i * 103515245 + 12345) % size; 
        sink = noise[index]; 

        sumando = (sumando * 6364136223846793005) + 1;
        divisor = 7 + (sumando & 1) * 4;  //alterna entre 7 y 11
        data = (data / divisor) + sumando;
    }


    printf("Resultado: %lld \n(Ruido final: %d)\n", data, sink);
    free(noise);
    return 0;
}

