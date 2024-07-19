CC = g++
CFLAGS = -Wall -O3 --std=c++11

es2.1.1.exe : es2.1.1.o random.o funzioni.o
	$(CC) funzioni.o random.o es2.1.1.o -o es2.1.1.exe
es2.1.1.o : es2.1.1.cpp
	$(CC) -c es2.1.1.cpp -o es2.1.1.o $(CFLAGS)
funzioni.o : funzioni.C funzioni.hpp
	$(CC) -c funzioni.C -o funzioni.o
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o es2.1.1.exe seed.out
make esegui :
	./es2.1.1.exe
