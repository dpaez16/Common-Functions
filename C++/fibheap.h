#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <string>

class FibonacciHeap {
public:
    FibonacciHeap(bool reverse);
    //FibonacciHeap(const FibonacciHeap &);
    //FibonacciHeap& operator=(const FibonacciHeap &);
    void push(std::string elem, float key);
    std::string top();
    void pop();
    //void decreaseKey(std::string elem, float newKey);
    size_t size();
    bool empty();
    //~FibonacciHeap();

private:
    struct ClassVars;
    ClassVars * ptr;
};

#endif