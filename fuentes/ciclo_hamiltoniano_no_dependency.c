#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));

    long long iterations = 10000000000; // 10^10
    //long long iterations = 100000000;
    // L3 de cada nodo de mi portatil: 12 MB
    //int size = 8 * 1024 * 1024; // 32 MB = 2^25 = int*8*1024*1024 
    // L3 de cada nodo MN5: 105 MB
    //int size = 16 * 1024 * 1024; // 64 MB = 2^26 = int*16*1024*1024  
    //int size = 32 * 1024 * 1024; // 128 MB = 2^27 = int*32*1024*1024  
    //int size = 64 * 1024 * 1024; // 256 MB = 2^28 = int*64*1024*1024  
    //int size = 8 * 1024 * 1024; // 32 MB = 2^25 = int*8*1024*1024  
    int size = 1024 * 1024; // 4 MB = 2^22 = int*512*1024  
    //int size = 512 * 1024; // 2 MB = 2^21 = int*512*1024  
    //int size = 256 * 1024; // 1 MB = 2^20 = int*256*1024  

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

        x = (x * 1103515245 + 12345);
        if (x & (1ULL << 60)) { 
            branch_taken++;
	    asm volatile("");
        }

    }

    printf("Iteraciones totales: %lld\n", iterations);
    printf("Saltos tomados (indices impares): %lld\n", branch_taken);
    printf("Porcentaje de saltos: %.2f%%\n", (double)branch_taken / iterations * 100);
    printf("Tamagno estructura: %d\n", size*4);
    printf("temp: %d\n", temp);
    

    free(order);
    free(vector);

    return 0;
}
