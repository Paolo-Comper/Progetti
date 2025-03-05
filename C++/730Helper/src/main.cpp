#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib> // Per exit()

// Funzione per scattare la foto
void scattaFoto() {
    cv::VideoCapture cap(0, cv::CAP_V4L);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

    if (!cap.isOpened()) {
        std::cerr << "Errore: Fotocamera non rilevata!" << std::endl;
        exit(-1);
    }

    cv::Mat temp_frame;
    char key;
    while (true) {
        cap >> temp_frame;
        if (temp_frame.empty()) break;

        cv::imshow("Camera", temp_frame);

        key = cv::waitKey(1);
        if (key == 27) { // ESC
            std::cout << "Uscita..." << std::endl;
            exit(0);
        } else if (key == 's' || key == 'S') { // Salva immagine
            std::cout << "Immagine salvata" << std::endl;
            cv::imwrite("scontrino.jpg", temp_frame);
            cv::destroyAllWindows();
            return;
        }
    }
}

// Funzione per trovare i 4 angoli dello scontrino
std::vector<cv::Point> angoliScontrino() {
    // Carica l'immagine
    cv::Mat img = cv::imread("scontrino.jpg", cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "Errore: Immagine non trovata!" << std::endl;
        return {};
    }

    // Converti l'immagine in scala di grigi
    cv::Mat gray, edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Applica il Canny edge detector
    cv::Canny(gray, edges, 100, 200);
    cv::imshow("Bordi", edges);
    // Trova i contorni nei bordi rilevati
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> best_contour;
    double max_area = 0;

    // Cerca il contorno con la maggiore area e con 4 vertici
    for (const auto& c : contours) {
        double area = cv::contourArea(c);
        if (area > max_area) {
            std::vector<cv::Point> approx;
            // Approssima il contorno a un poligono
            cv::approxPolyDP(c, approx, 0.02 * cv::arcLength(c, true), true);
            if (approx.size() == 4) {
                best_contour = approx;
                max_area = area;
            }
        }
    }

    return best_contour;
}


std::vector<cv::Point> ordinaPunti(const std::vector<cv::Point>& punti) {
    std::vector<cv::Point> ordinati;
    cv::Point2f centro(0, 0);

    for (const auto& p : punti) {
        centro += cv::Point2f(p);
    }
    centro *= 1.0 / punti.size();

    std::vector<cv::Point> punti_sx, punti_dx;
    for (const auto& p : punti) {
        if (p.x < centro.x) {
            punti_sx.push_back(p);
        } else {
            punti_dx.push_back(p);
        }
    }

    std::sort(punti_sx.begin(), punti_sx.end(), [](cv::Point a, cv::Point b) { return a.y < b.y; });
    std::sort(punti_dx.begin(), punti_dx.end(), [](cv::Point a, cv::Point b) { return a.y < b.y; });

    ordinati.push_back(punti_sx[0]); // alto-sx
    ordinati.push_back(punti_dx[0]); // alto-dx
    ordinati.push_back(punti_sx[1]); // basso-sx
    ordinati.push_back(punti_dx[1]); // basso-dx

    return ordinati;
}

cv::Mat trasformaScontrino(const cv::Mat& img, const std::vector<cv::Point>& punti) {
    if (punti.size() != 4) {
        std::cerr << "Errore: servono esattamente 4 punti!" << std::endl;
        return img;
    }

    // Ordina i punti (alto-sx, alto-dx, basso-sx, basso-dx)
    std::vector<cv::Point2f> srcPts = { punti[0], punti[1], punti[2], punti[3] };

    // Dimensioni dell'output (modificate per aumentare la larghezza)
    float larghezza1 = cv::norm(srcPts[0] - srcPts[1]);
    float larghezza2 = cv::norm(srcPts[2] - srcPts[3]);
    float larghezza = std::max(larghezza1, larghezza2);

    float altezza1 = cv::norm(srcPts[0] - srcPts[2]);
    float altezza2 = cv::norm(srcPts[1] - srcPts[3]);
    float altezza = std::max(altezza1, altezza2) * 1.3;

    std::vector<cv::Point2f> dstPts = {
        {0, 0}, {larghezza, 0}, {0, altezza}, {larghezza, altezza}
    };

    // Calcola la matrice di trasformazione omografica
    cv::Mat M = cv::getPerspectiveTransform(srcPts, dstPts);

    // Applica la trasformazione
    cv::Mat imgWarped;
    cv::warpPerspective(img, imgWarped, M, cv::Size(larghezza, altezza));

    // Ruota l'immagine di 90° a sinistra
    cv::rotate(imgWarped, imgWarped, cv::ROTATE_90_COUNTERCLOCKWISE);

    return imgWarped;
}

int main() {
    char key;
    
    Ricomincia: // Etichetta per ripetere il processo

    // Acquisisci una foto
    scattaFoto();

    // Leggi l'immagine salvata
    cv::Mat img = cv::imread("scontrino.jpg", cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "Errore: Immagine non trovata!" << std::endl;
        return -1;
    }

    std::vector<cv::Point> best_contour = angoliScontrino();

    std::vector<cv::Point> ordinati = ordinaPunti(best_contour);
    cv::Mat trasformato = trasformaScontrino(img, ordinati);
    cv::Mat rimpicciolito;
    cv::resize(trasformato, rimpicciolito, cv::Size(), 0.5, 0.5);
    // Mostra l'immagine con il contorno
    cv::imshow("Scontrino con contorno", rimpicciolito);

    // Gestisci l'input dell'utente
    while (true) {
        key = cv::waitKey(0);
        if (key == 27) { // ESC per uscire
            std::cout << "Uscita..." << std::endl;
            break;
        } else if (key == 'c' || key == 'C') { // 'C' per ricominciare
            std::cout << "Ricomincia..." << std::endl;
            cv::destroyAllWindows();
            goto Ricomincia;
        }
    }

    return 0;
}
