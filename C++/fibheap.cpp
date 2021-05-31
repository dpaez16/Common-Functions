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

FibonacciHeap::FibonacciHeap() {
    FibonacciHeap(false);
}

FibonacciHeap::FibonacciHeap(bool reverse) {
    this->ptr = new ClassVars;
    this->ptr->size = 0;
    this->ptr->reverse = reverse;
    this->ptr->minNode = NULL;
    this->ptr->rootHead = NULL;
    this->ptr->rootTail = NULL;
    this->ptr->nodeMap = std::unordered_map<std::string, FibNode *>();
}

void FibonacciHeap::copy(const FibonacciHeap & other) {
    this->ptr = new ClassVars;
    this->ptr->size = 0;
    this->ptr->reverse = other.ptr->reverse;
    this->ptr->minNode = NULL;
    this->ptr->rootHead = NULL;
    this->ptr->rootTail = NULL;
    this->ptr->nodeMap = std::unordered_map<std::string, FibNode *>();

    for (std::pair<std::string, FibNode *> kvPair : other.ptr->nodeMap) {
        std::string elem = kvPair.first;
        this->push(elem, kvPair.second->key);
    }

    for (std::pair<std::string, FibNode *> kvPair : this->ptr->nodeMap) {
        std::string elem = kvPair.first;
        FibNode * node = kvPair.second;
        FibNode * otherNode = other.ptr->nodeMap[elem];

        FibNode * parent = otherNode->parent;
        FibNode * next = otherNode->next;
        FibNode * prev = otherNode->prev;
        FibNode * childHead = otherNode->childHead;
        FibNode * childTail = otherNode->childTail;

        if (parent != NULL) node->parent = this->ptr->nodeMap[parent->elem];
        if (node->next != NULL) node->next = this->ptr->nodeMap[next->elem];
        if (node->prev != NULL) node->prev = this->ptr->nodeMap[prev->elem];
        if (node->childHead != NULL) node->childHead = this->ptr->nodeMap[childHead->elem];
        if (node->childTail != NULL) node->childTail = this->ptr->nodeMap[childTail->elem];

        node->rank = otherNode->rank;
        node->marked = otherNode->marked;
    }
}

FibonacciHeap::FibonacciHeap(const FibonacciHeap & other) {
    copy(other);
}

FibonacciHeap & FibonacciHeap::operator=(const FibonacciHeap & other) {
    if (this == &other) return *this;
    
    copy(other);
    return *this;
}

FibonacciHeap::~FibonacciHeap() {
    for (std::pair<std::string, FibNode *> kvPair : this->ptr->nodeMap) {
        delete kvPair.second;
    }

    delete this->ptr;
}

bool FibonacciHeap::contains(std::string elem) {
    return this->ptr->nodeMap.find(elem) != this->ptr->nodeMap.end();
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
    assert(!contains(elem));

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

std::pair<FibNode *, FibNode *> getNodes(FibNode *& a, FibNode *& b, bool reverse) {
    std::pair<FibNode *, FibNode *> p(a, b);
    std::pair<FibNode *, FibNode *> p2(b, a);
    
    if (reverse) {
        return a->key < b->key ? p2 : p;
    }

    return a->key < b->key ? p : p2;
}

void FibonacciHeap::consolidate() {
    std::unordered_map<size_t, FibNode *> rankMap;
    FibNode * ptr = this->ptr->rootHead;
    bool reverse = this->ptr->reverse;

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
            
            // remove child node from root list
            FibNode * back = childNode->prev;
            FibNode * front = childNode->next;
            childNode->prev = NULL;
            childNode->next = NULL;

            if (childNode == this->ptr->rootHead) {
                front->prev = NULL;
                this->ptr->rootHead = this->ptr->rootHead->next;
            } else if (childNode == this->ptr->rootTail) {
                back->next = NULL;
                this->ptr->rootTail = back;
            }

            // attach child node to main node
            childNode->parent = mainNode;
            if (mainNode->rank == 0) {
                mainNode->childHead = childNode;
                mainNode->childTail = childNode;
            } else {
                mainNode->childTail->next = childNode;
                childNode->prev = mainNode->childTail;
                mainNode->childTail = childNode;
            }
            mainNode->rank++;

            rankMap[rank] = NULL;
            ptr = this->ptr->rootHead;
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
    consolidate();
    this->ptr->minNode = findMinNode(this->ptr->rootHead, this->ptr->reverse);
}

void cut(FibNode *& node) {
    FibNode * parent = node->parent;
    if (parent == NULL) {
        parent->marked = false;
        return;
    }

    // remove node from parent's child list
    FibNode * head = parent->childHead;
    FibNode * tail = parent->childTail;
    FibNode * back = node->prev;
    FibNode * front = node->next;
    node->prev = NULL;
    node->next = NULL;
    node->parent = NULL;

    if (parent->rank == 1) {
        parent->childHead = NULL;
        parent->childTail = NULL;
    } else if (node == head) {
        front->prev = NULL;
        parent->childHead = front;
    } else if (node == tail) {
        back->next = NULL;
        parent->childTail = back;
    }

    parent->rank--;
}

void FibonacciHeap::decreaseKey(std::string elem, float newKey) {
    assert(contains(elem));

    FibNode * node = this->ptr->nodeMap[elem];
    if (newKey == node->key) return;

    assert(newKey < node->key);

    node->key = newKey;
    FibNode * parent = node->parent;
    bool reverse = this->ptr->reverse;

    if (parent == NULL) {
        this->ptr->minNode = getMinNode(this->ptr->minNode, node, reverse);
        return;
    }

    if (parent->key <= newKey) return;

    cut(node);
    this->ptr->rootTail->next = node;
    node->prev = this->ptr->rootTail;
    this->ptr->rootTail = node;

    node->marked = false;
    this->ptr->minNode = getMinNode(this->ptr->minNode, node, reverse); 

    node = parent;
    parent = parent->parent;

    while (parent != NULL) {
        if (!node->marked) {
            node->marked = true;
            break;
        }

        cut(node);
        this->ptr->rootTail->next = node;
        node->prev = this->ptr->rootTail;
        this->ptr->rootTail = node;

        node = parent;
        parent = parent->parent;
    }
}
