#include <SensoreSeguiLinea.h>

SensoreSeguiLinea sensore(0x20);

void setup() {
  Serial.begin(9600);
  sensore.begin();
}

void loop() {

  Serial.print("Valori: ");
  Serial.print(sensore.read(1));
  Serial.print(" : ");
  Serial.print(sensore.read(2));
  Serial.print(" : ");
  Serial.print(sensore.read(3));
  Serial.print(" : ");
  Serial.print(sensore.read(4));
  Serial.print("\n");
  delay(500);
}
