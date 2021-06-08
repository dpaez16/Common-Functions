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
        float key = kvPair.second->key;
        this->push(elem, key);
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
        if (next != NULL) node->next = this->ptr->nodeMap[next->elem];
        if (prev != NULL) node->prev = this->ptr->nodeMap[prev->elem];
        if (childHead != NULL) node->childHead = this->ptr->nodeMap[childHead->elem];
        if (childTail != NULL) node->childTail = this->ptr->nodeMap[childTail->elem];

        node->rank = otherNode->rank;
        node->marked = otherNode->marked;
    }
}

FibonacciHeap::FibonacciHeap(const FibonacciHeap & other) {
    copy(other);
}

FibonacciHeap & FibonacciHeap::operator=(const FibonacciHeap & other) {
    if (this == &other) return *this;
    
    this->~FibonacciHeap();
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
    FibNode * ptr = head;
    FibNode * minNode = head;

    while (ptr != NULL) {
        minNode = getMinNode(minNode, ptr, reverse);
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

std::pair<FibNode *, FibNode *> removeNode(FibNode *& node, FibNode *& head, FibNode *& tail) {
    if (node == NULL) return {head, tail};

    FibNode * back = node->prev;
    FibNode * front = node->next;
    node->next = NULL;
    node->prev = NULL;

    if (back != NULL && front != NULL) {
        back->next = front;
        front->prev = back;
        return {head, tail};
    } else if (back == NULL && front == NULL) {
        return {NULL, NULL};
    } else if (back == NULL && front != NULL) {
        front->prev = NULL;
        return {front, tail};
    } else {
        back->next = NULL;
        return {head, back};
    }
}

void attachChildNode(FibNode *& mainNode, FibNode *& childNode) {
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
}

std::pair<FibNode *, FibNode *> bumpNodeChildren(FibNode *& node, FibNode *& head, FibNode *& tail) {
    assert(node != NULL);

    FibNode * childHead = node->childHead;
    FibNode * childTail = node->childTail;

    FibNode * parent = head != NULL ? head->parent : NULL;
    FibNode * ptr = childHead;
    while (ptr != NULL) {
        ptr->parent = parent;
        ptr = ptr->next;
    }

    if (head == NULL) return {childHead, childTail};
    if (childHead == NULL) return {head, tail};

    tail->next = childHead;
    childHead->prev = tail;

    return {head, childTail};
}

void FibonacciHeap::consolidate() {
    std::unordered_map<size_t, FibNode *> rankMap;
    FibNode * ptr = this->ptr->rootHead;
    bool reverse = this->ptr->reverse;

    while (ptr != NULL) {
        size_t rank = ptr->rank;
        if (rankMap.find(rank) == rankMap.end() || rankMap[rank] == NULL) {
            rankMap[rank] = ptr;
            ptr = ptr->next;
        } else if (rankMap[rank] == ptr) {
            ptr = ptr->next;
        } else {
            FibNode * otherNode = rankMap[rank];
            std::pair<FibNode *, FibNode *> nodes = getNodes(ptr, otherNode, reverse);
            FibNode * mainNode = nodes.first;
            FibNode * childNode = nodes.second;
            
            std::pair<FibNode *, FibNode *> p = removeNode(childNode, this->ptr->rootHead, this->ptr->rootTail);
            this->ptr->rootHead = p.first;
            this->ptr->rootTail = p.second;

            attachChildNode(mainNode, childNode);

            rankMap[rank] = NULL;
            ptr = this->ptr->rootHead;
        }
    }
}

void FibonacciHeap::pop() {
    assert(!empty());

    std::string elem = top();
    this->ptr->nodeMap.erase(elem);
    this->ptr->size--;

    std::pair<FibNode *, FibNode *> p = removeNode(this->ptr->minNode, this->ptr->rootHead, this->ptr->rootTail);
    this->ptr->rootHead = p.first;
    this->ptr->rootTail = p.second;

    FibNode * minNode = this->ptr->minNode;
    p = bumpNodeChildren(minNode, this->ptr->rootHead, this->ptr->rootTail);
    this->ptr->rootHead = p.first;
    this->ptr->rootTail = p.second;
    delete minNode;

    consolidate();
    this->ptr->minNode = findMinNode(this->ptr->rootHead, this->ptr->reverse);
}

void cut(FibNode *& node, FibNode *& parent) {
    if (parent == NULL) {
        parent->marked = false;
        return;
    }

    // remove node from parent's child list
    std::pair<FibNode *, FibNode *> p = removeNode(node, parent->childHead, parent->childTail);
    parent->childHead = p.first;
    parent->childTail = p.second;

    parent->rank--;
}

void FibonacciHeap::decreaseKey(std::string elem, float newKey) {
    assert(contains(elem));

    FibNode * node = this->ptr->nodeMap[elem];
    if (newKey == node->key) return;
    bool reverse = this->ptr->reverse;

    assert(
        (!reverse && newKey < node->key) ||
        (reverse && newKey > node->key)
    );

    node->key = newKey;
    FibNode * parent = node->parent;

    if (parent == NULL) {
        this->ptr->minNode = getMinNode(this->ptr->minNode, node, reverse);
        return;
    }

    if (!reverse && parent->key <= newKey) return;
    if (reverse && parent->key >= newKey) return;

    cut(node, parent);
    node->parent = NULL;
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

        cut(node, parent);
        node->parent = NULL;
        this->ptr->rootTail->next = node;
        node->prev = this->ptr->rootTail;
        this->ptr->rootTail = node;
        node->marked = false;

        node = parent;
        parent = parent->parent;
    }
}
