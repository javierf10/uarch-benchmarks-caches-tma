#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    double data = 1234567.89;
    long long iterations = 10000000000; 
    
    int size = 256 * 1024; 
    int *noise = malloc(size * sizeof(int));
    for(int i=0; i<size; i++) noise[i] = i;

    volatile int sink; 

    for (long long i = 0; i < iterations; i++) {

        int index = (i * 103515245 + 12345) % size; 
        sink = noise[index]; 

        data = sqrt(data);
    }

    printf("Resultado: %f (Ruido final: %d)\n", data, sink);
    free(noise);
    return 0;
}

