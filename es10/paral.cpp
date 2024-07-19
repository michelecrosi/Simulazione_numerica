// questo è solo un file che ho usato per fare dei test (si può ignorare)


#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]){
	int size, rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status stat;
	double tstart = MPI_Wtime();
	int n = 100;
	// int* imesg = new int[n];
	// int* copia = new int[n];
	vector<int> imesg (n);
	vector<int> copia(n);
	int sum=0;
	copia[0]=-999;
    
	for(int i=0;i<n;i++){
		imesg[i]=rank;
		sum += imesg[i];
		// cerr << imesg[i] << endl;
		
		if(rank==1){
			for (int i = 0; i < n; i++) {
				copia[i] = imesg[i];
			}
			// copia=imesg;
			MPI_Send/*invia*/(&copia[0]/*a partire dall'indirizzo...*/, n/*n numeri*/, MPI_INTEGER/*interi*/, 0/*al processore 0*/, i/*usando la bandiera identificata con l'intero i*/, MPI_COMM_WORLD/*dell'insieme di processori creato*/);// mandato un vettore
			MPI_Recv(&imesg[0], n, MPI_INTEGER, 0, i+n, MPI_COMM_WORLD, &stat);
		} 
		if(rank==0) {
			MPI_Recv/*ricevi*/(&imesg[0]/*immagazzinando a partire dall'indirizzo...*/, n/*n numeri*/, MPI_INTEGER/*interi*/, 1/*dal processore 1*/, i/*usando la bandiera identificata con l'intero i*/, MPI_COMM_WORLD/*dell'insieme di processori creato*/, &stat /*se ci sono problemi metti informazione su questi in questa variabile*/);
			MPI_Send(&imesg[0], n, MPI_INTEGER, 1, i+n, MPI_COMM_WORLD);
		}
	
	}
	double tend = MPI_Wtime();
	double dt = tend - tstart; // anche se hanno stesso nome sono variabili diversi in processori diversi
	MPI_Barrier(MPI_COMM_WORLD); // sincronizzo
	for(int j=0;j<size;j++){
		if(rank == j) {
			cout << "\nio sono "<< rank << "; somma = "<< sum << "; tempo =" << dt << " vettore di:" << imesg[0] << " riceve: " << copia[0] << endl;
			for(int i=0;i<n;i++){
				cout << rank << "\t" << imesg[i] << endl;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	// delete[] imesg; 
    // delete[] copia; 
	return 0;
}


