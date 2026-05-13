#include "Array.h"
#include <iostream>

using namespace std;

ArrayQueue::ArrayQueue() {
    capacity = 10;
    size = 0;
    data = new Node[capacity];
}

ArrayQueue::~ArrayQueue() {
    delete[] data;
}

void ArrayQueue::resize() {
    capacity *= 2;
    Node* newData = new Node[capacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

void ArrayQueue::insert(int value, int priority) {
    if (size == capacity) {
        resize();
    }
    data[size].value = value;
    data[size].priority = priority;
    size++;
}

Node ArrayQueue::extractMax() {
    if (size <= 0) return {-1, -1};

    int maxIdx = 0;
    for (int i = 1; i < size; i++) {
        if (data[i].priority > data[maxIdx].priority) {
            maxIdx = i;
        }
    }

    Node maxNode = data[maxIdx];
    data[maxIdx] = data[size - 1];
    size--;

    return maxNode;
}

Node ArrayQueue::findMax() {
    if (size <= 0) return {-1, -1};
    
    int maxIdx = 0;
    for (int i = 1; i < size; i++) {
        if (data[i].priority > data[maxIdx].priority) {
            maxIdx = i;
        }
    }
    return data[maxIdx];
}

void ArrayQueue::modifyKey(int value, int newPriority) {
    for (int i = 0; i < size; i++) {
        if (data[i].value == value) {
            data[i].priority = newPriority;
            return;
        }
    }
}

int ArrayQueue::returnSize() {
    return size;
}

void ArrayQueue::display() {
    cout << "Tablica (rozmiar: " << size << "):" << endl;
    for (int i = 0; i < size; i++) {
        cout << "[" << i << "] P: " << data[i].priority << ", V: " << data[i].value << endl;
    }
}
