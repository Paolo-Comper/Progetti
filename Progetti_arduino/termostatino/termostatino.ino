#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "data.h"

// Definizioni dei pin
#define DHTPIN 4        // Pin collegato al sensore DHT
#define DHTTYPE DHT22    // Tipo di sensore DHT
#define LED_PIN 34       // Pin collegato al LED
#define SCREEN_WIDTH 128 // Larghezza display OLED
#define SCREEN_HEIGHT 64 // Altezza display OLED

// Inizializza il sensore DHT e il display OLED
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT);

// Variabili globali
float tempRef = 18.0;
float tempoRefON_1 = 18.0;
float tempoRefOFF_1 = 19.0;
unsigned long tempo;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  dht.begin();


  // Inizializzazione OLED
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Errore nell'avvio dell'OLED"));
    while (true);
  }
  oled.clearDisplay();

  oled.display();

  oled.clearDisplay();

  oled.drawBitmap(5, 3, image_weather_humidity_bits, 11, 16, 1);

  oled.drawBitmap(3, 45, image_crosshairs_bits, 15, 16, 1);

  oled.drawBitmap(6, 24, image_weather_temperature_bits, 16, 16, 1);
  oled.display();

}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  tempo = millis() / 1000;

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Errore nella lettura del sensore DHT!");
    return;
  }

  Serial.print("Temperatura: "); Serial.print(temperature);
  Serial.print(" °C | Umidità: "); Serial.print(humidity);
  Serial.print("% | Tempo: "); Serial.println(tempo);

  if (temperature > tempRef && tempo >= tempoRefON_1 && tempo <= tempoRefOFF_1) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED acceso!");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED spento!");
  }

  oled.setTextColor(1);
  oled.setTextSize(2);
  oled.setTextWrap(false);
  oled.setCursor(25, 26);
  oled.print(temperature, 1);

  oled.setCursor(25, 5);
  oled.print(humidity, 0); oled.print(" %");

  oled.setCursor(25, 46);
  oled.print(tempRef, 1);

  oled.drawRect(75, 21, 50, 38, 1);

  oled.drawBitmap(93, 3, image_clock_alarm_bits, 15, 16, 1);

  oled.setTextSize(1);
  oled.setCursor(86, 32);
  oled.print(tempoRefON_1);

  oled.setCursor(83, 24);
  oled.print("Inizio");

  oled.setCursor(89, 41);
  oled.print("Fine");

  oled.setCursor(86, 50);
  oled.print(tempoRefOFF_1);

  oled.display();

  delay(1000);
}