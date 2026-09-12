#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    double data = 0.5; 
    long long iterations = 10000000000; 
    
    
    int size = 256 * 1024; 
    int *noise = malloc(size * sizeof(int));
    for(int i=0; i<size; i++) noise[i] = i;

    volatile int sink; 

    for (long long i = 0; i < iterations; i++) {

        int index = (i * 103515245 + 12345) % size; 
        sink = noise[index]; 

        data = 3.999999 * data * (1.0 - data);
        data = sqrt(data);
    }

    printf("Resultado: %f (Ruido final: %d)\n", data, sink);
    free(noise);
    return 0;
}


