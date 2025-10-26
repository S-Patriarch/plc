#!/bin/bash
gcc -c src/conio.c -o bin/conio.o -Wall -O3 -std=c2x
gcc -c src/system.c -o bin/system.o -Wall -O3 -std=c2x
gcc -c src/io.c -o bin/io.o -Wall -O3 -std=c2x
gcc -c src/algorithm.c -o bin/algorithm.o -Wall -O3 -std=c2x
gcc -c src/string.c -o bin/string.o -Wall -O3 -std=c2x
gcc -c src/timer.c -o bin/timer.o -Wall -O3 -std=c2x
gcc -c src/dt.c -o bin/dt.o -Wall -O3 -std=c2x
gcc -c src/line.c -o bin/line.o -Wall -O3 -std=c2x
gcc -c src/window.c -o bin/window.o -Wall -O3 -std=c2x
gcc -c src/indicators.c -o bin/indicators.o -Wall -O3 -std=c2x
gcc -c src/menu.c -o bin/menu.o -Wall -O3 -std=c2x
gcc -c src/tcpip.c -o bin/tcpip.o -Wall -O3 -std=c2x
gcc -c src/logger.c -o bin/logger.o -Wall -O3 -lpthread
gcc -c src/stack.c -o bin/stack.o -Wall -O3 -std=c2x -lpthread
gcc -c src/async.c -o bin/async.o -Wall -O3 -std=c2x -lpthread
