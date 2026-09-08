#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(3141592653U); 
    //srand(time(NULL));

    long long iterations = 10000000000; // 10^10

    // L2 de cada nodo MN5: 2 MB
    //int size = 1024 * 1024; // 4 MB = 2^22 = int*1024*1024  
    //int size = (256 + 512) * 1024; // 3 MB   
    //int size = 512 * 1024; // 2 MB = 2^21 = int*512*1024  
    //int size = (256 + 128) * 1024; // 1.5 MB   
    //int size = 128 * 1024; // 0.5 MB  

    int size = 256 * 1024; // 1 MB = 2^20   

    int *vector = malloc(size * sizeof(int));
    //if (vector == NULL) return 1;

    int *order = malloc(size * sizeof(int));
    //if (order == NULL) {
    //    free(vector);
    //    return 1;
    //}

    // inicializar índices
    for (int i = 0; i < size; i++) {
        order[i] = i;
    }

    // mezclar índices (Algoritmo Fisher-Yates)
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    // coonectar los elementos 
    for (int i = 0; i < size - 1; i++) {
        vector[order[i]] = order[i + 1];
    }
    
    // cerrar el ciclo 
    vector[order[size - 1]] = order[0];

    long long branch_taken = 0;
    long long x = 0;

    int temp = 0;
    
    for (long long i = 0; i < iterations; i++) {

        temp = vector[temp]; 

       // x = (x * 1103515245 + 12345);

/*
#if LATENCY_LEVEL >= 1
x ^= 0x5a5a5a5a5a5a5a5aULL; // 1 ciclo (XOR)
#endif
#if LATENCY_LEVEL >= 2
x += 0x123456789abcdef1ULL; // 1 ciclo (ADD)
#endif
#if LATENCY_LEVEL >= 3
x ^= 0x3c3c3c3c3c3c3c3cULL; // 1 ciclo (XOR)
#endif
#if LATENCY_LEVEL >= 4
x += 0x9e3779b97f4a7c15ULL; // 1 ciclo (ADD)
#endif
#if LATENCY_LEVEL >= 5
x ^= 0x0f0f0f0f0f0f0f0fULL; // 1 ciclo (XOR)
#endif
#if LATENCY_LEVEL >= 6
x += 0xa5a5a5a5a5a5a5a5ULL; // 1 ciclo (ADD)
#endif
#if LATENCY_LEVEL >= 7
x ^= 0x5555555555555555ULL; // 1 ciclo (XOR)
#endif
#if LATENCY_LEVEL >= 8
x += 0x7fffffffffffffffULL; // 1 ciclo (ADD)
#endif
#if LATENCY_LEVEL >= 9
x ^= 0xaaaaaaaaaaaaaaaaULL; // 1 ciclo (XOR)
#endif
#if LATENCY_LEVEL >= 10
x += 0xedcba98765432110ULL; // 1 ciclo (ADD)
#endif
*/

       /*
        if (x & (1ULL << 60)) { 
            branch_taken++;
	    asm volatile("");
        }
	*/

    }

    printf("Iteraciones totales: %lld\n", iterations);
   // printf("Saltos tomados (indices impares): %lld\n", branch_taken);
   // printf("Porcentaje de saltos: %.2f%%\n", (double)branch_taken / iterations * 100);
    printf("Tamagno estructura: %d\n", size*4);
    printf("temp: %d\n", temp);
    

    free(order);
    free(vector);

    return 0;
}
