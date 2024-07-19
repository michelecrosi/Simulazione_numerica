CC = g++
CFLAGS = -Wall -O3 --std=c++11

es2.1.2.exe : es2.1.2.o random.o funzioni.o
	$(CC) funzioni.o random.o es2.1.2.o -o es2.1.2.exe
es2.1.2.o : es2.1.2.cpp random.h funzioni.hpp
	$(CC) -c es2.1.2.cpp -o es2.1.2.o $(CFLAGS)
funzioni.o : funzioni.C funzioni.hpp
	$(CC) -c funzioni.C -o funzioni.o
random.o : random.cpp random.h funzioni.hpp
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o es2.1.2.exe seed.out
make esegui :
	./es2.1.2.exe
