CC = g++
CFLAGS = -Wall -O3 --std=c++11

es2.2.1.exe : es2.2.1.o random.o
	$(CC) random.o es2.2.1.o -o es2.2.1.exe
es2.2.1.o : es2.2.1.cpp
	$(CC) -c es2.2.1.cpp -o es2.2.1.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o es2.2.1.exe seed.out
make esegui :
	./es2.2.1.exe
