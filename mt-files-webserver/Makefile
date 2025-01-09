# An admittedly primitive Makefile
# To compile, type "make" or make "all"
# To remove files, type "make clean"

CC = gcc
CFLAGS = -Wall
OBJS = wserver.o wclient.o wclient_multithreaded.o request.o io_helper.o sync_buffer.o thread_pool.o spin.o

.SUFFIXES: .c .o

all: wserver wclient wclient_multithreaded spin.cgi chmod_test_requests

wserver: wserver.o request.o io_helper.o sync_buffer.o thread_pool.o
	$(CC) $(CFLAGS) -o wserver wserver.o request.o io_helper.o sync_buffer.o thread_pool.o

wclient: wclient.o io_helper.o
	$(CC) $(CFLAGS) -o wclient wclient.o io_helper.o

wclient_multithreaded: wclient_multithreaded.o io_helper.o
	$(CC) $(CFLAGS) -o wclient_multithreaded wclient_multithreaded.o io_helper.o	

spin.cgi: spin.o
	$(CC) $(CFLAGS) -o spin.cgi spin.o

chmod_test_requests:
	chmod +x test_requests.sh

chmod_testing_multiple_clients:
	chmod +x chmod_testing_multiple_clients.sh	

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f $(OBJS) wserver wclient wclient_multithreaded spin.cgi




