#!/bin/bash

C_FILE="ciclo_hamiltoniano_arit_1.c"


for i in $(seq 1 10); do
    BINARY_NAME="ciclo_hamiltoniano_arit_det${i}"

    echo "--------------------------------------------"
    echo "Procesando Nivel de Latencia: ${i}"

    # 1. Compilación con -O3 y pasando la macro del nivel de operaciones
    echo "-> Compilando $BINARY_NAME..."
    gcc -O3 -DLATENCY_LEVEL=${i} $C_FILE -o $BINARY_NAME
    
    if [ $? -ne 0 ]; then
        echo "Error compilando nivel ${i}. Abortando."
        exit 1
    fi

done
