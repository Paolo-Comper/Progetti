#pragma once

#include <Arduino.h>

class SensoreSeguiLinea {
    public:
        SensoreSeguiLinea(uint8_t adress = 0x20);
        void begin();
        int read(uint8_t reg);
    private:
        uint8_t _adress;
};

