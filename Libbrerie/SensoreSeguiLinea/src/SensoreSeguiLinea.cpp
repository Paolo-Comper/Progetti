#include "SensoreSeguiLinea.h"
#include <Arduino.h>
#include <Wire.h>

SensoreSeguiLinea::SensoreSeguiLinea(uint8_t adress) {
    _adress = adress;
}

void SensoreSeguiLinea::begin() {
    Wire.begin();
}

int SensoreSeguiLinea::read(uint8_t reg) {
    Wire.beginTransmission(_adress);
    Wire.write(reg);
    uint8_t err = Wire.endTransmission();
    
    Wire.requestFrom(_adress, 1);

    if (Wire.available()) {
        return Wire.read();
    }

    return 0;
}

