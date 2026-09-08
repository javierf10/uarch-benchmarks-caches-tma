

///////////////////////////////////////////////////
/// CORE BOUND 
///////////////////////////////////////////////////


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {

    long long iterations = 10000000000; // 10^10
    long long data = 0xFFFFFFFFFFFFFFFF; 
    long long sumando = 1;
    long long divisor = 1;

    for (long long i = 0; i < iterations; i++) {

        sumando = (sumando * 6364136223846793005) + 1;
        divisor = 7 + (sumando & 1) * 4;  //alterna entre 7 y 11
        data = (data / divisor) + sumando;
    }


    printf("Resultado: %lld \n", data);
    return 0;
}



