#include <stdio.h>
#include <math.h>

int main() {

    double data = 0.5;
    long long iterations = 10000000000; 

    for (long long i = 0; i < iterations; i++) {
      
	    data = 3.999999 * data * (1.0 - data);
        data = sqrt(data);
    }

    printf("Resultado final: %f\n", data);

    return 0;
}

