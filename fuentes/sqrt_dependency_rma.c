

///////////////////////////////////////////////////
/// CORE BOUND RANDOM MEMORY ACCESS
///////////////////////////////////////////////////



#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    double data = 1234567.89;
    long long iterations = 1000000000;
    
    // L3 de cada nodo MN5: 105 MB
    
    int size = 16 * 1024 * 1024; // array de 64MB
    int *noise = malloc(size * sizeof(int));
    for(int i=0; i<size; i++) noise[i] = i;

    //volatile para que el compilador no optimice lecturas
    volatile int sink; 

    for (long long i = 0; i < iterations; i++) {

        // indice que no pueda predecir el prefetcher
        int index = (i * 1103515245 + 12345) % size; 
        sink = noise[index]; 

        // 3. Operación principal
        data = sqrt(data);
    }

    printf("Resultado: %f (Ruido final: %d)\n", data, sink);
    free(noise);
    return 0;
}



// int main() {
//     //volatile para evitar que el compilador se salte el bucle
//     // volatile double data = 1234567.89;
//     double data = 1234567.89;
//     long long iterations = 1000000000; // 100 millones
    
//     // clock_t start = clock();

//     for (long long i = 0; i < iterations; i++) {
       
//         data = sqrt(data);
//         // data = data + 0.000000001;
//         // data = data * data;
//         // data = data / 1.000000001;
//     }

//     // clock_t end = clock();
//     // double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

//     // printf("Resultado final: %f\n", data);
//     // printf("Tiempo de ejecucion: %f segundos\n", time_spent);

//     return 0;
// }