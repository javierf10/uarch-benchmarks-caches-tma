#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(3141592653U));

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
