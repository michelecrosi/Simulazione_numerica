import subprocess


titol=["solido","liquido","gas"]


def comando(cartella, comando):
	try:
		# Esegui il comando nella cartella specificata
		process = subprocess.Popen(comando, cwd=cartella, shell=True)
		process.wait()  # Attendere il completamento del processo
	except FileNotFoundError:
		print("Comando non trovato o errore durante l'esecuzione.")



def creadir():
	for i in range(len(titol)):	
		comando(".","mkdir " + titol[i])     

def compila():
	comando("./NSL_SIMULATOR/SOURCE", "make")

def input_file(stringa):
	try:
		comando("./NSL_SIMULATOR/INPUT","rm ./input.dat")
		comando("./NSL_SIMULATOR/INPUT", f"ln -s input_{stringa}.dat input.dat")
	except IOError:
		print("Errore durante la creazione del file.")
	comando("./NSL_SIMULATOR/INPUT", "rm ./properties.dat")
	comando("./NSL_SIMULATOR/INPUT", "ln -s './properties_U.dat' properties.dat")

def esegui():
	comando("./NSL_SIMULATOR/SOURCE", "./simulator.exe")

def sposta_output(stringa):
	comando(".",f"cp ./NSL_SIMULATOR/OUTPUT/potential_energy.dat ./{stringa}/potential_energy.dat")
	comando(".",f"cp ./NSL_SIMULATOR/OUTPUT/acceptance.dat ./{stringa}/acceptance.dat")



creadir()
compila()
D=0
for i in titol:
	if(D!=0):
		print(i)
		input_file(i)
		esegui()
		sposta_output(i)
	D+=1
