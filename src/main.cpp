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
    vector<int> sizes = {5000, 10000, 20000, 40000, 60000, 80000, 100000, 120000};
    const int iterations = 50; // Liczba zestawów danych
    const int proby = 100;    // Liczba powtórzeń dla uśrednienia pojedynczej operacji

    struct FullResult {
        int n;
        double hIns, hExt, hPeek, hMod, hSize;
        double aIns, aExt, aPeek, aMod, aSize;
    };
    vector<FullResult> finalResults;

    cout << "Rozpoczynanie pelnych badan...\n";

    for (int size : sizes) {
        double sumHIns = 0, sumHExt = 0, sumHPeek = 0, sumHMod = 0, sumHSize = 0;
        double sumAIns = 0, sumAExt = 0, sumAPeek = 0, sumAMod = 0, sumASize = 0;

        for (int i = 0; i < iterations; ++i) {
            string filename = "data/temp_test_data.txt";
            DataGenerator::generateRandomData(size, i + 100, filename);

            int count = 0;
            Node* testData = loadDataFromFile(filename, count);
            if (!testData) continue;

            // --- TESTY KOPIEC ---
            MaxHeap heap;
        
            for (int j = 0; j < count; ++j) heap.insert(testData[j].value, testData[j].priority);
 
            auto s_ins = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) { /* pętla masowa do średniej */ } 
            MaxHeap tempHeap;
            auto sH = chrono::high_resolution_clock::now();
            for (int j = 0; j < count; ++j) tempHeap.insert(testData[j].value, testData[j].priority);
            auto eH = chrono::high_resolution_clock::now();
            sumHIns += (double)chrono::duration_cast<chrono::nanoseconds>(eH - sH).count() / count;

            // 2. H_Peek (100 prób na pełnym kopcu)
            long long t_hpeek = 0;
            for(int k=0; k<proby; k++) {
                auto s = chrono::high_resolution_clock::now();
                heap.findMax();
                auto e = chrono::high_resolution_clock::now();
                t_hpeek += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
            }
            sumHPeek += (double)t_hpeek / proby;

            // 3. H_Ext 
            long long t_hext = 0;
            for(int k=0; k<proby; k++) {
                auto s = chrono::high_resolution_clock::now();
                Node m = heap.extractMax();
                auto e = chrono::high_resolution_clock::now();
                t_hext += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
                heap.insert(m.value, m.priority); // Powrót do rozmiaru N
            }
            sumHExt += (double)t_hext / proby;

            // 4. H_Mod
            long long t_hmod = 0;
            for(int k=0; k<proby; k++) {
                int valToMod = testData[rand() % count].value;
                auto s = chrono::high_resolution_clock::now();
                heap.modifyKey(valToMod, rand() % 100000);
                auto e = chrono::high_resolution_clock::now();
                t_hmod += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
            }
            sumHMod += (double)t_hmod / proby;

            // 5. H_Size
            long long t_hsize = 0;
            for(int k=0; k<proby; k++) {
                auto s = chrono::high_resolution_clock::now();
                heap.returnSize();
                auto e = chrono::high_resolution_clock::now();
                t_hsize += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
            }
            sumHSize += (double)t_hsize / proby;


            // --- TESTY TABLICA ---
            ArrayQueue arrayQ;
            for (int j = 0; j < count; ++j) arrayQ.insert(testData[j].value, testData[j].priority);

            // 1. A_Ins
            auto sA = chrono::high_resolution_clock::now();
            ArrayQueue tempArray;
            for (int j = 0; j < count; ++j) tempArray.insert(testData[j].value, testData[j].priority);
            auto eA = chrono::high_resolution_clock::now();
            sumAIns += (double)chrono::duration_cast<chrono::nanoseconds>(eA - sA).count() / count;

            // 2. A_Peek
            long long t_apeek = 0;
            for(int k=0; k<proby; k++) {
                auto s = chrono::high_resolution_clock::now();
                arrayQ.findMax();
                auto e = chrono::high_resolution_clock::now();
                t_apeek += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
            }
            sumAPeek += (double)t_apeek / proby;

            // 3. A_Ext 
            long long t_aext = 0;
            for(int k=0; k<proby; k++) {
                auto s = chrono::high_resolution_clock::now();
                Node m = arrayQ.extractMax();
                auto e = chrono::high_resolution_clock::now();
                t_aext += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
                arrayQ.insert(m.value, m.priority); 
            }
            sumAExt += (double)t_aext / proby;

            // 4. A_Mod
            long long t_amod = 0;
            for(int k=0; k<proby; k++) {
                int valToMod = testData[rand() % count].value;
                auto s = chrono::high_resolution_clock::now();
                arrayQ.modifyKey(valToMod, rand() % 100000);
                auto e = chrono::high_resolution_clock::now();
                t_amod += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
            }
            sumAMod += (double)t_amod / proby;

            // 5. A_Size
            long long t_asize = 0;
            for(int k=0; k<proby; k++) {
                auto s = chrono::high_resolution_clock::now();
                arrayQ.returnSize();
                auto e = chrono::high_resolution_clock::now();
                t_asize += chrono::duration_cast<chrono::nanoseconds>(e - s).count();
            }
            sumASize += (double)t_asize / proby;

            delete[] testData; 
        }

        finalResults.push_back({
            size,
            sumHIns / iterations, 
            sumHExt / iterations, 
            sumHPeek / iterations, 
            sumHMod / iterations, 
            sumHSize / iterations,
            sumAIns / iterations, 
            sumAExt / iterations, 
            sumAPeek / iterations, 
            sumAMod / iterations, 
            sumASize / iterations
        });

        cout << ">> Ukonczono n = " << size << "\n";
    }

    ofstream resFile("data/wyniki_pelne.csv");
    resFile << "n;H_Ins;H_Ext;H_Peek;H_Mod;H_Size;A_Ins;A_Ext;A_Peek;A_Mod;A_Size\n";
    resFile << fixed << setprecision(0); 

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
