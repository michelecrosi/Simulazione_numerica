CC = mpicxx 
CFLAGS = -O3 --std=c++11



# all: simulator.exe
# simulator.exe : metropolis.o main.o random.o
# 	$(CC) metropolis.o random.o main.o -o simulator.exe $(AFLAGS)
# main.o : main.cpp metropolis.hpp random.h
# 	$(CC) -c main.cpp -o main.o $(CFLAGS)
# metropolis.o : metropolis.cpp metropolis.hpp random.h
# 	$(CC) -c metropolis.cpp -o metropolis.o $(CFLAGS)
# random.o : random.cpp random.h
# 	$(CC) -c random.cpp -o random.o $(CFLAGS)

all: genera.exe simulator.exe 

generate: genera.exe
genera.exe: genera.o random.o
	$(CC) genera.o random.o -o genera.exe $(CFLAGS)
genera.o: genera.cpp random.h
	$(CC) -c genera.cpp -o genera.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

simulate: simulator.exe
simulator.exe : TSP.o simulator.o random.o
	$(CC) TSP.o random.o simulator.o -o simulator.exe 
simulator.o : simulator.cpp TSP.hpp random.h
	$(CC) -c simulator.cpp -o simulator.o $(CFLAGS)
TSP.o : TSP.cpp TSP.hpp random.h
	$(CC) -c TSP.cpp -o TSP.o $(CFLAGS)

esegui_sim: 
	mpiexec -np 8 ./simulator.exe
clean:
	rm *.o simulator.exe genera.exe
remove:
	rm ./OUTPUT/*.*
