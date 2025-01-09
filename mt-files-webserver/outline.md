

### Command-line Parameter Extension:
   - Add support for new flags `-t` (number of threads) and `-b` (buffer size) in `wserver.c`. Ensure default values:
     - Threads: `1`
     - Buffers: `1`
   - Validate input values for correctness (e.g., positive integers).

### Thread Pool Implementation:
   - **Files to Modify:** `wserver.c`, `request.c`
   - **Architecture:**
     - **Master Thread:**
       - Accept HTTP connections.
       - Place connection descriptors into a shared buffer (bounded buffer implementation).
     - **Worker Threads:**
       - Wait for connections using condition variables.
       - Retrieve connections from the buffer and process them (read requests and serve static or dynamic content).
   - **Key Functions:**
     - `master_thread()`:
       - Use `pthread_create()` to spawn a fixed number of worker threads at startup.
       - Enqueue requests into a synchronized shared buffer.
     - `worker_thread()`:
       - Dequeue requests.
       - Call `request_handle()` for processing.
   - **Synchronization:**
     - Use `pthread_mutex_t` and `pthread_cond_t` for synchronizing buffer access.

### Bounded Buffer Implementation:
   - **Architecture:**
     - Circular queue to store connection descriptors.
     - Buffer size defined by `-b` parameter.
   - **Key Functions:**
     - `enqueue()` and `dequeue()` to manage the buffer.
     - Ensure proper locking and signaling:
       - Master thread blocks if the buffer is full.
       - Worker threads block if the buffer is empty.

### Test and Validation:
   - **Concurrency Testing:**
     - Modify `wclient.c` to send simultaneous requests.
     - Use the `spin.cgi` script to simulate long-running dynamic requests.
   - **Performance Testing:**
     - Measure response times and throughput under load.

### Main files to Changes:
   - `wserver.c`: Main logic for multi-threaded server.
   - `request.c`: Ensure thread-safety during request handling.
   - New files: `thread_pool.c` and `sync_buffer.c` for modular thread-pool and buffer implementations.
