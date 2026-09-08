

///////////////////////////////////////////////////
/// CORE BOUND RANDOM MEMORY ACCESS
///////////////////////////////////////////////////



#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    double data = 0.5; 
    long long iterations = 10000000000; // 10^10
    
    // L3 de cada nodo MN5: 105 MB
    // L2 de cada nodo MN5: 2 MB 
    // L1d de cada nodo MN5: 48 KB
    
    // 32KB = int*8*1024 

    int size = 8 * 1024; 
    int *noise = malloc(size * sizeof(int));
    for(int i=0; i<size; i++) noise[i] = i;

    //volatile para que el compilador no optimice lecturas
    volatile int sink; 

    for (long long i = 0; i < iterations; i++) {

        // indice que no pueda predecir el prefetcher
        int index = (i * 103515245 + 12345) % size; 
        sink = noise[index]; 

        data = 3.999999 * data * (1.0 - data);

        data = sqrt(data);
    }

    printf("sqrt_dependency_rma_non1");
    printf("Resultado: %f (Ruido final: %d)\n", data, sink);
    free(noise);
    return 0;
}


