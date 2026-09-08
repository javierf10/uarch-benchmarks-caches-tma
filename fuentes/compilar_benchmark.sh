#!/bin/bash

# Comprobamos si se ha pasado un argumento
if [ -z "$1" ]; then
    echo "Uso: $0 archivo.c"
    exit 1
fi

# Variables de archivo
ENTRADA=$1
# Extraemos el nombre sin la extensión .c
SALIDA=$(basename "$ENTRADA" .c)

# Ejecución del comando de compilación
echo "Compilando $ENTRADA..."

gcc -O2 "$ENTRADA" -o "$SALIDA" -lm -ffast-math

# Verificamos si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Listo: Se ha generado el ejecutable '$SALIDA'"
else
    echo "Error: Hubo un problema durante la compilación."
fi
