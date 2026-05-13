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
    vector<int> sizes = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000};
    const int iterations = 50; // Liczba prób
    const int fastOpRepeats = 1000; // Powtórzenia 
    


    struct FullResult {
        int n;
        double hIns, hExt, hPeek, hMod, hSize;
        double aIns, aExt, aPeek, aMod, aSize;
    };
    vector<FullResult> finalResults;



    cout << "Rozpoczynanie pelnych badan";

    for (int size : sizes) {
        double sumHIns = 0, sumHExt = 0, sumHPeek = 0, sumHMod = 0, sumHSize = 0;
        double sumAIns = 0, sumAExt = 0, sumAPeek = 0, sumAMod = 0, sumASize = 0;

        for (int i = 0; i < iterations; ++i) {
            string filename = "data/temp_test_data.txt";
            DataGenerator::generateRandomData(size, i + 100, filename); // Losowe ziarno


            int count = 0;
            Node* testData = loadDataFromFile(filename, count);
            if (!testData) continue;



            // TESTY KOPIEC 
            MaxHeap heap;


            // 1. Insert
            auto s = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) heap.insert(testData[j].value, testData[j].priority);
            auto e = chrono::high_resolution_clock::now();
            sumHIns += chrono::duration_cast<chrono::nanoseconds>(e - s).count();

            // 2. Peek
            s = chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) heap.findMax();
            e = chrono::high_resolution_clock::now();
            sumHPeek += (chrono::duration_cast<chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);

            // 3. Return-size
            s = chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) heap.returnSize();
            e = chrono::high_resolution_clock::now();
            sumHSize += (chrono::duration_cast<chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);


            // 4. Modify-key
            int valToMod = testData[size/2].value;
            s = chrono::high_resolution_clock::now();
            heap.modifyKey(valToMod, 999999);
            e = chrono::high_resolution_clock::now();
            sumHMod += chrono::duration_cast<chrono::nanoseconds>(e - s).count();

            // 5. Extract-max [cite: 20]
            s = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) heap.extractMax();
            e = chrono::high_resolution_clock::now();
            sumHExt += chrono::duration_cast<chrono::nanoseconds>(e - s).count();




            // TESTY TABLICA 
            ArrayQueue arrayQ;


            // 1. Insert
            s = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) arrayQ.insert(testData[j].value, testData[j].priority);
            e = chrono::high_resolution_clock::now();
            sumAIns += chrono::duration_cast<chrono::nanoseconds>(e - s).count();

            // 2. Peek
            s = chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) arrayQ.findMax();
            e = chrono::high_resolution_clock::now();
            sumAPeek += (chrono::duration_cast<chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);

            // 3. Return-size
            s = chrono::high_resolution_clock::now();
            for(int k=0; k<fastOpRepeats; k++) arrayQ.returnSize();
            e = chrono::high_resolution_clock::now();
            sumASize += (chrono::duration_cast<chrono::nanoseconds>(e - s).count() / (double)fastOpRepeats);

            // 4. Modify-key
            s = chrono::high_resolution_clock::now();
            arrayQ.modifyKey(valToMod, 999999);
            e = chrono::high_resolution_clock::now();
            sumAMod += chrono::duration_cast<chrono::nanoseconds>(e - s).count();

            // 5. Extract-max
            s = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) arrayQ.extractMax();
            e = chrono::high_resolution_clock::now();
            sumAExt += chrono::duration_cast<chrono::nanoseconds>(e - s).count();

            delete[] testData; 
        }

        
        finalResults.push_back({
            size,
            (sumHIns / iterations) / size, (sumHExt / iterations) / size, (sumHPeek / iterations), (sumHMod / iterations), (sumHSize / iterations),
            (sumAIns / iterations) / size, (sumAExt / iterations) / size, (sumAPeek / iterations), (sumAMod / iterations), (sumASize / iterations)
        });

        cout << ">> Ukonczono n = " << size << "\n";
    }

    // Zapis do pliku CSV
    ofstream resFile("data/wyniki_pelne.csv");
    resFile << "n;H_Ins;H_Ext;H_Peek;H_Mod;H_Size;A_Ins;A_Ext;A_Peek;A_Mod;A_Size\n";
    for (const auto& r : finalResults) {
        resFile << r.n << ";" << r.hIns << ";" << r.hExt << ";" << r.hPeek << ";" << r.hMod << ";" << r.hSize << ";"
                << r.aIns << ";" << r.aExt << ";" << r.aPeek << ";" << r.aMod << ";" << r.aSize << "\n";
    }
    resFile.close();
    cout << "Wyniki zapisano w 'data/wyniki_pelne.csv'\n";
}

int main() {
    int choice;
    do {
        cout << "\n--- PROJEKT SD: KOLEJKA PRIORYTETOWA ---\n";
        cout << "1. Uruchom badania\n";
        cout << "0. Wyjscie\n";
        cout << "Wybor: ";
        cin >> choice;



        if (choice == 1) {
            runFullResearch();
        }
    } while (choice != 0);

    return 0;
}
