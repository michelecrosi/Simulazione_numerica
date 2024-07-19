import subprocess

titol=["solido","liquido","gas"]


def comando(cartella, comando):
	try:
		# Esegui il comando
		process = subprocess.Popen(comando, cwd=cartella, shell=True)
		process.wait()  # Attendere il completamento del processo
	except FileNotFoundError:
		print("Comando non trovato o errore durante l'esecuzione.")



def creadir():
	for i in range(len(titol)):	
		comando(".","mkdir " + titol[i])  

def compila():
	comando("./../NSL_SIMULATOR/SOURCE", "make")

def input_file(stringa):
	comando("./../NSL_SIMULATOR/INPUT","rm ./input.dat")
	comando("./../NSL_SIMULATOR/INPUT", f"ln -s ./input_molecular_dinamic/input_{stringa}.dat input.dat")
	comando("./../NSL_SIMULATOR/INPUT", "rm ./properties.dat")
	comando("./../NSL_SIMULATOR/INPUT", "ln -s ./'properties_U_P_g(r)_T.dat' properties.dat")
	
def esegui():
	comando("./../NSL_SIMULATOR/SOURCE", "./simulator.exe")

def sposta_output(stringa):
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/potential_energy.dat ./{stringa}/potential_energy.dat")
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/acceptance.dat ./{stringa}/acceptance.dat")
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/temperature.dat ./{stringa}/temperature.dat")
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/pressure.dat ./{stringa}/pressure.dat")
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/gofr.dat ./{stringa}/gofr.dat")


creadir()
compila()
for i in range(len(titol)):	
	print(titol[i])
	input_file(titol[i])
	esegui()
	sposta_output(titol[i])