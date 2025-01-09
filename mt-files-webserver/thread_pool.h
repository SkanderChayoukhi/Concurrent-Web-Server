#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

// Function prototypes
void thread_pool_init(int num_threads_param, int buffer_size_param);
void thread_pool_destroy();
void thread_pool_add_task(int conn_fd);

#endif // THREAD_POOL_H
