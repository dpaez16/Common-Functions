#include "fibheap.h"

#include <unordered_map>
#include <assert.h>


typedef struct FibNode {
    std::string elem;
    float key;
    FibNode * parent;
    FibNode * next;
    FibNode * prev;
    FibNode * childHead;
    FibNode * childTail;
    size_t rank;
    bool marked;
} FibNode;

struct FibonacciHeap::ClassVars {
    size_t size;
    bool reverse;
    FibNode * minNode;
    FibNode * rootHead;
    FibNode * rootTail;
    std::unordered_map<std::string, FibNode *> nodeMap;
};

FibonacciHeap::FibonacciHeap(bool reverse) {
    this->ptr = new ClassVars;
    this->ptr->size = 0;
    this->ptr->reverse = reverse;
    this->ptr->minNode = NULL;
    this->ptr->rootHead = NULL;
    this->ptr->rootTail = NULL;
    this->ptr->nodeMap = std::unordered_map<std::string, FibNode *>();
}

void FibonacciHeap::push(std::string elem, float key) {
    FibNode * node = new FibNode;
    node->elem = elem;
    node->key = key;
    node->parent = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->childHead = NULL;
    node->childTail = NULL;
    node->rank = 0;
    node->marked = false;

    this->ptr->size++;
    if (this->ptr->size == 1) {
        this->ptr->rootHead = node;
        this->ptr->rootTail = node;
        this->ptr->minNode = node;
	return;
    }

    this->ptr->rootTail->next = node;
    node->prev = this->ptr->rootTail;
    this->ptr->rootTail = node;
}

std::string FibonacciHeap::top() {
    assert(size() > 0);

    return this->ptr->minNode->elem;
}

size_t FibonacciHeap::size() {
    return this->ptr->size;
}
