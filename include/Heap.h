#ifndef HEAP_H
#define HEAP_H
#include <iostream>
#include <string>

struct Node {
    int priority; 
    int value;    
};

class MaxHeap {
private:
    Node* data;        
    int capacity;     
    int size;          

    
    void heapifyUp(int index);   
    void heapifyDown(int index); 
    void resize();               

public:
    MaxHeap();  
    ~MaxHeap(); 

    void insert(int value, int priority); 
    Node extractMax();                    
    Node findMax();                      
    void modifyKey(int value, int newPriority); 
    int returnSize();                    
    
    
    void display(); 
};

#endif
