# COMPE571 – Programming Assignment 1

**Authors:** Nick Schwartz, Robert Rodarte

---

## Case 1 – Baseline (`baseline.c`)

Sequential implementation. Calculates the sum of integers from 0 to N (non-inclusive) in a single loop.

### Compile

```
gcc baseline.c -o baseline
```

### Run

```
# Default (N = 10,000,000)
./baseline

# Custom N
./baseline <N>
./baseline 1000000000
```

---

## Case 2 – Multithreading (`multithreading.c`)

Divides the workload evenly among `NUM_THREADS` threads using the pthread library. Each thread computes a partial sum over its assigned range and stores the result in a shared `thread_data` struct. The main thread joins all threads and accumulates the final sum.

### Compile

```
gcc multithreading.c -o multithreading -lpthread
```

### Run

```
# Default (N = 100,000,000, NUM_THREADS = 2)
./multithreading

# Custom N and NUM_THREADS
./multithreading <N> <NUM_THREADS>
./multithreading 1000000000 4
```

---

## Case 3, Option 1 – Multitasking via fork() (`multitasking_one.c`)

Divides the workload among `NUM_TASKS` child processes created with `fork()`. Each child writes its partial sum back to the parent through a `pipe()` (using `dup()` to redirect stdout). The parent uses `waitpid()` to synchronize and then reads and accumulates the results.

### Compile

```
gcc multitasking_one.c -o multitasking_one
```

### Run

```
# Default (N = 100,000,000, NUM_TASKS = 2)
./multitasking_one

# Custom N and NUM_TASKS
./multitasking_one <N> <NUM_TASKS>
./multitasking_one 1000000000 4
```

---

## Case 3, Option 2 – Multitasking via popen() (`multitasking_2.c`)

Divides the workload among `NUM_TASKS` child processes created with `popen()`. The parent spawns all children concurrently, each running as a separate instance of the same binary in `--child` mode over a specific range. Children print their partial sums (split into two 64-bit halves) to stdout, which the parent reads back via `fscanf()` through the pipe returned by `popen()`.

### Compile

```
gcc multitasking_2.c -o multitasking_2
```

### Run

```
# Custom N and NUM_TASKS (required)
./multitasking_2 <N> <NUM_TASKS>
./multitasking_2 1000000000 4
```
