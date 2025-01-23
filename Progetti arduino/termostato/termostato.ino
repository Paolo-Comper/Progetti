#include <DHT.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <string.h>

// Cose del display
TFT_eSPI tft = TFT_eSPI();  // Inizializza l'oggetto TFT

// Pin del sensore e tipo di sensore
#define DHTPIN 22
#define DHTTYPE DHT22
#define RELE_PIN 26  // Pin del relè

// Inizializza il sensore e il file SD
DHT dht(DHTPIN, DHTTYPE);


float temperatura;
float umidita;
float tempRefON_1 = 20.5;  //tempo di riferimento per l'accenzione
float tempRefOFF_1 = 21; //tempo di riferimento per lo spegnimento

// Stato del relè (per evitare accensioni/spegnimenti ripetuti)
bool releStato = false;

void setup() {
  // Inizializzazione Serial Monitor
  Serial.begin(115200);
  Serial.println("Inizializzazione...");

  // Inizializza il sensore DHT
  dht.begin();

  // Inizializza il display TFT
  tft.init();
  tft.setRotation(2);  // Ruota il display se necessario
  tft.fillScreen(TFT_BLACK);  // Pulizia dello schermo

  // Inizializza il relè
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW);  // Spegne il relè all'avvio

  // Mostra una schermata iniziale
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.println("Sistema di Monitoraggio");
  tft.setCursor(60, 80);
  tft.println("Temperatura & Umidita'");
  delay(2000);
}

void loop() {
  // Legge i valori dal sensore
  temperatura = dht.readTemperature();
  umidita = dht.readHumidity();

  // Cancella lo schermo per l'aggiornamento
  tft.fillScreen(TFT_BLACK);

  // Disegna un layout più accattivante
  tft.fillRect(0, 0, 240, 30, TFT_BLUE);  // Barra superiore
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.println("Monitoraggio");

  tft.drawRect(10, 40, 220, 100, TFT_WHITE);  // Cornice dati

  // Mostra temperatura
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 50);
  tft.print("Temp:     ");
  tft.print(temperatura, 1);  // Una cifra decimale
  tft.println(" C");

  // Mostra umidità
  tft.setCursor(20, 80);
  tft.setTextColor(TFT_CYAN);
  tft.print("Umidita': ");
  tft.print(umidita, 1);
  tft.println(" %");

  // Stato del relè
  tft.setCursor(20, 110);
  if (releStato) {
    tft.setTextColor(TFT_GREEN);
    tft.println("Rele':    ATTIVO");
  } else {
    tft.setTextColor(TFT_RED);
    tft.println("Rele':    SPENTO");
  }

  // Controlla il relè in base alla temperatura
  if (temperatura > 21.0 && !releStato) {
    digitalWrite(RELE_PIN, HIGH);  // Attiva il relè
    releStato = true;
  } else if (temperatura < 20.5 && releStato) {
    digitalWrite(RELE_PIN, LOW);  // Disattiva il relè
    releStato = false;
  }

  // Aspetta 2 secondi prima della prossima lettura
  delay(2000);
}
