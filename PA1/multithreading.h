#ifndef MULTITHREADING_H
#define MULTITHREADING_H

typedef struct {
    __int128 start;
    __int128 stop;
    __int128 partial;   // partial sum computed by that thread
    int t_id;      // optional, useful for debugging
} 

void *t_id

#endif // MULTITHREADING_H