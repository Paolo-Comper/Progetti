
constexpr int PIN_PULSANTE = 4;

class Pulsante {
  private:
    bool old_stato;
    bool stato;
    const uint8_t pin;
    unsigned long lastDebounceTime = 0;
    static constexpr unsigned long debounceDelay = 50; // 50 ms di debounce

    bool leggiStatoGrezzo() { return !digitalRead(pin); } // PULLUP: premuto = LOW → ritorna true

  public:
    Pulsante(unsigned int pin_pulsante) : pin(pin_pulsante) {
      pinMode(pin, INPUT_PULLUP);
      old_stato = leggiStatoGrezzo();
      stato = old_stato;
    }

    // Restituisce true una sola volta alla pressione, con debounce
    bool checkOnPress() {
      bool lettura = leggiStatoGrezzo();

      if (lettura != stato) {
        lastDebounceTime = millis(); // reset debounce
      }

      if ((millis() - lastDebounceTime) > debounceDelay) {
        if (lettura != old_stato) {
          old_stato = lettura;
          if (old_stato) return true; // stato appena passato a premuto
        }
      }

      stato = lettura;
      return false;
    }
};

unsigned long int count = 0;
float flusso = 0; // m3/s
unsigned long int old_count = 0;
unsigned long int tempo;
unsigned long int diff_tempo = 0;

Pulsante pulsante(PIN_PULSANTE);

void setup() {
  tempo = millis();
  Serial.begin(115200);
}

void loop() {
  
  if (pulsante.checkOnPress()){
    count ++;
  }

  diff_tempo = millis() - tempo;
  if (diff_tempo > 5000) {

    Serial.printf("count = %d \n", count);

    Serial.printf("dif tempo = %f \n", diff_tempo / 1000.0);

    flusso = count / (diff_tempo / 1000.0);
    Serial.printf("flusso = %f \n \n", flusso);

    tempo = millis();
    count = 0;
  }
  

}
