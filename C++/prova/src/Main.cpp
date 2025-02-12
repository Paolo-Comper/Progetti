#include <iostream>
#include "Objects.h"
#include "Settings.h"

char image[DIMENSIONE_IMAGE_Y][DIMENSIONE_IMAGE_X];

void drawImage() {
    // Cancella la console compatibile con Windows e Linux/MacOS
    #if SystemType == 0
    system("cls");
    #else
    system("clear");
    #endif

    for (int y = 0; y < DIMENSIONE_IMAGE_Y; y++) {
        for (int x = 0; x < DIMENSIONE_IMAGE_X; x++) {
            std::cout << image[y][x];
        }
        std::cout << "\n";
    }
}

void drawPoint(Punto2D punto) {
    image[punto.y][punto.x] = '*';
}

void clearImage() {
    for (int y = 0; y < DIMENSIONE_IMAGE_Y; y++) {
        for (int x = 0; x < DIMENSIONE_IMAGE_X; x++) {
            image[y][x] = '@';
        }
    }
}

int main() {
    clearImage();
    drawPoint({ 10, 10 });
    drawPoint({ 20, 20 });
    drawPoint({ 30, 30 });
    drawImage();
    return 0;
}