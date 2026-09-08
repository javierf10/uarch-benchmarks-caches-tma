
///////////////////////////////////////////////////
/// CORE BOUND BASE
///////////////////////////////////////////////////



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







/////V1 MALA

// int main() {
//     //volatile para evitar que el compilador se salte el bucle
//     // volatile double data = 1234567.89;
//     double data = 1234567.89;
//     long long iterations = 1000000000; // 100 millones
    
//     // clock_t start = clock();

//     for (long long i = 0; i < iterations; i++) {
       
//         data = sqrt(data);
//         // data = data + 0.000000001;
//         // data = data * data;
//         // data = data / 1.000000001;
//     }

//     // clock_t end = clock();
//     // double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

//     // printf("Resultado final: %f\n", data);
//     // printf("Tiempo de ejecucion: %f segundos\n", time_spent);

//     return 0;
// }