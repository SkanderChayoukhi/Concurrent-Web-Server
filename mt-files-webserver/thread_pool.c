#include "thread_pool.h"
#include "sync_buffer.h"
#include "request.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static pthread_t *threads;
static int num_threads;
static volatile int shutdown_flag = 0;
static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *worker_thread(void *arg);

void thread_pool_init(int num_threads_param, int buffer_size_param) {
    num_threads = num_threads_param;
    threads = malloc(num_threads * sizeof(pthread_t));
    if (!threads) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    sync_buffer_init(buffer_size_param);

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, worker_thread, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
}


void thread_pool_destroy() {
    printf("Starting thread pool destruction...\n");
    shutdown_flag = 1;
    sync_buffer_signal_all();  // Ensure all threads are signaled

    for (int i = 0; i < num_threads; i++) {
        printf("Joining thread %d...\n", i);

        pthread_join(threads[i], NULL);  // Wait for each thread to finish
        printf("Thread %d has been joined.\n", i);  // Debug print
    }

    free(threads);
    sync_buffer_destroy();  // Clean up shared buffer
    printf("Thread pool destroyed successfully.\n");
}


void thread_pool_add_task(int conn_fd) {
    if (!shutdown_flag) {
        sync_buffer_enqueue(conn_fd);
    }
}

static void *worker_thread(void *arg) {
    while (!shutdown_flag) {
        int conn_fd = sync_buffer_dequeue();
        if (conn_fd != -1) {
            request_handle(conn_fd);
            close(conn_fd);

            pthread_mutex_lock(&print_mutex);
            printf("Thread %lu processed connection %d\n", pthread_self(), conn_fd);
            pthread_mutex_unlock(&print_mutex);
        }
    }
    printf("Thread %lu exiting gracefully.\n", pthread_self());

    return NULL;
}
