#include <TFT_eSPI.h>

// Cose del display
TFT_eSPI tft = TFT_eSPI();  // Inizializza l'oggetto TFT

// funzione che serve per definire quante righe ha una matrice
#define array_length(x) (sizeof(x) / sizeof(x[0]))

// Struttura dei dati
struct vertici2D {
  float x;
  float y;
};

struct vertici3D {
  float x;
  float y;
  float z;
};

// Forma geometrica
// Coordinate 3D dei vertici
vertici3D vertici[12] = {
  {0.00, -8.00, 0.00},
  {5.79, -3.58, 4.21},
  {-2.21, -3.58, 6.81},
  {-7.16, -3.58, 0.00},
  {-2.21, -3.58, -6.81},
  {5.79, -3.58, -4.21},
  {2.21, 3.58, 6.81},
  {-5.79, 3.58, 4.21},
  {-5.79, 3.58, -4.21},
  {2.21, 3.58, -6.81},
  {7.16, 3.58, 0.00},
  {0.00, 8.00, 0.00},
};

// Indice della matrice dei vertici da collegare a coppie
uint8_t spigoli[30][2] = {
  {3, 4}, {4, 9}, {3, 7}, {0, 2}, {5, 10}, {8, 9}, {0, 5},
  {1, 6}, {9, 11}, {6, 11}, {4, 5}, {4, 8}, {5, 9}, {8, 11},
  {0, 1}, {9, 10}, {1, 2}, {0, 4}, {10, 11}, {2, 7}, {1, 5},
  {6, 7}, {6, 10}, {3, 8}, {0, 3}, {2, 3}, {2, 6}, {1, 10},
  {7, 11}, {7, 8},
};

uint8_t Colore = 0;

// Impostazioni generali
int lunghezzaFocale = 10;
int Distanza = 20;
int zoom = 20;

vertici2D verticiProiettati[12];

void setup() {
  Serial.begin(115200);

  // Inizializza il display TFT
  tft.init();
  tft.setRotation(0);  // Ruota il display se necessario
  tft.fillScreen(TFT_BLACK);  // Pulizia dello schermo
}

void loop() {
  for (int m = 0; m < 3; m++) {
    Colore++;
    if (Colore == 16){
      Colore = 0;
      }

    for (int j = 0; j < 360; j += 3) {
      vertici3D verticiRuotati[array_length(vertici)];

      // Applica la rotazione a tutti i vertici
      for (int g = 0; g < array_length(vertici); g++) {
        if (m == 1) {
          verticiRuotati[g] = RotazioneX(vertici[g], j);
        } else if (m == 2) {
          verticiRuotati[g] = RotazioneY(vertici[g], j);
        } else {
          verticiRuotati[g] = RotazioneZ(vertici[g], j);
        }
      }

      tft.fillScreen(TFT_BLACK);  // Pulizia dello schermo
      Disegnatore(verticiRuotati, Colore);  // Disegna la forma ruotata
      //delay(10);
    }
  }
}

void Disegnatore(vertici3D verticiN[], uint8_t C) {
  // Proiezione dei vertici 3D in 2D
  for (int r = 0; r < array_length(vertici); ++r) {
    verticiProiettati[r] = Proiettore(verticiN[r], lunghezzaFocale);
  }

  // Disegna i lati del cubo
  for (int j = 0; j < array_length(spigoli); ++j) {
    int x0 = verticiProiettati[spigoli[j][0]].x * zoom + 120;  // Offset per il centro
    int y0 = verticiProiettati[spigoli[j][0]].y * zoom + 160;  // Offset per il centro
    int x1 = verticiProiettati[spigoli[j][1]].x * zoom + 120;
    int y1 = verticiProiettati[spigoli[j][1]].y * zoom + 160;

    tft.drawLine(x0, y0, x1, y1, default_4bit_palette[C]);  // Disegna una linea bianca
  }
}

// Calcola le coordinate dei punti 3D in 2D
vertici2D Proiettore(vertici3D punto, int lunghezzaFocale) {
  vertici2D puntoProiettato;

  // Calcola le coordinate proiettate
  puntoProiettato.x = (lunghezzaFocale * punto.x) / (Distanza - punto.z);
  puntoProiettato.y = (lunghezzaFocale * punto.y) / (Distanza - punto.z);

  return puntoProiettato;
}

// Rotazione attorno all'asse X
vertici3D RotazioneX(vertici3D punto, float angolo) {
  vertici3D risultato;
  float rad = radians(angolo);  // Conversione dell'angolo in radianti
  risultato.x = punto.x;
  risultato.y = punto.y * cos(rad) - punto.z * sin(rad);
  risultato.z = punto.y * sin(rad) + punto.z * cos(rad);
  return risultato;
}

// Rotazione attorno all'asse Y
vertici3D RotazioneY(vertici3D punto, float angolo) {
  vertici3D risultato;
  float rad = radians(angolo);  // Conversione dell'angolo in radianti
  risultato.x = punto.x * cos(rad) + punto.z * sin(rad);
  risultato.y = punto.y;
  risultato.z = -punto.x * sin(rad) + punto.z * cos(rad);
  return risultato;
}

// Rotazione attorno all'asse Z
vertici3D RotazioneZ(vertici3D punto, float angolo) {
  vertici3D risultato;
  float rad = radians(angolo);  // Conversione dell'angolo in radianti
  risultato.x = punto.x * cos(rad) - punto.y * sin(rad);
  risultato.y = punto.x * sin(rad) + punto.y * cos(rad);
  risultato.z = punto.z;
  return risultato;
}
