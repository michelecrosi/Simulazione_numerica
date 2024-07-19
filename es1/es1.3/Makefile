CC = g++
CFLAGS = -Wall -O3 --std=c++11

es1.3.exe : es1.3.o random.o Buffon.o
	$(CC) random.o Buffon.o es1.3.o -o es1.3.exe
es1.3.o : es1.3.cpp
	$(CC) -c es1.3.cpp -o es1.3.o $(CFLAGS)
Buffon.o : Buffon.cpp Buffon.h random.h
	$(CC) -c Buffon.cpp -o Buffon.o
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o es1.3.exe seed.out
make esegui :
	./es1.3.exe
