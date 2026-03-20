# COMPE571 – Programming Assignment 2

**Authors:** Nick Schwartz, Robert Rodarte

---

Implements and compares CPU scheduling algorithms to simulate a scheduler. Four child processes run a workload at varying loads. The parent acts as the scheduler.

## Files

- **`fcfs.c`** — First Come First Served: Runs each process to completion in arrival order
- **`round_robin.c`** — Round Robin: Cycles through processes using a configurable time quantum
- **`shortest_job_first.c`** — Shortest Job First: Schedules processes in order of expected workload (shortest first)
- **`mlfq.c`** — Multi-Level Feedback Queue: Level 1 is Round-Robin. Processes that exceed the quantum are demoted to Level 2 (FCFS)
- **`sample_program.c`** — Provided by professor

## Compile

```bash
gcc PA2/fcfs.c -o PA2/fcfs
gcc PA2/round_robin.c -o PA2/round_robin
gcc PA2/shortest_job_first.c -o PA2/shortest_job_first
gcc PA2/mlfq.c -o PA2/mlfq
```

## Run

```bash
./PA2/fcfs
./PA2/round_robin [quantum_microseconds] # default: 1000 µs
./PA2/shortest_job_first
./PA2/mlfq [quantum_microseconds] # default: 1000 µs
```
