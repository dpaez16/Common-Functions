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

DisjointSet::~DisjointSet() {
    for (std::pair<std::string, DisjointSetNode *> kvPair : this->ptr->nodeMap) {
        DisjointSetNode * node = kvPair.second;
        delete node;
    }

    delete this->ptr;
}
