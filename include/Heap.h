#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <string>

// Struktura reprezentująca element kolejki: para klucz-wartość [cite: 12]
struct Node {
    int priority; // Klucz (priorytet)
    int value;    // Wartość (4-bajtowa liczba całkowita) [cite: 832]
};

class MaxHeap {
private:
    Node* data;        // Dynamiczna tablica na elementy kopca [cite: 830]
    int capacity;      // Aktualny rozmiar zarezerwowanej pamięci
    int size;          // Aktualna liczba elementów w kopcu

    // Metody pomocnicze do utrzymania własności kopca
    void heapifyUp(int index);   // Przywracanie struktury w górę po dodaniu elementu
    void heapifyDown(int index); // Przywracanie struktury w dół po usunięciu elementu
    void resize();               // Dynamiczne powiększanie tablicy [cite: 830]

public:
    MaxHeap();  // Konstruktor - alokacja początkowa
    ~MaxHeap(); // Destruktor - zwolnienie pamięci [cite: 830]

    // Operacje wymagane przez miniprojekt [cite: 17]
    void insert(int value, int priority); // insert(e,p): dodanie elementu [cite: 18, 19]
    Node extractMax();                    // extract-max(): usunięcie i zwrot największego [cite: 20]
    Node findMax();                       // find-max()/peek(): podejrzenie największego [cite: 21]
    void modifyKey(int value, int newPriority); // modify-key(e, p): zmiana priorytetu 
    int returnSize();                     // return-size: rozmiar kolejki [cite: 23]
    
    // Metoda do menu i diagnostyki
    void display(); // Wyświetlanie struktury kopca [cite: 850]
};

#endif