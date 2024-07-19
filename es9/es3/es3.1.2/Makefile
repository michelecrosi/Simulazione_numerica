CC = g++
CFLAGS = -Wall -O3 --std=c++11

es3.1.2.exe : es3.1.2.o random.o finanza.o
	$(CC) random.o finanza.o es3.1.2.o -o es3.1.2.exe
es3.1.2.o : es3.1.2.cpp
	$(CC) -c es3.1.2.cpp -o es3.1.2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
finanza.o: finanza.cpp finanza.h 
	$(CC) -c finanza.cpp -o finanza.o
clean :
	rm *.o es3.1.2.exe seed.out
make esegui :
	./es3.1.2.exe
