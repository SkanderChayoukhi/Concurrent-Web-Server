#include "io_helper.h"
#include <pthread.h>

static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

#define MAXBUF (16384)



void client_send(int fd, char *filename) {
    char buf[MAXBUF];
    char hostname[MAXBUF];
    
    gethostname_or_die(hostname, MAXBUF);
    
    /* Form and send the HTTP request */
    sprintf(buf, "GET %s HTTP/1.1\n", filename);
    sprintf(buf, "%shost: %s\n\r\n", buf, hostname);
    write_or_die(fd, buf, strlen(buf));
}

//
// Read the HTTP response and print it out
//
void client_print(int fd) {
    char buf[MAXBUF];  
    int n;
    
    // Read and display the HTTP Header 
    n = readline_or_die(fd, buf, MAXBUF);
    while (strcmp(buf, "\r\n") && (n > 0)) {
    pthread_mutex_lock(&print_mutex);  // Lock the mutex
	printf("Header: %s", buf);
    pthread_mutex_unlock(&print_mutex);  // Unlock the mutex
	n = readline_or_die(fd, buf, MAXBUF);
	
	// If you want to look for certain HTTP tags... 
	// int length = 0;
	//if (sscanf(buf, "Content-Length: %d ", &length) == 1) {
	//    printf("Length = %d\n", length);
	//}
    }
    
    // Read and display the HTTP Body 
    n = readline_or_die(fd, buf, MAXBUF);
    while (n > 0) {
    pthread_mutex_lock(&print_mutex);  // Lock the mutex
	printf("%s", buf);
    pthread_mutex_unlock(&print_mutex);  // Unlock the mutex
	n = readline_or_die(fd, buf, MAXBUF);
    }
}


struct client_request_t {
    int clientfd;
    char *filename;
};

void *send_request(struct client_request_t *request ) {
    // Send HTTP request to the server
    client_send(request->clientfd, request->filename);
    client_print(request->clientfd);
    
    close_or_die(request->clientfd);
    free(request);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <host> <port> <filename1> [<filename2> ...]\n", argv[0]);
        exit(1);
    }
    
    char *host = argv[1];
    int port = atoi(argv[2]);
    int num_files = argc - 3;

    pthread_t threads[num_files];  // Array to hold thread IDs

    // Create a thread for each file requested
    for (int i = 0; i < num_files; i++) {
        int clientfd = open_client_fd_or_die(host, port);

        struct client_request_t *request = malloc(sizeof(struct client_request_t));
        if (request == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        request->clientfd = clientfd;
        request->filename = argv[3 + i];  // File name passed from command line

        // Create a thread to handle the request
        if (pthread_create(&threads[i], NULL, send_request, request) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_files; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

