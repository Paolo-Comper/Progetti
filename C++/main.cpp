#include <iostream>

int main() {
    int num1, num2, sum;

    // Chiedi all'utente di inserire due numeri
    std::cout << "Inserisci il primo numero: ";
    std::cin >> num1;
    std::cout << "Inserisci il secondo numero: ";
    std::cin >> num2;

    // Calcola la somma dei due numeri
    sum = num1 + num2;

    // Stampa il risultato
    std::cout << "La somma dei due numeri è: " << sum << std::endl;

    return 0;
}