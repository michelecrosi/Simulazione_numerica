CC = mpicxx 

paral.exe : paral.cpp
	$(CC) paral.cpp -o paral.exe 
esegui: 
	mpiexec -np 9 ./paral.exe
