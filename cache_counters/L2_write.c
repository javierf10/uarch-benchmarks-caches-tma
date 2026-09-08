#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

// Tamaño L2: 1 MB. Usamos un array de 1.5 MB para desbordarla de forma controlada
#define L2_SIZE (1024 * 1024)
#define WORKING_SET_SIZE (1536 * 1024) // 1.5 MB
#define ARRAY_SIZE (WORKING_SET_SIZE / sizeof(int))

// Desfase seguro: 64 KB (Mayor que L1 de 32KB, pero una fracción pequeña de L2)
#define OFFSET_BYTES (64 * 1024)
#define OFFSET_ELEMENTS (OFFSET_BYTES / sizeof(int))

// 16 enteros = 64 bytes (Línea de caché)
#define STRIDE 16 

#define NUM_REPETITIONS 500000 

int main() {
    // -------------------------------------------------------------------------
    // 1. CONFIGURACIÓN DEL PIPE DE CONTROL DE PERF
    // -------------------------------------------------------------------------
    const char *perf_pipe = "perf_ctl.fifo";
    int perf_pipe_fd = open(perf_pipe, O_WRONLY);
    if (perf_pipe_fd == -1) {
        fprintf(stderr, "Error abriendo la tubería FIFO.\n");
        return EXIT_FAILURE;
    }

    // -------------------------------------------------------------------------
    // 2. INICIALIZACIÓN DE MEMORIA
    // -------------------------------------------------------------------------
    printf("Reservando array de %d KB (1.5x L2) con desfase seguro de %d KB...\n", 
           WORKING_SET_SIZE / 1024, OFFSET_BYTES / 1024);

    volatile int *array = NULL;
    if (posix_memalign((void**)&array, 64, WORKING_SET_SIZE) != 0) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < ARRAY_SIZE; i++) array[i] = i; 

    // -------------------------------------------------------------------------
    // 3. ACTIVACIÓN DE PERF 
    // -------------------------------------------------------------------------
    write(perf_pipe_fd, "enable", 6);

    /// =========================================================================
    // ZONA DE MEDICIÓN (50% L2 Ratio con Desfase Seguro)
    // =========================================================================
    // Restamos el offset del límite para no salirnos del array al hacer 'i + OFFSET'
    size_t limite_bucle = ARRAY_SIZE - OFFSET_ELEMENTS;

    for (int rep = 0; rep < NUM_REPETITIONS; rep++) {
        for (size_t i = 0; i < limite_bucle; i += STRIDE) {
            
            // Acceso 1: Sufre L2 MISS (El desborde de 1.5 MB barrió esta zona)
            array[i] = 0xFF; 

            // Acceso 2: Da L2 HIT (Saltamos 64 KB adelante, donde la línea 
            // mapea en un set libre de la L2)
            array[i + OFFSET_ELEMENTS] = 0xFF; 
        }
    }
    // =========================================================================

    // -------------------------------------------------------------------------
    // 4. DESACTIVACIÓN DE PERF
    // -------------------------------------------------------------------------
    write(perf_pipe_fd, "disable", 7);
    close(perf_pipe_fd);

    // -------------------------------------------------------------------------
    // 5. REPORTE METADATOS DEL BENCHMARK
    // -------------------------------------------------------------------------
    // (limite_bucle / STRIDE) es el número de iteraciones por repetición. 
    // En cada iteración se hacen exactamente 2 lecturas (val1 y val2).
    unsigned long iteraciones_por_rep = limite_bucle / STRIDE;
    unsigned long accesos_por_rep = iteraciones_por_rep * 2; 
    unsigned long accesos_totales = accesos_por_rep * NUM_REPETITIONS;


    printf("\n======================================================================\n");
    printf("                  RESUMEN DE METADATOS DEL BENCHMARK                  \n");
    printf("======================================================================\n");
    printf(" CONFIGURACIÓN DE HARDWARE Y MEMORIA:\n");
    printf("  - Tamaño total del array principal : %d KB (1.5x tamaño de la L2)\n", WORKING_SET_SIZE / 1024);
    printf("  - Número total de elementos        : %lu enteros (int)\n", (unsigned long)ARRAY_SIZE);
    printf("  - Desfase seguro elegido (Offset)  : %d KB (Mayor que L1, menor que L2)\n", OFFSET_BYTES / 1024);
    printf("  - Paso del bucle interno (Stride)  : %d enteros (64 bytes -> 1 Línea de Caché)\n", STRIDE);
    printf("  - Número total de repeticiones     : %d\n", NUM_REPETITIONS);
    
    printf("\n DINÁMICA DEL BUCLE (POR REPETICIÓN):\n");
    printf("  - Elementos evaluados por vuelta   : %lu de %lu totales\n", (unsigned long)limite_bucle, (unsigned long)ARRAY_SIZE);
    printf("  - Pasos del bucle por vuelta       : %lu iteraciones\n", iteraciones_por_rep);
    printf("  - Lecturas en memoria por vuelta   : %lu (1 LOADS_A + 1 LOADS_B por paso)\n", accesos_por_rep);

    free((void *)array);
    return EXIT_SUCCESS;
}