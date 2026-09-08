#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

// tamaño L2: 1MB

// num componentes array: 2^20
//#define ARRAY_SIZE (4 * 1024 * 1024 / sizeof(int)) 

//64 KB
#define WORKING_SET_SIZE (64 * 1024) 
#define ARRAY_SIZE (WORKING_SET_SIZE / sizeof(int))

//32 KB
#define L1_SIZE (32 * 1024)
#define FLUSHER_SIZE (L1_SIZE / sizeof(int))

#define NUM_REPETITIONS 50000000

int main() {
    // -------------------------------------------------------------------------
    // 1. CONFIGURACIÓN DEL PIPE DE CONTROL DE PERF
    // -------------------------------------------------------------------------
    const char *perf_pipe = "perf_ctl.fifo";
    const char *perf_enable = "enable";
    const char *perf_disable = "disable";

    // El programa se pausa a hasta que 'perf' 
    // se conecte al otro extremo del archivo FIFO.

    printf("Abriendo la tubería de control '%s'...\n", perf_pipe);
    int perf_pipe_fd = open(perf_pipe, O_WRONLY);
    if (perf_pipe_fd == -1) {
        fprintf(stderr, "No se pudo abrir el FIFO.\n");
        return EXIT_FAILURE;
    }

    // -------------------------------------------------------------------------
    // 2. INICIALIZACIÓN DE MEMORIA
    // -------------------------------------------------------------------------
    printf("Reservando e inicializando array de %d KB...\n", WORKING_SET_SIZE / 1024);

    // se reserva memoria para el array de enteros de ARRAY_SIZE
    volatile int *array = (volatile int *)malloc(ARRAY_SIZE * sizeof(int));
    
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i; 
    }


    // Array auxiliar para llenar la L1d de basura
    volatile int *flusher = (volatile int *)malloc(FLUSHER_SIZE * sizeof(int));
    for (size_t i = 0; i < FLUSHER_SIZE; i++) {
        flusher[i] = 0x12345678; 
    }

    // -------------------------------------------------------------------------
    // 3. ACTIVACIÓN DE PERF 
    // -------------------------------------------------------------------------
    printf("Enviando 'enable' a perf e iniciando bucle\n");
    // se escribe la palabra "enable" en el FIFO. Perf la leerá y encenderá los contadores.
    write(perf_pipe_fd, perf_enable, strlen(perf_enable));



    /// =========================================================================
    // ZONA DE MEDICIÓN
    // =========================================================================
    // Se salta de 8 en 8 enteros (32 bytes) para tocar cada línea de caché 
    //  2 veces: miss, y luego hit

    //como el array es el doble de grande que la L1, para cuando se termina la repeticion
    // la L1 ya no tiene los datos anteriores 
    for (int rep = 0; rep < NUM_REPETITIONS; rep++) {
        for (size_t i = 0; i < ARRAY_SIZE; i += 8) {
            int val = array[i]; 
        }
    }
    // =========================================================================



    // -------------------------------------------------------------------------
    // 4. DESACTIVACIÓN DE PERF (STOP)
    // -------------------------------------------------------------------------
    // El bucle terminó, enviamos "disable" inmediatamente para congelar contadores.
    write(perf_pipe_fd, perf_disable, strlen(perf_disable));
    printf("Bucle terminado. Enviado 'disable' a perf.\n");

    // Cerrar el descriptor de la tubería
    close(perf_pipe_fd);


    // -------------------------------------------------------------------------
    // 5. REPORTE METADATOS DEL BENCHMARK
    // -------------------------------------------------------------------------
    printf("\n==================================================\n");
    printf("        RESUMEN DE METADATOS DEL BENCHMARK        \n");
    printf("==================================================\n");
    printf("Tamaño del array principal : %d KB\n", WORKING_SET_SIZE / 1024);
    printf("Número total de elementos  : %lu enteros (int)\n", (unsigned long)ARRAY_SIZE);
    printf("Tamaño del flusher inicial : %d KB\n", L1_SIZE / 1024);
    printf("Número de repeticiones     : %d\n", NUM_REPETITIONS);
    printf("Paso del bucle (Stride)    : 8 enteros (32 bytes)\n");
    printf("--------------------------------------------------\n");
    
    unsigned long accesos_por_rep = ARRAY_SIZE / 8;
    unsigned long accesos_totales = accesos_por_rep * NUM_REPETITIONS;
    printf("Lecturas L1 esperadas (Totales) : %lu\n", accesos_totales);
    printf("Fallos L1 esperados (Teóricos)  : %lu (Ratio: 50.00%%)\n", accesos_totales / 2);
    printf("==================================================\n\n");


    // Limpieza final de memoria 
    free((void *)array);
    free((void *)flusher);
    printf("Programa finalizado.\n");

    return EXIT_SUCCESS;
}

