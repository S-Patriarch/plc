#!/bin/bash
gcc -c src/conio.c -o bin/conio.o -Wall
gcc -c src/system.c -o bin/system.o -Wall -lpthread
gcc -c src/io.c -o bin/io.o -Wall
gcc -c src/algorithm.c -o bin/algorithm.o -Wall
gcc -c src/string.c -o bin/string.o -Wall
gcc -c src/timer.c -o bin/timer.o -Wall
gcc -c src/dt.c -o bin/dt.o -Wall
gcc -c src/line.c -o bin/line.o -Wall
gcc -c src/window.c -o bin/window.o -Wall
gcc -c src/indicators.c -o bin/indicators.o -Wall
gcc -c src/menu.c -o bin/menu.o -Wall
gcc -c src/tcpip.c -o bin/tcpip.o -Wall
gcc -c src/logger.c -o bin/logger.o -Wall -lpthread
gcc -c src/stack.c -o bin/stack.o -Wall -lpthread
gcc -c src/async.c -o bin/async.o -Wall -lpthread
gcc -c src/file.c -o bin/file.o -Wall
gcc -c src/error.c -o bin/error.o -Wall
gcc -c src/crypto.c -o bin/crypto.o -Wall
gcc -c src/bitset.c -o bin/bitset.o -Wall
gcc -c src/inet.c -o bin/inet.o -Wall
gcc -c src/ringbuf.c -o bin/ringbuf.o -Wall
gcc -c src/thread.c -o bin/thread.o -Wall
gcc -c src/sds.c -o bin/sds.o -Wall
gcc -c src/hashmap.c -o bin/hashmap.o -Wall
gcc -c src/net.c -o bin/net.o -Wall
gcc -c src/terminal.c -o bin/terminal.o -Wall
gcc -c src/regexp.c -o bin/regexp.o -Wall