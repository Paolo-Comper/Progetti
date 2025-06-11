// Costanti dei pin
constexpr uint8_t AUTO_VERDE_PIN     = 4;
constexpr uint8_t AUTO_GIALLO_PIN    = 2;
constexpr uint8_t AUTO_ROSSO_PIN     = 15;

constexpr uint8_t PEDONE_VERDE_PIN   = 5;
constexpr uint8_t PEDONE_ROSSO_PIN   = 17;

constexpr uint8_t PULSANTE_PIN       = 18;

enum Stato {
  VERDE  = 0b001,
  GIALLO = 0b010,
  ROSSO  = 0b100
};

enum Tipo {
  STRADALE,
  PEDONALE
};

enum StatoIncrocio {
  AUTO_VERDE_PEDONI_ROSSO,
  AUTO_GIALLO_PEDONI_ROSSO,
  AUTO_ROSSO_PEDONI_VERDE
};

class Semaforo {
  private:
    uint8_t led_verde, led_giallo, led_rosso;
    const Tipo tipo_semaforo;

  public:
    // Costruttore per semafori stradali (3 luci)
    Semaforo(Tipo tipo, uint8_t pin_verde, uint8_t pin_giallo, uint8_t pin_rosso)
        : led_verde(pin_verde), led_giallo(pin_giallo), led_rosso(pin_rosso), tipo_semaforo(tipo) {
      pinMode(led_verde, OUTPUT);
      pinMode(led_giallo, OUTPUT);
      pinMode(led_rosso, OUTPUT);
    }

    // Costruttore per semafori pedonali (solo verde e rosso)
    Semaforo(Tipo tipo, uint8_t pin_verde, uint8_t pin_rosso)
        : led_verde(pin_verde), led_rosso(pin_rosso), tipo_semaforo(tipo) {
      pinMode(led_verde, OUTPUT);
      pinMode(led_rosso, OUTPUT);
    }

    void Stato(Stato stato) {
      // Usa operatori tra bit per verificare se accendere i singoli LED
      // Esempio: se stato = 0b010, allora (stato & 0b010) = true -> accendi giallo
      // Permette di poter aggiungere uno stato dove sono accese 2 luci contemporasneamente 
      // Esempio: se stato fosse 0b011 accenderebbe il giallo e il verde
      digitalWrite(led_verde, stato & 0b001);
      if (tipo_semaforo == STRADALE)
        digitalWrite(led_giallo, stato & 0b010);
      digitalWrite(led_rosso, stato & 0b100);
    }
};

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

// Istanziazione semafori e pulsante
Semaforo semaforo_auto(STRADALE, AUTO_VERDE_PIN, AUTO_GIALLO_PIN, AUTO_ROSSO_PIN);
Semaforo semaforo_pedonale(PEDONALE, PEDONE_VERDE_PIN, PEDONE_ROSSO_PIN);
Pulsante pulsante(PULSANTE_PIN);

// Stato iniziale dell'incrocio
StatoIncrocio stato_incrocio = AUTO_VERDE_PEDONI_ROSSO;

unsigned long tempo;

void setup() {
  tempo = millis();
}

void loop() {
  switch (stato_incrocio) {
    case AUTO_VERDE_PEDONI_ROSSO:
      semaforo_auto.Stato(VERDE);
      semaforo_pedonale.Stato(ROSSO);

      if (pulsante.checkOnPress()) {
        stato_incrocio = AUTO_GIALLO_PEDONI_ROSSO;
        tempo = millis();
      }
      break;

    case AUTO_GIALLO_PEDONI_ROSSO:
      semaforo_auto.Stato(GIALLO);
      semaforo_pedonale.Stato(ROSSO);

      if ((millis() - tempo) > 3'000) {
        stato_incrocio = AUTO_ROSSO_PEDONI_VERDE;
        tempo = millis();
      }
      break;

    case AUTO_ROSSO_PEDONI_VERDE:
      semaforo_auto.Stato(ROSSO);
      semaforo_pedonale.Stato(VERDE);

      if ((millis() - tempo) > 10'000) {
        stato_incrocio = AUTO_VERDE_PEDONI_ROSSO;
        tempo = millis();
      }
      break;
  }
}
