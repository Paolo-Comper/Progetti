#include<iostream>
#include <cstdint>
#include "Log.h"
#include <deque>

using std::cout, std::cin, std::deque, std::string, std::pair;

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

Posizione ultima_mossa;

void drawTabellone(Player turno) {
    system("clear");
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

bool dropPiece(Player player, uint8_t colonna) {
    for(int riga = ROWS - 1; riga >= 0; riga--) {
        if(tabellone[riga][colonna] == NONE) {  // Trova la prima riga libera partendo dal basso
            tabellone[riga][colonna] = player;
            ultima_mossa.rows = riga;
            ultima_mossa.cols = colonna;
            return true;
        }
    }
    return false; // Colonna piena
}

bool check(Player player, int8_t s_row, int8_t s_col) {
    uint8_t n_pices = 1;
    Posizione pos_corrente;

    for (int i = 1; i <= 3; i++) {
        pos_corrente.rows = ultima_mossa.rows + (i * s_row);
        pos_corrente.cols = ultima_mossa.cols + (i * s_col);

        if ((pos_corrente.rows < 0) || 
            (pos_corrente.rows >= ROWS) || 
            (pos_corrente.cols < 0) || 
            (pos_corrente.cols >= COLS))
            break;

        else if (tabellone[pos_corrente.rows][pos_corrente.cols] == player)
            n_pices++;

        else
            break;
    }

    for (int i = 1; i <= 3; i++) {
        pos_corrente.rows = ultima_mossa.rows + (i * (- s_row));
        pos_corrente.cols = ultima_mossa.cols + (i * (- s_col));

        if ((pos_corrente.rows < 0) || 
            (pos_corrente.rows >= ROWS) || 
            (pos_corrente.cols < 0) || 
            (pos_corrente.cols >= COLS))
            break;

        else if (tabellone[pos_corrente.rows][pos_corrente.cols] == player)
            n_pices++;

        else
            break;
    }

    return (n_pices == 4);
}

Player checkWin(){
    Player player = tabellone[ultima_mossa.rows][ultima_mossa.cols];
    return (check(player, 0, 1) || 
            check(player, 1, 0) || 
            check(player, 1, 1) || 
            check(player, 1, -1)) ? player : NONE;

}

int main() {
    Player turn = PLAYER1;
    svuotaLogFile();
    while (true) {
        drawTabellone(turn);
        int colonna = -1;
        cout << "Inserisci la posizione: ";
        cin  >> colonna;
        if(colonna < 1 || colonna > 7) {
            logToFile("Colonna non valida", ERROR);
            continue;
        }
        if(!dropPiece(turn, colonna - 1)) {
            logToFile("Colonna piena", WARNING);
            continue;
        }
        if(checkWin() != NONE) break;
        turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
    }

    drawTabellone(turn);
    cout << ((checkWin() == PLAYER1) ? RED : BLU) << R"(
     _____                                              _____ 
    ( ___ )--------------------------------------------( ___ )
     |   |                                              |   | 
     |   |                                              |   | 
     |   |    _   _       _         _       _           |   | 
     |   |   | | | | __ _(_) __   _(_)_ __ | |_ ___     |   | 
     |   |   | |_| |/ _` | | \ \ / / | '_ \| __/ _ \    |   | 
     |   |   |  _  | (_| | |  \ V /| | | | | || (_) |   |   | 
     |   |   |_| |_|\__,_|_|   \_/ |_|_| |_|\__\___/    |   | 
     |   |                                              |   | 
     |___|                                              |___| 
    (_____)--------------------------------------------(_____)
    )" << RESET;

    return 0;
}
