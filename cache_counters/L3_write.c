#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

// Tamaño exacto calibrado: 29 MB (1.5x tu L3 de ~19.3 MB)
#define WORKING_SET_SIZE (29 * 1024 * 1024) 
#define ARRAY_SIZE (WORKING_SET_SIZE / sizeof(int))

// Desfase seguro para L3: 2 MB (Salta L2, pero es insignificante para tus 28672 sets de L3)
#define OFFSET_BYTES (2 * 1024 * 1024)
#define OFFSET_ELEMENTS (OFFSET_BYTES / sizeof(int))

// 16 enteros = 64 bytes (1 Línea de Caché)
#define STRIDE 16 

// Número de repeticiones adaptado al nuevo tamaño
#define NUM_REPETITIONS 250000 

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
    printf("Reservando array de %d KB (1.5x L3) con desfase de %d KB...\n", 
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
    // ZONA DE MEDICIÓN (50% L3 Ratio Exacto)
    // =========================================================================
    size_t limite_bucle = ARRAY_SIZE - OFFSET_ELEMENTS;

    for (int rep = 0; rep < NUM_REPETITIONS; rep++) {
        for (size_t i = 0; i < limite_bucle; i += STRIDE) {
            
            array[i] = 0xFF; 

            // Acceso 2: Da L2 MISS y prepara la L2 para que el acceso 1 de hit en el futuro
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
    unsigned long iteraciones_por_rep = limite_bucle / STRIDE;
    unsigned long accesos_por_rep = iteraciones_por_rep * 2; 
    unsigned long accesos_totales = accesos_por_rep * NUM_REPETITIONS;
    unsigned long trafico_ram_bytes_teorico = (accesos_totales / 2) * 64;

    printf("\n======================================================================\n");
    printf("                  RESUMEN DE METADATOS DEL BENCHMARK (L3 Exacto)      \n");
    printf("======================================================================\n");
    printf(" CONFIGURACIÓN DE HARDWARE Y MEMORIA:\n");
    printf("  - Tamaño detectado L3              : 19.3 MB (%d sets)\n", 28672);
    printf("  - Tamaño del array principal       : %d MB (1.5x L3 aproximado)\n", WORKING_SET_SIZE / (1024 * 1024));
    printf("  - Desfase elegido (Offset)         : %d MB\n", OFFSET_BYTES / (1024 * 1024));
    printf("  - Paso del bucle interno (Stride)  : %d enteros (64 bytes)\n", STRIDE);
    printf("  - Número total de repeticiones     : %d\n", NUM_REPETITIONS);
    
    printf("\n DINÁMICA DEL BUCLE (POR REPETICIÓN):\n");
    printf("  - Pasos del bucle por vuelta       : %lu iteraciones\n", iteraciones_por_rep);
    printf("  - Lecturas en memoria por vuelta   : %lu\n", accesos_por_rep);

    printf("\n PREDICCIÓN MATEMÁTICA PARA VALIDAR EN PERF:\n");
    printf("  --------------------------------------------------------------------\n");
    printf("  => [mem_inst_retired.all_loads]         : %lu\n", accesos_totales);
    printf("  => [longest_latency_cache.reference]    : %lu\n", accesos_totales);
    printf("  => [longest_latency_cache.miss]         : %lu (Ratio L3-Miss: 50.00%%)\n", accesos_totales / 2);
    printf("  --------------------------------------------------------------------\n");
    
    printf("\n TRÁFICO HACIA LA MEMORIA PRINCIPAL (RAM):\n");
    printf("  - Volumen total de datos leídos    : %.2f GB\n", (double)(accesos_totales * sizeof(int)) / (1024.0 * 1024.0 * 1024.0));
    printf("  - Tráfico provocado hacia RAM      : %.2f GB (Solo L3 Misses)\n", (double)trafico_ram_bytes_teorico / (1024.0 * 1024.0 * 1024.0));
    printf("======================================================================\n\n");

    free((void *)array);
    return EXIT_SUCCESS;
}