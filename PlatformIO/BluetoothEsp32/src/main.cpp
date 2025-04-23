#include <Arduino.h>
#include <BluetoothSerial.h>
#include <string>

String data;
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("GG per ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  if(SerialBT.available()){
    data = SerialBT.readString();
    data.trim();
    Serial.println(data);
    SerialBT.println(data);
  }
  
  if (data == "ON"){
    Serial.println("LED ON");
    digitalWrite(2, HIGH);
    delay(100);
  }
}
