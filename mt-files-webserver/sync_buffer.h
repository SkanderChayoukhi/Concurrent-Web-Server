#ifndef SYNC_BUFFER_H
#define SYNC_BUFFER_H

void sync_buffer_init(int size);
void sync_buffer_destroy();
void sync_buffer_enqueue(int conn_fd);
int sync_buffer_dequeue();
void sync_buffer_signal_all();

#endif // SYNC_BUFFER_H
