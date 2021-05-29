#include "fibheap.h"

#include <unordered_map>
#include <assert.h>

class LinkedList {
public:
    typedef struct LLNode {
        LLNode * prev;
        LLNode * next;
        void * data;
    } LLNode;

    LinkedList() {
        this->n = 0;
        this->head = NULL;
        this->tail = NULL;
        this->minElem = NULL;
    };

    ~LinkedList() {
        if (this->n == 0) return;

        LLNode * ptr = this->head;
        while (ptr != NULL) {
            LLNode * next = ptr->next;
            delete ptr;
            ptr = next;
        }
    };

    LLNode * push(std::string, float key);
    LLNode * pushNode(LLNode * node);
    LLNode * remove(LLNode * node);
    LLNode * pop();
    void merge(const LinkedList & other);
    LLNode * min() {
        assert(size() > 0);
        return this->minElem;
    };

    size_t size() {
        return this->n;
    };

private:
    size_t n;
    LLNode * minElem;
    LLNode * head;
    LLNode * tail;
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

FibonacciHeap::FibonacciHeap(bool reverse) {
    this->ptr = new ClassVars;
    this->ptr->size = 0;
    this->ptr->reverse = reverse;
    this->ptr->minNode = NULL;
    this->ptr->rootList = LinkedList();
    this->ptr->nodeMap = std::unordered_map<std::string, FibNode *>();
}
