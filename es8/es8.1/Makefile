CC = g++
CFLAGS = -O3 --std=c++11
AFLAGS = -larmadillo
all: simulator.exe
simulator.exe : metropolis.o main.o random.o
	$(CC) metropolis.o random.o main.o -o simulator.exe $(AFLAGS)
main.o : main.cpp metropolis.hpp random.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)
metropolis.o : metropolis.cpp metropolis.hpp random.h
	$(CC) -c metropolis.cpp -o metropolis.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o simulator.exe
remove:
	rm ./OUTPUT/*.*
