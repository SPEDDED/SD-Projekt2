#include "DataGenerator.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

using namespace std;

void DataGenerator::generateRandomData(int count, int seed, const string& filename) {
    // Użycie ziarna do zapewnienia powtarzalności badań
    srand(seed);

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Blad: Nie mozna otworzyc pliku do zapisu: " << filename << endl;
        return;
    }

    // zakres priorytetów kilkukrotnie większy niż wielkość struktury
    int priorityRange = count * 5;


    outFile << count << "\n";    // Pierwsza linia to liczba elementów

    for (int i = 0; i < count; ++i) {
        // Losowanie klucza oraz wartości (4-bajt int)
        int priority = rand() % priorityRange;
        int value = rand();
        
        outFile << priority << " " << value << "\n";
    }

    outFile.close();
    cout << "Utworzono plik: " << filename << " (" << count << " par)" << endl;
}
