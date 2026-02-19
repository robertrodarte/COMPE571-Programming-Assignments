# Programming Assignment 1

Authors: Nick Schwartz, Robert Rodarte

## Multithreading

```
multithreading.h
multithreading.c
```

### Overview:

Calculates the sum (non-inclusive) of the range 0-N by utilizing multithreading.

### Compile:

```
gcc multithreading.c -o multithreading -lpthread
```

### Use:

Run via command line using the following:

Use default options:
N = 10,000,000
Threads = 2

```
./multithreading
```

Set N and Threads:

```
./multithreading $N_VAL $THREADS_VAL
./multithreading 10000000000 4
```
