import subprocess

nblocks=200
nsteps=2000

titol=["solido","liquido","gas"]
Temp=[0.8,1.1,1.2]
rho=[1.1,0.8,0.05]
rc=[2.2,2.5,5.0]
delt=[0.056,0.1,6.42]

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
		comando("./../NSL_SIMULATOR/INPUT","mkdir blocchi_" + titol[i])    

def compila():
	comando("./../NSL_SIMULATOR/SOURCE", "make")

def input_file(stringa,T,rho,rc,delta):
	contenuto = f"SIMULATION_TYPE        1\nRESTART                0\nTEMP                   {T}\nNPART                  108\nRHO                    {rho}\nR_CUT                  {rc}\nDELTA                  {delta}\nNBLOCKS                {nblocks}\nNSTEPS                 {nsteps}\n\nENDINPUT"


	try:
		with open(f"./../NSL_SIMULATOR/INPUT/blocchi_{stringa}/input_{stringa}.dat", 'w') as file:
			# Scrivi il contenuto nel file
			file.write(contenuto)
			comando("./../NSL_SIMULATOR/INPUT","rm ./input.dat")
			comando("./../NSL_SIMULATOR/INPUT", f"ln -s ./blocchi_{stringa}/input_{stringa}.dat input.dat")
	except IOError:
		print("Errore durante la creazione del file.")
	
	comando("./../NSL_SIMULATOR/INPUT", "rm ./properties.dat")
	comando("./../NSL_SIMULATOR/INPUT", "ln -s './properties_U_P_g(r).dat' properties.dat")


def esegui():
	comando("./../NSL_SIMULATOR/SOURCE", "./simulator.exe")

def sposta_output(stringa):
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/potential_energy.dat ./{stringa}/potential_energy.dat")
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/acceptance.dat ./{stringa}/acceptance.dat")
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/pressure.dat ./{stringa}/pressure.dat")
	comando(".",f"cp ./../NSL_SIMULATOR/OUTPUT/gofr.dat ./{stringa}/gofr.dat")


creadir()
compila()
for i in range(len(titol)):	
	print(titol[i])
	input_file(titol[i],Temp[i],rho[i],rc[i],delt[i])
	esegui()
	sposta_output(titol[i])