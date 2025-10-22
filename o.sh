#!/bin/bash
gcc -std=c2x -Wall -O3 -c src/conio.c -o bin/conio.o
gcc -std=c2x -Wall -O3 -c src/system.c -o bin/system.o
gcc -std=c2x -Wall -O3 -c src/io.c -o bin/io.o
gcc -std=c2x -Wall -O3 -c src/algorithm.c -o bin/algorithm.o
gcc -std=c2x -Wall -O3 -c src/string.c -o bin/string.o
gcc -std=c2x -Wall -O3 -c src/timer.c -o bin/timer.o
gcc -std=c2x -Wall -O3 -c src/dt.c -o bin/dt.o
gcc -std=c2x -Wall -O3 -c src/line.c -o bin/line.o
gcc -std=c2x -Wall -O3 -c src/window.c -o bin/window.o
gcc -std=c2x -Wall -O3 -c src/indicators.c -o bin/indicators.o
gcc -std=c2x -Wall -O3 -c src/menu.c -o bin/menu.o
gcc -std=c2x -Wall -O3 -c src/logger.c -o bin/logger.o
gcc -std=c2x -Wall -O3 -c src/tcpip.c -o bin/tcpip.o
gcc -std=c2x -Wall -O3 -c src/stack.c -o bin/stack.o -lpthread
gcc -std=c2x -Wall -O3 -c src/async.c -o bin/async.o -lpthread
