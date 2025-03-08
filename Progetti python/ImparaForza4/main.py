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
    Aggiunge un pezzo al tabellone
    :param colonna: colonna in cui aggiungere il pezzo
    :param giocatore: giocatore che aggiunge il pezzo che può essere 1 o 2
    :return: True se il pezzo è stato aggiunto, False se la colonna è piena
    """
    for riga in range(6):

        #se si trova all'ultima riga o se in quella corrente è vuota e in quella sotto è piena
        if (riga == 5) or (tabellone[riga][colonna] == 0 and tabellone[riga + 1][colonna] != 0):
            tabellone[riga][colonna] = giocatore
            return True
        
        #se in quella corrente è vuota e in quella sotto è vuota
        elif tabellone[riga][colonna] == 0 and tabellone[riga + 1][colonna] == 0: 
            continue

        #se in quella corrente è piena
        else:
            return False 

# Funzione per disegnare il tabellone
def draw_board():
    plt.imshow(tabellone, cmap=cmap)
    plt.xticks(np.arange(0, 7, 1))  # Mostra le etichette sull'asse x
    plt.yticks([])  # Rimuove le etichette sull'asse y
    plt.show()


while True:
    add_piece(0, input("Giocatore 1, in quale colonna vuoi mettere il pezzo? "))
    draw_board()
    add_piece(1, input("Giocatore 2, in quale colonna vuoi mettere il pezzo? "))
    draw_board()