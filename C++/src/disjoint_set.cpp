#include <assert.h>
#include <unordered_map>
#include "disjoint_set.h"

typedef struct DisjointSetNode DisjointSetNode;
struct DisjointSetNode {
    DisjointSetNode * parent;
    std::string s;
    size_t rank;
};

DisjointSetNode * createNewNode(std::string & s) {
    DisjointSetNode * node = new DisjointSetNode;
    node->parent = NULL;
    node->s = s;
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

size_t DisjointSet::size() {
    return this->ptr->size;
}

bool DisjointSet::empty() {
    return size() == 0;
}

DisjointSet::~DisjointSet() {
    for (std::pair<std::string, DisjointSetNode *> kvPair : this->ptr->nodeMap) {
        DisjointSetNode * node = kvPair.second;
        delete node;
    }

    delete this->ptr;
}
