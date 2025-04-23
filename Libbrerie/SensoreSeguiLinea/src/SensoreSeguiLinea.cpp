#include "SensoreSeguiLinea.h"
#include <Arduino.h>
#include <Wire.h>

SensoreSeguiLinea::SensoreSeguiLinea(uint8_t adress) {
    _adress = (uint8_t)adress; 
}

void SensoreSeguiLinea::begin() {
    Wire.begin();
}

uint8_t SensoreSeguiLinea::read(uint16_t reg) {
    Wire.beginTransmission(_adress);
    Wire.write(highByte(reg));  //Il byte alto
    Wire.write(lowByte(reg));   //Il byte basso
    uint8_t err = Wire.endTransmission();

    Wire.requestFrom(_adress, (uint8_t)1);

    if (Wire.available()) {
        return Wire.read();
    }

    return -1;  //restituisce -1 in caso di errore
}

