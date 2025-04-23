// Codice per il monitoraggio da remoto della temperatura e umidità
// con NodeRED su http://NODEREDCLIENT_IPADDRESS/8080/ui
// con webserver ESP_IPADDRESS
// e display SSD1306
#include <Arduino.h>
#include "Adafruit_Sensor.h"
#include <DHT.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <PubSubClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <ESPAsyncWebSrv.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "webpage.h"


// Wokwi Virtual WiFi
char* ssid = "myspot";
char* password = "settete";

const char* mqtt_server = "test.mosquitto.org";// MQTT Broker address
int mqtt_broker_port = 1883;
const char* mqttTopic_DHT_temp = "esp32/iot1/dht/temp";
const char* mqttTopic_DHT_hum  = "esp32/iot1/dht/hum";
 
WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);
// Se si volessero introdurre certificati di SSL/TLS bisognerebbe
// creare qui gli oggetti a partire dal client(espClient)


//Constants
#define DHTPIN 4     // pin DATI sul GPIO
#define DHTTYPE DHT11   // Sensore DHT11 (AM2302)
//#define DHTTYPE DHT22   // DHT 11  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Inizializza il sensore

float temp;      // valori sensore temperatura
float humidity;  // valori sensore umidità

int LEDPIN = 34; //GPIO pin usato dal LED


const int ledPin = 35; //Pin controllato dal WebServer
bool ledState    = 0; //Stato pin contrallato dal WebServer

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------




void notifyClients() {
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------


#define SCREEN_WIDTH 128 // OLED width, in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels

// ------------------- For i2c -------------------
//// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------


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








void setup() {
  Serial.begin(115200); // Inizializzazione del monitor seriale (solo per il debugging!)
  //myservo.attach(5); // Evetuale servo sul GPIO 5
 
  pinMode(LEDPIN, OUTPUT);
 
  dht.begin();

  setup_wifi();
  pubSubClient.setServer(mqtt_server, mqtt_broker_port); // Server e porta MQTT del broker al quale puntare
  //pubSubClient.setCallback(callback); // Funzione di callback per i messaggi in ingresso
 

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);


  initWebSocket();
  // Route root webpage : /
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.begin(); // Start Web Server
  delay(2000);



//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------


  while (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Initialize OLED display with address 0x3C
    Serial.println(F("Failed to start SSD1306 OLED"));
    delay(500);
  }

  //oled.width(), oled.height()
  //`  0,  0` sono le coordinate (X, Y) in cui vuoi posizionare l'immagine.
  //`128, 64` sono le dimensioni del logo.
  //`WHITE`   è il colore dei pixel accesi (con libreria come Adafruit GFX)
  // Show initial display buffer contents on the screen --
  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(WHITE);    // Set text color

  oled.clearDisplay(); // Clear display
  oled.setCursor(5, 32); // Set position to display (x,y)
  oled.println("Avvio OLED ...");
  oled.display(); // Display on OLED
  delay(2000); // Wait 2 seconds for loading screen

  oled.clearDisplay(); // Clear display
  oled.drawBitmap(  9,   7,        tcpConn1Inv, 50,  50, WHITE);
  oled.drawBitmap( 68,   7, antennaSending1Inv, 50,  50, WHITE);
  oled.display(); // Display on OLED
  delay(4000); // Wait 2 seconds for loading screen



  oled.display();
  delay(500);



}





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






void loop() {
 
  if (!pubSubClient.connected()) { // Verifica lo stato della connessione MQTT
    digitalWrite(LEDPIN, LOW);
    mqttReconnect();
  } else {
    digitalWrite(LEDPIN, HIGH);
  }

  pubSubClient.loop();
  delay(100); // ritardo per gestire il canale loop
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
 
  Serial.print("Temp : ");
  Serial.print(temp);
  Serial.println(" C ");
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" % ");

  String messaget = String(temp);     // Formato di scambio stringhe  con pubsubclient
  String messageh = String(humidity); // Formato di scambio stringhe  con pubsubclient
  pubSubClient.publish(mqttTopic_DHT_temp, messaget.c_str());
  pubSubClient.publish(mqttTopic_DHT_hum, messageh.c_str());
 
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);

  delay(200);
 

}
