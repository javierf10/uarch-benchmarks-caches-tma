

///////////////////////////////////////////////////
/// CORE BOUND RANDOM MEMORY ACCESS
///////////////////////////////////////////////////



#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    double data = 1234567.89;
    long long iterations = 10000000000; // 10^10
    
    // L2 de cada nodo MN5: 2MB 
    
    int size = 256 * 1024; // 1MB = int*256*1024 
    int *noise = malloc(size * sizeof(int));
    for(int i=0; i<size; i++) noise[i] = i;

    //volatile para que el compilador no optimice lecturas
    volatile int sink; 

    for (long long i = 0; i < iterations; i++) {

        // indice que no pueda predecir el prefetcher
        int index = (i * 103515245 + 12345) % size; 
        sink = noise[index]; 

        data = sqrt(data);
    }

    printf("Resultado: %f (Ruido final: %d)\n", data, sink);
    free(noise);
    return 0;
}

