#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <string>

class FibonacciHeap {
public:
    FibonacciHeap(bool reverse = false);
    FibonacciHeap(const FibonacciHeap &);
    FibonacciHeap& operator=(const FibonacciHeap &);
    void push(std::string elem, float key);
    std::string top();
    void pop();
    void decreaseKey(std::string elem, float newKey);
    bool contains(std::string elem);
    size_t size();
    bool empty();
    ~FibonacciHeap();

private:
    void consolidate();
    void copy(const FibonacciHeap &);
    struct ClassVars;
    ClassVars * ptr;
};

#endif