#pragma once

#include <Arduino.h>

class SensoreSeguiLinea {
    public:
        SensoreSeguiLinea(uint8_t adress = 0x20);
        void begin();
        uint8_t read(uint16_t reg);
    private:
        uint8_t _adress;
};

