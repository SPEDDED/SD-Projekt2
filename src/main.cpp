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

    // 8 punktów pomiarowych
    std::vector<int> sizes = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000};
    const int iterations = 30; // Liczba prób
    const int fastOpRepeats = 1000; // Powtórzenia 
    


    struct FullResult {
        int n;
        double hIns, hExt, hPeek, hMod, hSize;
        double aIns, aExt, aPeek, aMod, aSize;
    };
    std::vector<FullResult> finalResults;



    std::cout << "Rozpoczynanie pelnych badan (5 operacji, 2 struktury)... To moze potrwac.\n";

    for (int size : sizes) {
        // Zmienne do sumowania czasów (w nanosekundach)
        double sumHIns = 0, sumHExt = 0, sumHPeek = 0, sumHMod = 0, sumHSize = 0;
        double sumAIns = 0, sumAExt = 0, sumAPeek = 0, sumAMod = 0, sumASize = 0;

        for (int i = 0; i < iterations; ++i) {
            std::string filename = "data/temp_test_data.txt";
            DataGenerator::generateRandomData(size, i + 100, filename); // Losowe ziarno


            int count = 0;
            Node* testData = loadDataFromFile(filename, count);
            if (!testData) continue;



            // --- TESTY KOPIEC ---
            MaxHeap heap;


            // 1. Insert
            auto s = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) heap.insert(testData[j].value, testData[j].priority);
            auto e = std::chrono::high_resolution_clock::now();
            sumHIns += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();

            // 2. Peek
            s = std::chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) heap.findMax();
            e = std::chrono::high_resolution_clock::now();
            sumHPeek += (std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);

            // 3. Return-size
            s = std::chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) heap.returnSize();
            e = std::chrono::high_resolution_clock::now();
            sumHSize += (std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);


            // 4. Modify-key
            int valToMod = testData[size/2].value;
            s = std::chrono::high_resolution_clock::now();
            heap.modifyKey(valToMod, 999999);
            e = std::chrono::high_resolution_clock::now();
            sumHMod += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();

            // 5. Extract-max [cite: 20]
            s = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) heap.extractMax();
            e = std::chrono::high_resolution_clock::now();
            sumHExt += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();




            // --- TESTY TABLICA ---
            ArrayQueue arrayQ;


            // 1. Insert
            s = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) arrayQ.insert(testData[j].value, testData[j].priority);
            e = std::chrono::high_resolution_clock::now();
            sumAIns += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();

            // 2. Peek
            s = std::chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) arrayQ.findMax();
            e = std::chrono::high_resolution_clock::now();
            sumAPeek += (std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);

            // 3. Return-size
            s = std::chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) arrayQ.returnSize();
            e = std::chrono::high_resolution_clock::now();
            sumASize += (std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);

            // 4. Modify-key
            s = std::chrono::high_resolution_clock::now();
            arrayQ.modifyKey(valToMod, 999999);
            e = std::chrono::high_resolution_clock::now();
            sumAMod += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();

            // 5. Extract-max
            s = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) arrayQ.extractMax();
            e = std::chrono::high_resolution_clock::now();
            sumAExt += std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();

            delete[] testData; // Zwolnienie pamięci po każdej iteracji 
        }

        // Uśrednianie wyników w nanosekundach
        finalResults.push_back({
            size,
            (sumHIns / iterations) / size, (sumHExt / iterations) / size, (sumHPeek / iterations), (sumHMod / iterations), (sumHSize / iterations),
            (sumAIns / iterations) / size, (sumAExt / iterations) / size, (sumAPeek / iterations), (sumAMod / iterations), (sumASize / iterations)
        });

        std::cout << ">> Ukonczono n = " << size << "\n";
    }

    // Zapis do pliku CSV
    std::ofstream resFile("data/wyniki_pelne.csv");
    resFile << "n;H_Ins;H_Ext;H_Peek;H_Mod;H_Size;A_Ins;A_Ext;A_Peek;A_Mod;A_Size\n";
    for (const auto& r : finalResults) {
        resFile << r.n << ";" << r.hIns << ";" << r.hExt << ";" << r.hPeek << ";" << r.hMod << ";" << r.hSize << ";"
                << r.aIns << ";" << r.aExt << ";" << r.aPeek << ";" << r.aMod << ";" << r.aSize << "\n";
    }
    resFile.close();
    std::cout << "Wyniki zapisano w 'data/wyniki_pelne.csv'\n";
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
