# PA3 — Virtual Memory Simulation

**Authors:** Nick Schwartz, Robert Rodarte

## Overview

Simulates virtual memory management with page tables and page replacement. Reads data files (`data/data1.txt`, `data/data2.txt`) and runs them through 4 replacement algorithms: RAND, FIFO, LRU, and PER.

**Background Info:**

- 16-bit virtual addresses (64KB virtual address space)
- 512-byte pages → 128 virtual pages per process
- 32 physical frames shared across all processes

**Metrics reported:** total page faults, total disk references, total dirty page writes.

## Build

```bash
g++ main.cpp memory.cpp loader.cpp algorithm.cpp -o vm_sim
```

## Run

Runs all 4 algorithms against both data files and prints results for each.

```bash
./vm_sim
```
