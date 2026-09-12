#include <stdio.h>
#include <math.h>

int main() {

    double data = 1234567.89;
    long long iterations = 1000000000;

    for (long long i = 0; i < iterations; i++) {
       
        data = sqrt(data);
    }

    printf("Resultado final: %f\n", data);

    return 0;
}
