import cv2
import pytesseract

# Carica l'immagine
img = cv2.imread("scontrino.jpg")

# Converti in scala di grigi
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Applica una soglia adattiva per migliorare il contrasto
thresh = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, 
                               cv2.THRESH_BINARY, 11, 2)

# Usa Tesseract per riconoscere il testo
text = pytesseract.image_to_string(thresh, lang="ita")

print("Testo estratto:")
print(text)
