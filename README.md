# Concurrent-Web-Server

This project is a multithreaded web server implemented in C. It is designed to handle multiple client requests simultaneously, providing basic web server functionalities such as serving static files and processing HTTP requests.

## Features

- **Multithreading**: The web server can handle multiple client requests concurrently using threads.
- **HTTP Support**: Supports basic HTTP GET requests for static files.
- **Efficient File Serving**: Implements a basic file-serving mechanism for requested resources.
- **Error Handling**: Returns appropriate HTTP status codes for errors like file not found.

## Getting Started

### Prerequisites

- C Compiler (e.g., GCC)
- Make (for building the project)

### Installing

1. Clone the repository:

   ```bash
   git clone [https://github.com/your-username/webserver.git](https://github.com/SkanderChayoukhi/Concurrent-Web-Server.git)
   cd Concurrent-Web-Server
   ```

2. Compile the project:

   ```bash
   make
   ```

3. Run the server:

   ```bash
   ./webserver [port]
   ```

   Replace `[port]` with the port number you want the server to listen on (e.g., `./webserver 8080`).

### Command-line Parameters
The web server is configured and run via command-line arguments. The following options are available:

   ```bash
   ./wserver [-d basedir] [-p port] [-t threads] [-b buffers]
   ```

### Parameters:
-d basedir: The root directory for serving files. Defaults to the current working directory (.).
-p port: The port number the server listens on. Defaults to 10000.
-t threads: The number of worker threads to create. Defaults to 1.
-b buffers: The number of request buffers for handling incoming connections. Defaults to 1.
### Example Usage:
To run the server on port 8003 with 8 threads and a buffer size of 16, use the following command:

   ```bash
   ./wserver -d . -p 8003 -t 8 -b 16
   ```



## Directory Structure

```
/webserver
    /src         - Source files for the server logic
    /include     - Header files
    Makefile     - Makefile for building the project
    README.md    - This file
```

## Security Considerations
When developing a networked server, security is a critical concern. The following considerations will be taken into account:

- **File System Access**: The server will ensure that client requests are constrained to a specified directory (the basedir), preventing access to files outside this directory. Path traversal attacks (e.g., using ../ to access parent directories) will be blocked.
- **Server Shutdown**: The server will ensure it is only running during the testing phase and does not remain active indefinitely, potentially leaving open access to the system.
- **uffer Overflows and Input Validation**: The server will carefully validate input data to prevent common vulnerabilities such as buffer overflows.
