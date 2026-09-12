#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {

    long long iterations = 10000000000; 
    long long data = 0xFFFFFFFFFFFFFFFF; 
    long long sumando = 1;
    long long divisor = 1;


    int size = 16 * 1024 * 1024; 
    int *noise = malloc(size * sizeof(int));
    for(int i=0; i<size; i++) noise[i] = i;

    volatile int sink; 

    for (long long i = 0; i < iterations; i++) {

        int index = (i * 103515245 + 12345) % size; 
        sink = noise[index]; 

        sumando = (sumando * 6364136223846793005) + 1;
        divisor = 7 + (sumando & 1) * 4;
        data = (data / divisor) + sumando;
    }


    printf("Resultado: %lld \n(Ruido final: %d)\n", data, sink);
    free(noise);
    return 0;
}

