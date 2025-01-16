#include <DHT.h>
#include <SPI.h>
#include <SD.h>

// Pin del sensore e tipo di sensore
#define DHTPIN 4      // Pin collegato al sensore
#define DHTTYPE DHT22 // AM2302 è compatibile con DHT22

// Pin per il modulo SD
#define SD_CS 35 // Chip Select del modulo SD

// Inizializza il sensore e il file SD
DHT dht(DHTPIN, DHTTYPE);
File dataFile;

void setup() {
  // Inizializzazione Serial Monitor
  Serial.begin(115200);
  Serial.println("Inizializzazione...");

  // Inizializza il sensore DHT
  dht.begin();

  // Inizializza la SD
  if (!SD.begin(SD_CS)) {
    Serial.println("Errore nell'inizializzazione della SD!");
    while (true); // Blocco il programma
  }
  Serial.println("Scheda SD pronta.");

  // Crea o apre il file per salvare i dati
  dataFile = SD.open("dati.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Temperatura (°C),Umidità (%)");
    dataFile.close();
  } else {
    Serial.println("Errore nell'apertura del file.");
    while (true); // Blocco il programma
  }
}

void loop() {
  // Legge i valori dal sensore
  float temperatura = dht.readTemperature();
  float umidita = dht.readHumidity();

  // Controlla errori nella lettura
  if (isnan(temperatura) || isnan(umidita)) {
    Serial.println("Errore nella lettura dal sensore.");
    return;
  }

  // Mostra i valori nel Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C, Umidità: ");
  Serial.print(umidita);
  Serial.println(" %");

  // Salva i valori nella scheda SD
  dataFile = SD.open("dati.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(temperatura);
    dataFile.print(",");
    dataFile.println(umidita);
    dataFile.close();
    Serial.println("Dati salvati.");
  } else {
    Serial.println("Errore nell'apertura del file.");
  }

  // Aspetta 2 secondi prima della prossima lettura
  delay(2000);
}
