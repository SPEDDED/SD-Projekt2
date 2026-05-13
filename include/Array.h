#ifndef ARRAY_H
#define ARRAY_H

#include "Heap.h"

class ArrayQueue {
private:
    Node* data;
    int capacity;
    int size;

    void resize();

public:
    ArrayQueue();
    ~ArrayQueue();

    void insert(int value, int priority);
    Node extractMax();
    Node findMax();
    void modifyKey(int value, int newPriority);
    int returnSize();
    
    void display();
};

#endif
