#include "Heap.h"

MaxHeap::MaxHeap() {
    capacity = 10;// Początkowa pojemność
    size = 0;
    data = new Node[capacity]; // Dynamiczna alokacja
}

MaxHeap::~MaxHeap() {
    delete[] data; // Zwolnienie pamięci
}

void MaxHeap::resize() {
    capacity *= 2; // Podwojenie rozmiaru
    Node* newData = new Node[capacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

void MaxHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (data[index].priority > data[parent].priority) {
            // Zamiana (swap)
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
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && data[left].priority > data[largest].priority) {
            largest = left;
        }
        if (right < size && data[right].priority > data[largest].priority) {
            largest = right;
        }

        if (largest != index) {
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
    if (size == capacity) {
        resize();
    }
    data[size].value = value;
    data[size].priority = priority;
    heapifyUp(size);
    size++;
}

Node MaxHeap::extractMax() {
    if (size <= 0) return {-1, -1};
    
    Node maxNode = data[0];
    data[0] = data[size - 1];
    size--;
    heapifyDown(0);
    return maxNode;
}

Node MaxHeap::findMax() {
    if (size <= 0) return {-1, -1};
    return data[0];
}

void MaxHeap::modifyKey(int value, int newPriority) {
    // W kopcu wyszukiwanie wartości 'e' zajmuje O(n) 
    for (int i = 0; i < size; i++) {
        if (data[i].value == value) {
            int oldPriority = data[i].priority;
            data[i].priority = newPriority;
            
            if (newPriority > oldPriority) {
                heapifyUp(i);
            } else {
                heapifyDown(i);
            }
            return;
        }
    }
}

int MaxHeap::returnSize() {
    return size;
}

void MaxHeap::display() {
    std::cout << "Kopiec (rozmiar: " << size << "):" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "[" << i << "] Priorytet: " << data[i].priority 
                  << ", Wartosc: " << data[i].value << std::endl;
    }
}