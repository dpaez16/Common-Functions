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

FibNode * createFibNode(std::string elem, float key) {
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

    return node;
}

FibNode * getMinNode(FibNode *& a, FibNode *& b, bool reverse) {
    if (reverse) {
        return a->key < b->key ? b : a;
    }

    return a->key < b->key ? a : b;
}

void FibonacciHeap::push(std::string elem, float key) {
    assert(this->ptr->nodeMap.find(elem) == this->ptr->nodeMap.end());

    FibNode * node = createFibNode(elem, key);
    this->ptr->nodeMap[elem] = node;

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

    this->ptr->minNode = getMinNode(this->ptr->minNode, node, this->ptr->reverse);
}

std::string FibonacciHeap::top() {
    assert(size() > 0);

    return this->ptr->minNode->elem;
}

size_t FibonacciHeap::size() {
    return this->ptr->size;
}

bool FibonacciHeap::empty() {
    return this->ptr->size == 0;
}

FibNode * findMinNode(FibNode *& head, bool reverse) {
    assert(head != NULL);

    FibNode * ptr = head;
    FibNode * minNode = head;

    while (ptr != NULL) {
        minNode = getMinNode(ptr, minNode, reverse);
        ptr = ptr->next;
    }

    return minNode;
}

void consolidate(FibNode *& rootHead, FibNode *& rootTail, bool reverse) {
    std::unordered_map<size_t, FibNode *> rankMap;
    FibNode * ptr = rootHead;

    while (ptr != NULL) {
        size_t rank = ptr->rank;
        if (rankMap.find(rank) == rankMap.end()) {
            rankMap[rank] = ptr;
            ptr = ptr->next;
        } else if (rankMap[rank] == ptr) {
            ptr = ptr->next;
        } else {
            FibNode * otherNode = rankMap[rank];
            std::pair<FibNode *, FibNode *> nodes = getNodes(ptr, otherNode, reverse);
            FibNode * mainNode = nodes.first;
            FibNode * childNode = nodes.second;
            remove(childNode);
            childNode->parent = mainNode;
            mainNode->rank++;

            rankMap[rank] = NULL;
            ptr = rootHead;
        }
    }
}

void FibonacciHeap::pop() {
    assert(!empty());

    std::string elem = top();
    this->ptr->size--;
    this->ptr->nodeMap.erase(elem);

    if (this->ptr->size == 0) {
        this->ptr->rootHead = NULL;
        this->ptr->rootTail = NULL;
        this->ptr->minNode = NULL;
        return;
    }

    FibNode * minNode = this->ptr->minNode;
    FibNode * childHead = minNode->childHead;
    FibNode * childTail = minNode->childTail;

    if (childHead == NULL) return;

    FibNode * ptr = childHead;
    while (ptr != NULL) {
        ptr->parent = NULL;
        ptr = ptr->next;
    }

    this->ptr->rootTail->next = childHead;
    this->ptr->rootTail = childTail;
    consolidate(this->ptr->rootHead, this->ptr->rootTail, this->ptr->reverse);
    this->ptr->minNode = findMinNode(this->ptr->rootHead, this->ptr->reverse);
}
