#!/bin/bash

HOST="localhost"   # Server hostname
PORT=10000          # Server port
FILES=(
    "spin.cgi?3"
    "outline.md"
    "spin.c"
    "test_requests.sh"
    "wclient.c"
    "io_helper.h"
    "request.c"
    "sync_buffer.c"
    "thread_pool.c"
    "wserver.c"
    "Makefile"
    "request.h"
    "sync_buffer.h"
    "thread_pool.h"
)

# Loop through the list of files and send requests
for FILENAME in "${FILES[@]}"; do
    echo "Requesting file: $FILENAME"
    ./wclient "$HOST" "$PORT" "/$FILENAME" #> "${FILENAME}.txt" 2>&1
done

# Wait for all background processes to finish
# wait


echo "All requests completed."
