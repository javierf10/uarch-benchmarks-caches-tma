#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(3141592653U); 

    long long iterations = 10000000000; 

    int size = 256 * 1024; 

    int *vector = malloc(size * sizeof(int));

    int *order = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        order[i] = i;
    }

    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    for (int i = 0; i < size - 1; i++) {
        vector[order[i]] = order[i + 1];
    }
    
    vector[order[size - 1]] = order[0];

    long long branch_taken = 0;
    long long x = 0;

    int temp = 0;
    
    for (long long i = 0; i < iterations; i++) {

        temp = vector[temp]; 

        x = (x * 1103515245 + 12345);

        #if LATENCY_LEVEL >= 1
        x ^= 0x5a5a5a5a5a5a5a5aULL; 
        #endif
        #if LATENCY_LEVEL >= 2
        x += 0x123456789abcdef1ULL; 
        #endif
        #if LATENCY_LEVEL >= 3
        x ^= 0x3c3c3c3c3c3c3c3cULL; 
        #endif
        #if LATENCY_LEVEL >= 4
        x += 0x9e3779b97f4a7c15ULL; 
        #endif
        #if LATENCY_LEVEL >= 5
        x ^= 0x0f0f0f0f0f0f0f0fULL; 
        #endif
        #if LATENCY_LEVEL >= 6
        x += 0xa5a5a5a5a5a5a5a5ULL; 
        #endif
        #if LATENCY_LEVEL >= 7
        x ^= 0x5555555555555555ULL; 
        #endif
        #if LATENCY_LEVEL >= 8
        x += 0x7fffffffffffffffULL; 
        #endif
        #if LATENCY_LEVEL >= 9
        x ^= 0xaaaaaaaaaaaaaaaaULL; 
        #endif
        #if LATENCY_LEVEL >= 10
        x += 0xedcba98765432110ULL;
        #endif

        if (x & (1ULL << 60)) { 
            branch_taken++;
	        asm volatile("");
        }

    }

    printf("temp: %d\n", temp);
    
    free(order);
    free(vector);

    return 0;
}
