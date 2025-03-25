// Codice per il monitoraggio da remoto della temperatura e umidità
#include "Adafruit_Sensor.h"
#include <DHT.h>

#include <WiFi.h>
#include <PubSubClient.h>

// Wokwi Virtual WiFi
const char* ssid = "myspot";
const char* password = "settete7";

const char* mqtt_server = "test.mosquitto.org";// MQTT Broker address
int mqtt_broker_port = 1883;
char* mqttTopic_DHT_temp = "esp32/iot1/dht/temp"; // MQTT topic2: sensor-temerature
char* mqttTopic_DHT_hum = "esp32/iot1/dht/hum";  // MQTT topic3: sensor-humidity
 
WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);
// Se si volessero introdurre certificati di SSL/TLS bisognerebbe
// creare qui gli oggetti a partire dal client(espClient)


//Constants
#define DHTPIN 22     // pin DATI sul GPIO
#define DHTTYPE DHT11   // Sensore DHT11 (AM2302)
//#define DHTTYPE DHT22   // DHT 11  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Inizializza il sensore

int LEDPIN = 2; //GPIO pin usato dal LED

void setup() {
  Serial.begin(115200); // Inizializzazione del monitor seriale (solo per il debugging!)
 
  pinMode(LEDPIN, OUTPUT);
 
  dht.begin();

  setup_wifi();
  pubSubClient.setServer(mqtt_server, mqtt_broker_port); // Server e porta MQTT del broker al quale puntare
  //pubSubClient.setCallback(callback); // Funzione di callback per i messaggi in ingresso
 
  delay(2000);
}

void setup_wifi() { // Connessione al WiFi

  WiFi.mode(WIFI_STA); // default, se non lo si dichiara non serve

  //delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Indirizzo IP associato all'ESP32 una volta connesso
}
// Process incoming MQTT message and control the servo motor
//void callback(char* topic, byte* payload, unsigned int length) {  
  //if (topic = mqttTopic_DHT_temp){
   
  //}
//}

void mqttReconnect() { // Riconnessione al broker MQTT
  while (!pubSubClient.connected()) {
   
    Serial.print("Attempting MQTT connection...");
   
    if (pubSubClient.connect("ESPClient")) {
      Serial.println("connected");
      pubSubClient.subscribe(mqttTopic_DHT_temp);
      pubSubClient.subscribe(mqttTopic_DHT_hum);

    } else {
      Serial.print("failed, rc=");
      Serial.print(pubSubClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

float temp;
float humidity;

void loop() {
 
  if (!pubSubClient.connected()) { // Verifica lo stato della connessione MQTT
    digitalWrite(LEDPIN, LOW);
    mqttReconnect();
  } else {
    digitalWrite(LEDPIN, HIGH);
  }

  pubSubClient.loop();
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
 
  Serial.print("Temp : ");
  Serial.print(temp);
  Serial.println(" C ");
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % ");

  String messaget = String(temp);
  String messageh = String(humidity);
 
  pubSubClient.publish(mqttTopic_DHT_temp, messaget.c_str());
  pubSubClient.publish(mqttTopic_DHT_hum, messageh.c_str());
 
  delay(200);
 
}