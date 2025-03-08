import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors

# Inizializza il tabellone
tabellone = np.zeros((6, 7), dtype=int)

# Definisci i colori
colors = ['gray', 'blue', 'red']  # 0: grigio, 1: blu, 2: rosso
cmap = matplotlib.colors.ListedColormap(colors)

def add_piece(colonna, giocatore):
    """
    Aggiunge un pezzo al tabellone.
    :param colonna: colonna in cui aggiungere il pezzo
    :param giocatore: giocatore che aggiunge il pezzo (1 o 2)
    :return: True se il pezzo è stato aggiunto, False se la colonna è piena
    """
    for riga in range(5, -1, -1):  # Partiamo dall'ultima riga
        if tabellone[riga][colonna] == 0:
            tabellone[riga][colonna] = giocatore
            return True
    return False  # Se la colonna è piena

# Funzione per disegnare il tabellone
def draw_board():
    plt.clf()  # Pulisce la figura prima di ridisegnarla
    plt.imshow(tabellone, cmap=cmap, origin='upper')  # Disegna il tabellone
    plt.xticks(np.arange(7))  # Mostra le etichette sulle colonne
    plt.yticks([])  # Rimuove le etichette sulle righe
    plt.pause(0.1)  # Aggiorna la finestra senza chiuderla

# Imposta la modalità interattiva di Matplotlib
plt.ion()
fig = plt.figure()

# Loop di gioco
turno = 1
while True:
    draw_board()
    try:
        colonna = int(input(f"Giocatore {turno}, scegli una colonna (0-6): "))
        if 0 <= colonna < 7:
            if add_piece(colonna, turno):
                turno = 3 - turno  # Alterna tra 1 e 2
            else:
                print("Colonna piena! Scegli un'altra colonna.")
        else:
            print("Scelta non valida! Inserisci un numero tra 0 e 6.")
    except ValueError:
        print("Errore! Inserisci un numero valido.")
