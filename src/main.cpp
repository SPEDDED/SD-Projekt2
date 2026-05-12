#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include "Heap.h"
#include "DataGenerator.h"
#include "Array.h"

using namespace std;

// Rozbudowana struktura do przechowywania wyników obu struktur
struct TestResult {
    int size;
    double avgInsertHeap;
    double avgExtractHeap;
    double avgInsertArray;
    double avgExtractArray;
};

Node* loadDataFromFile(const string& filename, int& count) {
    ifstream inFile(filename);
    if (!inFile.is_open()) return nullptr;

    inFile>> count;
    Node* dataArray =new Node[count];
    for (int i= 0; i < count;++i) {
        inFile >> dataArray[i].priority >> dataArray[i].value;
    }


    inFile.close();
    return dataArray;
}

void runFullResearch() {
    // Rozmiary tablic do testów
    vector<int> sizes= {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000};
    const int iterations= 100; // liczba prób
    vector<TestResult> finalResults;


    cout << "Rozpoczynanie badan porownawczych\n";


    for (int size : sizes) {
        double tInsHeap = 0, tExtHeap = 0;
        double tInsArr = 0, tExtArr = 0;


        for (int i = 0; i < iterations; ++i) {
            string filename = "data/temp_test_data.txt";
            DataGenerator::generateRandomData(size, i + 100, filename);

            int count = 0;
            Node* testData = loadDataFromFile(filename, count);
            if (!testData) continue;



            // ---TESTY KOPIEC---

            MaxHeap heap;
            auto s1 = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) heap.insert(testData[j].value, testData[j].priority);
            auto e1 = chrono::high_resolution_clock::now();
            tInsHeap += chrono::duration_cast<chrono::nanoseconds>(e1 - s1).count();

            auto s2 = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) heap.extractMax();
            auto e2 = chrono::high_resolution_clock::now();
            tExtHeap += chrono::duration_cast<chrono::nanoseconds>(e2 - s2).count();


            // ---TESTY TABLICA---

            ArrayQueue arrayQ;
            auto s3 = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) arrayQ.insert(testData[j].value, testData[j].priority);
            auto e3 = chrono::high_resolution_clock::now();
            tInsArr += chrono::duration_cast<chrono::nanoseconds>(e3 - s3).count();

            auto s4 = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) arrayQ.extractMax();
            auto e4 = chrono::high_resolution_clock::now();
            tExtArr += chrono::duration_cast<chrono::nanoseconds>(e4 - s4).count();


            delete[] testData;
        }



        // uśrednione czasy w nanosekundach
        finalResults.push_back({
            size,
            (tInsHeap / iterations) / size,
            (tExtHeap / iterations) / size,
            (tInsArr / iterations) / size,
            (tExtArr / iterations) / size
        });

        cout << ">> Zakonczono pomiary dla n= " << size << "\n";
    }


    ofstream resFile("data/wyniki_porownawcze.csv");     // Zapis do  pliku CSV
    resFile << "n;Insert_Heap_ns;ExtractMax_Heap_ns;Insert_Array_ns;ExtractMax_Array_ns\n";
    for (const auto& r : finalResults) {
        resFile << r.size << ";" << r.avgInsertHeap << ";" << r.avgExtractHeap << ";" 
                << r.avgInsertArray << ";" << r.avgExtractArray << "\n";
    }
    resFile.close();


    cout << "\nBadania ukonczone! Wyniki znajdziesz w 'data/wyniki_porownawcze.csv'.\n";
}

int main() {
    int choice;
    do {
        cout << "\n--- PROJEKT SD: KOLEJKA PRIORYTETOWA ---\n";
        cout << "1. Uruchom badania\n";
        cout << "2. Test reczny Kopca\n";
        cout << "3. Test reczny Tablicy\n";
        cout << "0. Wyjscie\n";
        cout << "Wybor: ";
        cin >> choice;



        if (choice == 1) {
            runFullResearch();
        }

        else if (choice==2) {
            MaxHeap h;
            h.insert(100, 10); h.insert(200, 20); h.insert(50, 5);
            h.display();
            cout << "Wyciagnieto max (priorytet): " << h.extractMax().priority << "\n";
            h.display();
        } 

        else if (choice==3) {
            ArrayQueue a;
            a.insert(100, 10); a.insert(200, 20); a.insert(50, 5);
            a.display();
            cout << "Wyciagnieto max (priorytet): " << a.extractMax().priority << "\n";
            a.display();
        }

    } while (choice != 0);

    return 0;
}