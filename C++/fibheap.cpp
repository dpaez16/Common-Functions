#include "fibheap.h"

#include <unordered_map>

class LinkedList {
    typedef struct LLNode {
        LLNode * prev;
        LLNode * next;
        void * data;
    } LLNode;

    LinkedList();
    ~LinkedList();
    LLNode * push(std::string, float key);
    LLNode * pushNode(LLNode * node);
    LLNode * remove(LLNode * node);
    LLNode * pop();
    void merge(const LinkedList & other);
    LLNode * min();
    size_t size();
};

typedef struct FibNode {
    std::string elem;
    float key;
    FibNode * parent;
    LinkedList childList;
    bool marked;
} FibNode;

struct FibonacciHeap::ClassVars {
    size_t size;
    bool reverse;
    FibNode * minNode;
    LinkedList rootList;
    std::unordered_map<std::string, FibNode *> nodeMap;
};
