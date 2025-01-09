#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "request.h"
#include "io_helper.h"
#include "thread_pool.h"

char default_root[] = ".";
volatile sig_atomic_t graceful_shutdown = 0;
int listen_fd;

// Signal handler for shutdown
void handle_shutdown_signal(int signal) {
    if (!graceful_shutdown) {
        graceful_shutdown = 1;
        if (listen_fd != -1) {
            close_or_die(listen_fd);
            listen_fd = -1;
        }
        printf("Signal received, initiating shutdown...\n");
    }
}

int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;
    int num_threads = 4;
    int buffer_size = 16;

    signal(SIGINT, handle_shutdown_signal);
    signal(SIGTERM, handle_shutdown_signal);

    while ((c = getopt(argc, argv, "d:p:t:b:")) != -1) {
        switch (c) {
            case 'd':
                root_dir = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                num_threads = atoi(optarg);
                break;
            case 'b':
                buffer_size = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: wserver [-d basedir] [-p port] [-t threads] [-b buffersize]\n");
                exit(1);
        }
    }

    chdir_or_die(root_dir);
    thread_pool_init(num_threads, buffer_size);
    listen_fd = open_listen_fd_or_die(port);

    printf("Server started on port %d. Press Ctrl+C to shut down.\n", port);

    while (!graceful_shutdown) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);

        int conn_fd = accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);

        if (graceful_shutdown) {
            if (conn_fd >= 0) close_or_die(conn_fd);
            break;
        }

        if (conn_fd >= 0) {
            thread_pool_add_task(conn_fd);
        }
    }

    printf("Shutting down server...\n");
    thread_pool_destroy();
    if (listen_fd != -1) close_or_die(listen_fd);
    printf("Server shut down gracefully.\n");
    return 0;
}
