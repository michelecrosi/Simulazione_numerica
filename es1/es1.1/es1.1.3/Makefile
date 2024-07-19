CC = g++
CFLAGS = -Wall -O3 --std=c++11

es1.1.3.exe : es1.1.3.o random.o
	$(CC) random.o es1.1.3.o -o es1.1.3.exe
es1.1.3.o : es1.1.3.cpp
	$(CC) -c es1.1.3.cpp -o es1.1.3.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o es1.1.3.exe seed.out
make esegui :
	./es1.1.3.exe
