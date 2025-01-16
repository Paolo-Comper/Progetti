#include <DHT.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <string.h>

// Cose del display
TFT_eSPI tft = TFT_eSPI();  // Inizializza l'oggetto TFT

// Pin del sensore e tipo di sensore
#define DHTPIN 22
#define DHTTYPE DHT22

// Inizializza il sensore e il file SD
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inizializzazione Serial Monitor
  Serial.begin(115200);
  Serial.println("Inizializzazione...");

  // Inizializza il sensore DHT
  dht.begin();

  // Inizializza il display TFT
  tft.init();
  tft.setRotation(1);  // Ruota il display se necessario
  tft.fillScreen(TFT_BLACK);  // Pulizia dello schermo

  tft.setCursor(0,40,4);
  tft.setTextColor(TFT_WHITE);
}

void loop() {
  // Legge i valori dal sensore
  float temperatura = dht.readTemperature();
  float umidita = dht.readHumidity();

  tft.fillScreen(TFT_BLACK); 
  tft.setCursor(0,4,4);
  // Mostra i valori nel Serial Monitor
  tft.print("Temperatura: ");
  tft.print(temperatura);
  tft.println(" °C");
  tft.print("Umidità: ");
  tft.print(umidita);
  tft.println(" %");

  // Aspetta 2 secondi prima della prossima lettura
  delay(2000);

}