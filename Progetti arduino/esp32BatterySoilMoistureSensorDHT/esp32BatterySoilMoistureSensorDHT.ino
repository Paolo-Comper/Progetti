// Include DHT11 library
#include "DHT.h"
#define DHTPIN 22
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin per la lettura dell'umidità del terreno
const int soilMoisturePin = 32;

// Variabile globale per la lettura smussata dell'umidità del terreno
float smoothedSoilMoisture = 0.0;

void setup() {
  // Impostazione pin e avvio seriale
  pinMode(soilMoisturePin, INPUT);
  Serial.begin(115200);

  // Avvio del sensore DHT
  dht.begin();
  Serial.println("Sensori inizializzati.");
}

void loop() {
  // Lettura e smussamento dei dati sull'umidità del terreno
  smoothedSoilMoisture = 0.95 * smoothedSoilMoisture + 0.05 * analogRead(soilMoisturePin);

  // Lettura dei dati dal DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Controllo errori nella lettura
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Errore nella lettura del sensore DHT11!");
  } else {
    // Stampa dei dati su seriale
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Umidità aria: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  Serial.print("Umidità del terreno: ");
  Serial.println(smoothedSoilMoisture);

  // Attesa prima della prossima lettura
  delay(2000);
}
