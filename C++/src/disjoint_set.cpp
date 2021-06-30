#include <assert.h>
#include <unordered_map>
#include "disjoint_set.h"

typedef struct DisjointSetNode DisjointSetNode;
struct DisjointSetNode {
    DisjointSetNode * parent;
    std::string elem;
    size_t rank;
};

DisjointSetNode * createNewNode(std::string & elem) {
    DisjointSetNode * node = new DisjointSetNode;
    node->parent = NULL;
    node->elem = elem;
    node->rank = 0;

    return node;
}

struct DisjointSet::ClassVars {
    size_t size;
    std::unordered_map<std::string, DisjointSetNode *> nodeMap;
};

DisjointSet::DisjointSet() {
    this->ptr = new ClassVars;
    this->ptr->size = 0;
}

DisjointSet::DisjointSet(std::vector<std::string> & vec) {
    this->ptr = new ClassVars;
    this->ptr->size = 0;

    for (std::string s : vec) {
        insert(s);
    }
}

void DisjointSet::copy(const DisjointSet & other) {
    this->ptr = new ClassVars;
    this->ptr->size = other.ptr->size;
    
    for (std::pair<std::string, DisjointSetNode *> kvPair : other.ptr->nodeMap) {
        std::string elem = kvPair.first;
        this->insert(elem);
    }

    for (std::pair<std::string, DisjointSetNode *> kvPair : this->ptr->nodeMap) {
        std::string elem = kvPair.first;
        DisjointSetNode * node = kvPair.second;

        DisjointSetNode * otherNode = other.ptr->nodeMap[elem];
        node->parent = this->ptr->nodeMap[otherNode->parent->elem];
        node->rank = otherNode->rank;
    }
}

DisjointSet::DisjointSet(const DisjointSet & other) {
    copy(other);
}

DisjointSet & DisjointSet::operator=(const DisjointSet & other) {
if (this == &other) return *this;

    this->~DisjointSet();
    copy(other);
    return *this;
}

size_t DisjointSet::size() {
    return this->ptr->size;
}

bool DisjointSet::empty() {
    return size() == 0;
}

bool DisjointSet::contains(std::string x) {
    return this->ptr->nodeMap.find(x) != this->ptr->nodeMap.end();
}

void DisjointSet::insert(std::string x) {
    if (contains(x)) return;

    DisjointSetNode * node = createNewNode(x);
    node->parent = node;
    this->ptr->nodeMap[x] = node;
    this->ptr->size++;
}

bool isRoot(DisjointSetNode *& node) {
    DisjointSetNode * parent = node->parent;

    return node == parent;
}

std::string DisjointSet::find(std::string x) {
    assert(contains(x));
    
    DisjointSetNode * curr = this->ptr->nodeMap[x];
    if (!isRoot(curr)) {
        curr->parent = this->ptr->nodeMap[find(curr->parent->elem)];
    }
    
    return curr->parent->elem;
}

void DisjointSet::setUnion(std::string x, std::string y) {
    if (!contains(x) || !contains(y)) return;

    DisjointSetNode * u = this->ptr->nodeMap[find(x)];
    DisjointSetNode * v = this->ptr->nodeMap[find(y)];

    if (u->rank == v->rank) {
        u->rank++;
        v->parent = u;
    } else if (u->rank > v->rank) {
        v->parent = u;
    } else {
        u->parent = v;
    }
}

DisjointSet::~DisjointSet() {
    for (std::pair<std::string, DisjointSetNode *> kvPair : this->ptr->nodeMap) {
        DisjointSetNode * node = kvPair.second;
        delete node;
    }

    delete this->ptr;   
}
