// Used to pass parameters to pthread functions
struct task_data
{
    long long start;
    long long stop;
    __int128_t sum;
};

void sum_range(struct task_data *data);