import cv2
import numpy as np

def scatta_foto():
    cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)
    
    if not cap.isOpened():
        print("Errore: Fotocamera non rilevata!")
        return None
    
    while True:
        ret, frame = cap.read()
        if not ret:
            continue
        
        cv2.imshow("Camera", frame)
        key = cv2.waitKey(1)
        
        if key == 27:  # ESC per uscire
            cap.release()
            cv2.destroyAllWindows()
            return None
        elif key in [ord('s'), ord('S')]:  # 'S' per scattare la foto
            cap.release()
            cv2.destroyAllWindows()
            return frame

def trova_angoli(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(gray, 100, 200)
    
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    best_contour = []
    max_area = 0
    
    for c in contours:
        area = cv2.contourArea(c)
        approx = cv2.approxPolyDP(c, 0.02 * cv2.arcLength(c, True), True)
        
        if len(approx) == 4 and area > max_area:
            best_contour = approx.reshape(4, 2)
            max_area = area
    
    return best_contour

def ordina_punti(punti):
    punti = sorted(punti, key=lambda p: p[0] + p[1])
    alto_sx, basso_dx = punti[0], punti[3]
    alto_dx, basso_sx = punti[1], punti[2]
    
    if alto_dx[1] > basso_sx[1]:
        alto_dx, basso_sx = basso_sx, alto_dx
    
    return np.array([alto_sx, alto_dx, basso_sx, basso_dx], dtype=np.float32)

def correggi_prospettiva(img, punti):
    if len(punti) != 4:
        print("Errore: servono esattamente 4 punti!")
        return img
    
    punti = ordina_punti(punti)
    larghezza = max(np.linalg.norm(punti[0] - punti[1]), np.linalg.norm(punti[2] - punti[3]))
    altezza = max(np.linalg.norm(punti[0] - punti[2]), np.linalg.norm(punti[1] - punti[3])) * 1.3
    
    dst_pts = np.array([[0, 0], [larghezza, 0], [0, altezza], [larghezza, altezza]], dtype=np.float32)
    M = cv2.getPerspectiveTransform(punti, dst_pts)
    img_warped = cv2.warpPerspective(img, M, (int(larghezza), int(altezza)))
    return cv2.rotate(img_warped, cv2.ROTATE_90_CLOCKWISE)

def migliora_legibilità(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (7, 7), 0)
    return cv2.adaptiveThreshold(blur, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

def main():
    while True:
        img = scatta_foto()
        if img is None:
            break
        
        angoli = trova_angoli(img)
        if len(angoli) != 4:
            print("Errore: impossibile trovare lo scontrino.")
            continue
        
        trasformato = correggi_prospettiva(img, angoli)
        leggibile = migliora_legibilità(trasformato)
        
        cv2.imshow("Scontrino Processato", cv2.resize(leggibile, None, fx=0.5, fy=0.5))
        
        key = cv2.waitKey(0)
        if key == 27:  # ESC per uscire
            break
        elif key in [ord('c'), ord('C')]:  # 'C' per ripetere
            cv2.destroyAllWindows()
            cv2.imwrite("scontrino.jpg", trasformato)
            continue
    
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()