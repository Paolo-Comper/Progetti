import cv2
import numpy as np
import serial

# Creazione di un'immagine nera
background = np.zeros((160, 120, 3), np.uint8) 

# Porta seriale e baudrate
porta_seriale = '/dev/ttyUSB0'  # Cambia con la tua porta corretta
baudrate = 115200

try:
    ser = serial.Serial(porta_seriale, baudrate, timeout=1)
    print(f"Connesso a {porta_seriale}")

    while True:
        background = np.zeros((120, 160, 3), np.uint8)

        if ser.in_waiting > 0:  # Se ci sono dati disponibili
            riga = ser.readline().decode('utf-8').strip()
            
            # Controlla se la riga contiene una virgola
            if ',' in riga:
                try:
                    coordinate = riga.split(",")
                    x, y = int(coordinate[0]), int(coordinate[1])

                    # Disegna il punto solo se le coordinate sono valide
                    if 0 <= x < 160 and 0 <= y < 120:
                        cv2.circle(background, (x, y), 2, (0, 255, 0), -1)

                    print(f"Dati ricevuti: {x}, {y}")

                except ValueError:
                    print(f"Errore di conversione: {riga}")

        # RIDIMENSIONA l'immagine correttamente
        background_resized = cv2.resize(background, (640, 480), interpolation=cv2.INTER_NEAREST)
        
        # Mostra l'immagine ridimensionata
        cv2.imshow('img', background_resized)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except serial.SerialException as e:
    print(f"Errore nella connessione seriale: {e}")
except KeyboardInterrupt:
    print("Interruzione manuale")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Porta seriale chiusa")
    cv2.destroyAllWindows()
