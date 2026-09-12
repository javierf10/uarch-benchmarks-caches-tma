#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

#define WORKING_SET_SIZE (29 * 1024 * 1024) 
#define ARRAY_SIZE (WORKING_SET_SIZE / sizeof(int))

#define OFFSET_BYTES (2 * 1024 * 1024)
#define OFFSET_ELEMENTS (OFFSET_BYTES / sizeof(int))

#define STRIDE 16 

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
            
            // Acceso 1: Forzar L3 MISS (El desborde cíclico limpió la L3)
            int val1 = array[i]; 

            // Acceso 2: Forzar L3 HIT (El dato está retenido de forma segura en los sets)
            int val2 = array[i + OFFSET_ELEMENTS]; 
        }
    }
    // =========================================================================

    // -------------------------------------------------------------------------
    // 4. DESACTIVACIÓN DE PERF
    // -------------------------------------------------------------------------
    write(perf_pipe_fd, "disable", 7);
    close(perf_pipe_fd);

    free((void *)array);
    return EXIT_SUCCESS;
}