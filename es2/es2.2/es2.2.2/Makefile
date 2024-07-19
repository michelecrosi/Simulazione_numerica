CC = g++
CFLAGS = -Wall -O3 --std=c++11

es2.2.2.exe : es2.2.2.o random.o libposizione.o
	$(CC) random.o libposizione.o es2.2.2.o -o es2.2.2.exe
es2.2.2.o : es2.2.2.cpp random.h libposizione.hpp
	$(CC) -c es2.2.2.cpp -o es2.2.2.o $(CFLAGS)
libposizione.o: libposizione.C libposizione.hpp
	$(CC) -c libposizione.C -o libposizione.o
random.o : random.cpp random.h libposizione.hpp
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o es2.2.2.exe seed.out
make esegui :
	./es2.2.2.exe
