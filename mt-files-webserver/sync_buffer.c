#include "sync_buffer.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static int buffer_size;

static int *buffer;
static int head = 0, tail = 0, count = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

static int shutdown_flag = 0;  // Global flag for shutdown

void sync_buffer_init(int size) {
    buffer_size = size;
    buffer = malloc(buffer_size * sizeof(int));
    if (!buffer) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head = tail = count = 0;
}

void sync_buffer_destroy() {
    free(buffer);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
}

void sync_buffer_enqueue(int conn_fd) {
    pthread_mutex_lock(&mutex);

    while (count == buffer_size && !shutdown_flag) {
        pthread_cond_wait(&not_full, &mutex);
    }

    if (shutdown_flag) {
        pthread_mutex_unlock(&mutex);
        return; // Exit if shutdown is in progress
    }

    buffer[tail] = conn_fd;
    tail = (tail + 1) % buffer_size;
    count++;

    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);
}

int sync_buffer_dequeue() {
    pthread_mutex_lock(&mutex);

    // Wait for the buffer to have data or for shutdown to be initiated
    while (count == 0 && !shutdown_flag) {
        if (pthread_cond_wait(&not_empty, &mutex) != 0) {
            pthread_mutex_unlock(&mutex);
            return -1; // Error or shutdown signal
        }
    }

    // If shutdown flag is set, return -1 to exit the thread
    if (shutdown_flag) {
        pthread_mutex_unlock(&mutex);
        printf("Thread exiting due to shutdown flag.\n");
        return -1;
    }

    int conn_fd = buffer[head];
    head = (head + 1) % buffer_size;
    count--;

    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mutex);

    return conn_fd;
}


void sync_buffer_signal_all() {
    printf("Signaling all threads to shut down.\n");

    pthread_mutex_lock(&mutex);
    shutdown_flag = 1;
    pthread_cond_broadcast(&not_empty);  // Wake all waiting threads
    pthread_mutex_unlock(&mutex);
}
