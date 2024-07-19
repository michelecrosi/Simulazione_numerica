import subprocess

nblocks=20
nsteps=20000

titol=["magnetization","specific_heat","susceptibility","total_energy"]
camp=["h:0","h:0.02"]
metodi=["Gibbs","Metropolis"]

def metodo(i): # converte l'intero che do come input per il tipo specificare il metodo di campionamento nella stringa che mi descrive il metodo di campionamento usato
    if(i==3):
        return "Gibbs"
    if(i==2): 
        return "Metropolis"
    else:
        print("ATTENZIONE! INVALIDO TIPO DI SIMULAZIONE! Questo set-up è pensato per simulazioni del tipo 2 e 3!")

def comando(cartella, comando):
    try:
        # Esegue il comando nella cartella specificata
        process = subprocess.Popen(comando, cwd=cartella, shell=True)
        process.wait()  # Attendere il completamento del processo
    except FileNotFoundError:
        print("Comando non trovato o errore durante l'esecuzione.")



def creadir():
    for i in range(len(titol)):	
        comando(".","mkdir " + titol[i])     
        for j in camp:
            comando("./"+ titol[i],"mkdir "+j) 
            for k in metodi:
                comando(f"./{titol[i]}/{j}","mkdir "+k) 
    for j in camp:
        comando("./NSL_SIMULATOR/INPUT", "mkdir "+j)
        comando("./NSL_SIMULATOR/INPUT/CONFIG", "mkdir "+j)
        for k in metodi:
            comando(f"./NSL_SIMULATOR/INPUT/{j}", "mkdir "+k)
            comando(f"./NSL_SIMULATOR/INPUT/CONFIG/{j}","mkdir "+k) 



def compila():
    comando("./NSL_SIMULATOR/SOURCE", "make")

def input_file(M,T,h):
    if(abs(T-2)<0.00001): 
        rest=0
    else:
        rest=1
    
    contenuto = f"""SIMULATION_TYPE        {M} 1. {h}\nRESTART                {rest}\nTEMP                   {T}\nNPART                  50\nRHO                    1\nR_CUT                  0.0\nDELTA                  0.0\nNBLOCKS                {nblocks}\nNSTEPS                 {nsteps}\n\nENDINPUT"""

    try:
        # Apri il file in modalità scrittura
        with open(f"./NSL_SIMULATOR/INPUT/h:{h}/{metodo(M)}/input_T:{T}.dat", 'w') as file:
            # Scrivi il contenuto nel file
            file.write(contenuto)
        print(f"File 'input_T:{T}.dat input.dat' creato con successo.")
        comando("./NSL_SIMULATOR/INPUT","rm ./input.dat")
        comando("./NSL_SIMULATOR/INPUT", f"ln -s ./h:{h}/{metodo(M)}/input_T:{T}.dat input.dat")
    except IOError:
        print("Errore durante la creazione del file.")


def esegui():
    comando("./NSL_SIMULATOR/SOURCE", "./simulator.exe")

def sposta_output(M,T,h):
    for i in range(len(titol)):
        comando(".",f"cp ./NSL_SIMULATOR/OUTPUT/{titol[i]}.dat ./{titol[i]}/h:{h}/{metodo(M)}/{titol[i]}_T:{T}.dat")
    
def sposta_config(M,T,h):
    comando(".",f"cp ./NSL_SIMULATOR/OUTPUT/CONFIG/config.spin ./NSL_SIMULATOR/INPUT/CONFIG/h:{h}/{metodo(M)}/config_finale_T:{T}.spin")
    comando("./NSL_SIMULATOR/INPUT/CONFIG", "rm ./config.spin")
    comando("./NSL_SIMULATOR/INPUT/CONFIG", f"ln -s ./h:{h}/{metodo(M)}/config_finale_T:{T}.spin config.spin")



# Temp=2.
# H=0
# tipo=3
# # print("ln -s input_T="+str(Temp)+"_h="+str(H)+ ".dat input.dat")
# # comando("./..","code input_T="+str(Temp)+"_h="+str(H)+ ".dat input.dat")

# creadir()
# compila()

# # comando("./../","cp -r h k")
# # comando("./NSL_SIMULATOR/INPUT","readlink input_T="+str(Temp)+"_h="+str(H)+ ".dat" )
# # comando("./NSL_SIMULATOR/INPUT","readlink input.dat" )

# input_file(tipo,Temp,H)

# esegui()
# sposta_output(tipo,Temp,H)
# sposta_config(tipo,Temp,H)




creadir()
compila()

Temp=2.
# print(round(Temp-0.1,1))
H=0
tipo=3 # intero corrispondente al metodo usato per campionare
while(Temp>=0.5):
    input_file(tipo,Temp,H)
    esegui()
    sposta_output(tipo,Temp,H)
    sposta_config(tipo,Temp,H)
    Temp=round(Temp-0.1,1)


Temp=2.
H=0
tipo=2
while(Temp>=0.5):
    input_file(tipo,Temp,H)
    esegui()
    sposta_output(tipo,Temp,H)
    sposta_config(tipo,Temp,H)
    Temp=round(Temp-0.1,1)


Temp=2.
H=0.02
tipo=3
while(Temp>=0.5):
    input_file(tipo,Temp,H)
    esegui()
    sposta_output(tipo,Temp,H)
    sposta_config(tipo,Temp,H)
    Temp=round(Temp-0.1,1)

Temp=2.
H=0.02
tipo=2
while(Temp>=0.5):
    input_file(tipo,Temp,H)
    esegui()
    sposta_output(tipo,Temp,H)
    sposta_config(tipo,Temp,H)
    Temp=round(Temp-0.1,1)


