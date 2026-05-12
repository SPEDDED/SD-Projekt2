#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include "Heap.h"
#include "DataGenerator.h"
#include "Array.h"

using namespace std;

// struktura do przechowywania uśrednionych wyników
struct TestResult {
    int size;
    double avgInsertTime;
    double avgExtractMaxTime;
};

// Wczytanie danych do tablicy przed pmiarem
Node* loadDataFromFile(const string& filename, int& count) {
    ifstream inFile(filename);
    if (!inFile.is_open()) return nullptr;

    inFile >> count;
    Node* dataArray = new Node[count];
    for (int i = 0; i < count; ++i) {
        inFile >> dataArray[i].priority >> dataArray[i].value;
    }
    inFile.close();
    return dataArray;
}

void runFullResearch() {
    // 8 punktów pomiarowych
    vector<int> sizes = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000};
    const int iterations = 100; // Minimum 100 powtórzeń dla przypadku średniego 
    vector<TestResult> finalResults;

    cout << "Rozpoczynanie badan... Moze to potrwac kilka minut.\n";

    for (int size : sizes) {
        double totalInsertTime = 0;
        double totalExtractTime = 0;

        for (int i = 0; i < iterations; ++i) {
            string filename = "data/temp_test_data.txt";
            // Generowanie nowej populacji danych z innym ziarnem dla kazdej proby
            DataGenerator::generateRandomData(size, i + 100, filename);

            int count = 0;
            Node* testData = loadDataFromFile(filename, count);
            if (!testData) continue;

            MaxHeap heap;

            // --- Pomiar operacji INSERT ---
            auto startIn = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) {
                heap.insert(testData[j].value, testData[j].priority);
            }
            auto endIn = chrono::high_resolution_clock::now();
            totalInsertTime += chrono::duration_cast<chrono::nanoseconds>(endIn - startIn).count();

            // --- Pomiar operacji EXTRACT-MAX ---
            auto startEx = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) {
                heap.extractMax();
            }
            auto endEx = chrono::high_resolution_clock::now();
            totalExtractTime += chrono::duration_cast<chrono::nanoseconds>(endEx - startEx).count();

            delete[] testData;
        }

        // Obliczanie sredniego czasu na pojedyncza operacje w nanosekundach
        finalResults.push_back({
            size,
            (totalInsertTime / iterations) / size,
            (totalExtractTime / iterations) / size
        });

        cout << "Zakonczono pomiary dla rozmiaru: " << size << "\n";
    }

    // Zapisywanie wynikow w folderze data 
    ofstream resFile("data/wyniki_kopiec.csv");
    resFile << "Rozmiar_n;Sredni_Insert_ns;Sredni_ExtractMax_ns\n";
    for (const auto& res : finalResults) {
        resFile << res.size << ";" << res.avgInsertTime << ";" << res.avgExtractMaxTime << "\n";
    }
    resFile.close();

    cout << "\nBadania ukonczone! Wyniki zapisano w 'data/wyniki_kopiec.csv'.\n";
}

int main() {

    int choice;
    do {
        cout << "\n--- MENU PROJEKTOWE ---\n";
        cout << "1. Uruchom pelna procedure badawcza (8 rozmiarow x 10 prob)\n";
        cout << "2. Testuj recznie Kopiec (dodaj/usun/wyswietl)\n";
        cout << "0. Wyjscie\n";
        cout << "Wybor: ";
        cin >> choice;

        if (choice == 1) {
            runFullResearch();
        } else if (choice == 2) {
            // Menu do testów ręcznych
            MaxHeap h;
            h.insert(10, 5); h.insert(20, 15); h.insert(5, 1);
            h.display();
            cout << "Extract Max: " << h.extractMax().priority << "\n";
            h.display();
        }
    } while (choice != 0);

    return 0;
}
