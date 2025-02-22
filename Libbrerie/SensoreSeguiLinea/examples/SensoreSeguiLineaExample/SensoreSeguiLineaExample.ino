#include <SensoreSeguiLinea.h>

SensoreSeguiLinea sensoreSeguiLinea(0x20);

void setup() {
  sensoreSeguiLinea.begin();
}

void loop() {
  uint8_t value = sensoreSeguiLinea.read(1);
  Serial.println(value);
  delay(1000);
}

