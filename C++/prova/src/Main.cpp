#include<iostream>
#include <cstdint>
#include "Log.h"

using std::cout, std::cin;

#define ROWS 6
#define COLS 7

#define ROWSOFCONSOLE 4

enum Player {
    PLAYER1 = 1,  // X in rosso
    PLAYER2 = -1, // O in azzurro
    NONE = 0
};

struct Posizione {
    uint8_t rows;
    uint8_t cols;
};

Player tabellone[ROWS][COLS] = {NONE};

void drawTabellone(Player turno) {
    cout << "---{   " << ((turno == PLAYER1) ? RED : BLU) << ((turno == PLAYER1) ? "X" : "O") << RESET << "   }---\n";
    for(int riga = 0; riga < ROWS; riga++) {
        for(int colonna = 0; colonna < COLS; colonna++) {
            cout << '|';
            if(tabellone[riga][colonna] == PLAYER1)
                cout << RED << "X" << RESET;  // X in rosso
            else if(tabellone[riga][colonna] == PLAYER2) 
                cout << BLU << "O" << RESET; // O in azzurro
            else 
                cout << " ";
        }
        cout << "|\n";
    }
    cout << "---------------\n" 
         << " 1 2 3 4 5 6 7 \n";
}

// void drawAll(){
//     system("clear");
//     drawTabellone();

//     cout 
// }

bool dropPiece(Player player, uint8_t colonna) {
    for(int riga = ROWS - 1; riga >= 0; riga--) {
        if(tabellone[riga][colonna] == NONE) {  // Trova la prima riga libera partendo dal basso
            tabellone[riga][colonna] = player;
            return true;
        }
    }
    return false; // Colonna piena
}

Player checkRow(uint8_t riga) {
    uint8_t n_pices = 0;
    Player old_player = tabellone[riga][0];
    Player player;

    for(int colonna = 0; colonna < COLS; colonna++){
        player = tabellone[riga][colonna];

        if (player != NONE) n_pices = (player == old_player) ? n_pices++ : 0;
        if (n_pices == 4) return player;

        old_player = player;
    }
}

Player check(Posizione ultima_mossa, int8_t s_row, int8_t s_col) {
    while (ultima_mossa.rows < ROWS) || (ultima_mossa.rows >= 0) {
        
    }
}

Player checkCol(uint8_t colonna) {
    uint8_t n_pices = 0;
    Player old_player = tabellone[0][colonna];
    Player player;

    for(int riga = 0; riga < ROWS; riga++){
        player = tabellone[riga][colonna];

        if (player != NONE) n_pices = (player == old_player) ? n_pices++ : 0;
        if (n_pices == 4) return player;

        old_player = player;
    }
}

Player checkOblDown(Posizione ultima_mossa) {

}

Player checkOblUp(Posizione ultima_mossa) {

}

Player checkWin(Posizione ultima_mossa){

    /*Lo deve fare sempre   */ checkRow(ultima_mossa.rows);
    if (ultima_mossa.rows < 4) checkCol(ultima_mossa.cols);
    if ( )


}

int main() {
    Player turn = PLAYER1;
    while (true) {
        drawTabellone(turn);
        int colonna;
        cout << "Inserisci la posizione: ";
        cin  >> colonna;
        if(colonna < 1 || colonna > 7) {
            log("Colonna non valida", ERROR);
            continue;
        }
        if(!dropPiece(turn, colonna - 1)) {
            log("Colonna piena", WARNING);
            continue;
        }

        turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
    }
    return 0;

    
}

//TODO: Trovare un modo per loggare i messaggi e fare clear della console contemporaneamente
//TODO: Implementare la logica per il controllo della vittoria