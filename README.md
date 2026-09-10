# No te fíes ni de tus contadores hardware

  

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

[![GCC Version](https://img.shields.io/badge/GCC-11.4%20%7C%2011.5-blue.svg)](https://gcc.gnu.org/)

[![Linux Perf](https://img.shields.io/badge/Tool-Linux%20perf-green.svg)](https://perf.wiki.kernel.org/)

  

Código fuente y scripts del Trabajo de Fin de Grado: **"No te fíes ni de tus contadores hardware"**, realizado en la **Universidad de Zaragoza**.
Este repositorio contiene una suite de *microbenchmarks* deterministas diseñados para evaluar la precisión de los contadores hardware de rendimiento, así como la validez diagnóstica del método **Top-Down Microarchitecture Analysis (TMA)** frente a interferencias de *pipeline* y comportamiento de la jerarquía de memoria.

  

---

## Descripción general

Los *microbenchmarks* están implementados en C con control estricto sobre el código ensamblador generado. Aíslan condiciones microarquitectónicas específicas para validar las métricas reportadas por la herramienta `perf`:

  

* **Interferencia Core Bound vs. Memory Bound:** Evalúa la falsa asignación de estancamientos del *backend* cuando interfieren cadenas de dependencia de alta latencia con lecturas de memoria independientes impredecibles.

* **Interferencia Bad Speculation vs. Backend Bound:** Analiza la sobreestimación de *Bad Speculation* frente a fallos de predicción de salto no críticos que ocurren durante la latencia de un acceso a memoria.

* **Fidelidad de contadores de memoria:** Mide el conteo de accesos, aciertos y fallos en L1D, L2 y L3 bajo patrones de acceso contables.

  

---


## Flujo de trabajo con los benchmarks

### Requisitos previos

Para replicar los experimentos se requiere un entorno Linux con los siguientes paquetes:

* **GCC** (v11.4 o superior)

* **Linux `perf`** (con acceso a los eventos del procesador):

```bash
sudo sysctl -w kernel.perf_event_paranoid=-1
```

### Compilación

#### Interferencia Core Bound vs. Memory Bound:
```bash
gcc -O2 benchmark.c -o benchmark -lm -ffast-math
```

#### Interferencia Bad Speculation vs. Backend Bound
```bash
gcc -O3 benchmark.c -o benchmark
```

#### Fidelidad de contadores de memoria:
```bash
gcc -O2 -fno-tree-vectorize benchmark.c -o benchmark
```


### Caracterización y medición con `perf`

La recolección de eventos hardware se realiza mediante Linux `perf`. 
#### Interferencia Core Bound vs. Memory Bound:
```bash
perf stat -e "{slots,topdown-fe-bound,topdown-be-bound,topdown-bad-spec,topdown-retiring,topdown-mem-bound}" ./benchmark
```

#### Interferencia Bad Speculation vs. Backend Bound
```bash
perf stat -e "{slots,topdown-fe-bound,topdown-be-bound,topdown-bad-spec,topdown-retiring,topdown-mem-bound,topdown-br-mispredict}" ./benchmark
```

#### Fidelidad de contadores de memoria:

Antes de ejecutar las mediciones con control de peticiones, cree la tubería nombrada en el directorio de trabajo:

```bash
mkfifo perf_ctl.fifo
```

**Medición en L1 Cache:**

```bash
perf stat -e mem_inst_retired.all_loads,mem_inst_retired.all_stores,L1-dcache-loads,L1-dcache-load-misses --control fifo:perf_ctl.fifo ./benchmark_L1
```

**Medición en L2 Cache:**

```bash
perf stat -e mem_inst_retired.all_loads,L1-dcache-loads,L1-dcache-load-misses,l2_rqsts.all_demand_data_rd,l2_rqsts.demand_data_rd_miss --control fifo:perf_ctl.fifo ./benchmark_L2
```

**Medición en L3 Cache:**

```bash
perf stat -e L1-dcache-loads,L1-dcache-load-misses,l2_rqsts.all_demand_data_rd,l2_rqsts.demand_data_rd_miss,LLC-loads,LLC-load-misses --control fifo:perf_ctl.fifo ./benchmark_L3
```


## Licencia y Autoría

* **Autor:** Javier F. ([@javierf10](https://github.com/javierf10))
* **Institución:** Universidad de Zaragoza (Unizar) 
* **Licencia:** Este proyecto está licenciado bajo la licencia [GNU General Public License v3.0 (GPLv3)](LICENSE).
