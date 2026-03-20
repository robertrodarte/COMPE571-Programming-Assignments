# PA2 — CPU Scheduling

Implements and compares CPU scheduling algorithms to simulate a scheduler. Four child processes run a workload at varying loads; the parent acts as the scheduler.

## Files

- **`fcfs.c`** — First Come First Served: runs each process to completion in arrival order
- **`round_robin.c`** — Round Robin: cycles through processes using a configurable time quantum
- **`shortest_job_first.c`** — Shortest Job First: schedules processes in order of expected workload (shortest first)
- **`mlfq.c`** — Multi-Level Feedback Queue: Level 1 is round-robin; processes that exceed the quantum are demoted to Level 2 (FCFS)
- **`sample_program.c`** — Reference template provided with the assignment, not compiled or run directly
- **`test_files/`** — Scratch directory used for experimenting with scheduling variations during development

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
./PA2/round_robin [quantum_microseconds]       # default: 1000 µs
./PA2/shortest_job_first
./PA2/mlfq [quantum_microseconds]              # default: 1000 µs
```

## Metrics Reported

Each scheduler outputs metrics:

- **Response time** — time from process creation to first CPU access
- **Context switches** — number of times each process was stopped and resumed
- **Context switch time** — total and average time spent on context switches
