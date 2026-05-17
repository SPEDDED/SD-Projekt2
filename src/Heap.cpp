#include "Heap.h"
 
using namespace std;

MaxHeap::MaxHeap() {
    capacity = 10;// Początkowa pojemność
    size = 0;
    data = new Node[capacity]; // Dynamiczna alokacja
}

MaxHeap::~MaxHeap() {
    delete[] data; 
}

void MaxHeap::resize() {
    capacity *= 2; // Podwojenie pojemności
    Node* newData = new Node[capacity]; // Stworzenie nowej tablicy o większej pojemności
    for (int i = 0; i < size; i++) {
        newData[i] = data[i]; // Kopiowanie danych
    }
    delete[] data; //destruktor
    data = newData;
}

void MaxHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (data[index].priority > data[parent].priority) { //zmiana jeśli priorytet jest większy niż rodzica

            Node temp = data[index]; 
            data[index] = data[parent];
            data[parent] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

void MaxHeap::heapifyDown(int index) {
    while (true) { 
        int left = 2 * index + 1; //lewe dziecko
        int right = 2 * index + 2; //prawe dziecko
        int largest = index; 

        if (left < size && data[left].priority > data[largest].priority) { 
            largest = left;
        }
        if (right < size && data[right].priority > data[largest].priority) {
            largest = right;
        }

        if (largest != index) {//zmiana jeśli dziecko ma większy priorytet
            Node temp = data[index];
            data[index] = data[largest];
            data[largest] = temp;
            index = largest;
        } else {
            break;
        }
    }
}

void MaxHeap::insert(int value, int priority) {
    if (size == capacity) { //Zwiększzenie rozmariu jak tablica jest zapełniona

        resize();
    }
    data[size].value = value;
    data[size].priority = priority;
    heapifyUp(size);
    size++;
}

Node MaxHeap::extractMax() {
    if (size <= 0) return {-1, -1}; //Dla pustego kopca zwrócenie braku elementu
    
    Node maxNode = data[0];
    data[0] = data[size - 1];
    size--;
    heapifyDown(0);
    return maxNode; 
}

Node MaxHeap::findMax() {
    if (size <= 0) return {-1, -1}; //Dla pustego kopca zwrócenie braku elementu
    return data[0];
}

void MaxHeap::modifyKey(int value, int newPriority) { 
    for (int i = 0; i < size; i++) {
        if (data[i].value == value) {
            int oldPriority = data[i].priority; // zapisanie starego priorytetu
            data[i].priority = newPriority;// nowy priorytet
            if (newPriority > oldPriority) heapifyUp(i); // jeśli nowy priorytet jest większy to wywołanie heapifyUp, inaczej dfheapifyDown
            else heapifyDown(i);
            return; 
        }
    }
}

int MaxHeap::returnSize() {
    return size;
}

void MaxHeap::display() {
    cout << "Kopiec (rozmiar: " << size << "):" << endl;
    for (int i = 0; i < size; i++) {
        cout << "[" << i << "] Priorytet: " << data[i].priority 
                  << ", Wartosc: " << data[i].value << endl;
    }
}
