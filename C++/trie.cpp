#include <cstring>
#include "trie.h"

typedef struct TrieNode TrieNode;
struct TrieNode {
    TrieNode * children[256];
    bool terminal;
};

struct Trie::ClassVars {
    size_t size;
    TrieNode * root;
};

TrieNode * createNewNode() {
    TrieNode * node = new TrieNode;
    memset(node->children, 0, sizeof(node->children));
    node->terminal = false;

    return node;
}

void cleanup(TrieNode * root) {
    if (root == NULL) return;

    for (int i = 0; i < 256; i++) {
        if (root->children[i] == NULL) continue;

        cleanup(root->children[i]);
    }

    delete root;
}

Trie::Trie() {
    this->ptr = new ClassVars;
    this->ptr->size = 0;
    this->ptr->root = createNewNode();
}

Trie::~Trie() {
    cleanup(this->ptr->root);
    delete this->ptr;
}

size_t Trie::size() {
    return this->ptr->size;
}

bool Trie::empty() {
    return size() == 0;
}

void Trie::insert(std::string word) {
    TrieNode * curr = this->ptr->root;

    for (size_t i = 0; i < word.size(); i++) {
        int idx = word[i];
        if (curr->children[idx] == NULL) {
            TrieNode * newNode = createNewNode();
            curr->children[idx] = newNode;
        }

        curr = curr->children[idx];
    }

    if (!curr->terminal) {
        this->ptr->size++;
    }

    curr->terminal = true;
}

int getNumChildren(TrieNode * node) {
    int numChildren = 0;
    for (int i = 0; i < 256; i++) {
        if (node->children[i] == NULL) continue;

        numChildren++;
    }

    return numChildren;
}

bool isSingularBranch(TrieNode * root) {
    if (root == NULL) return true;

    int numChildren = getNumChildren(root);
    if (numChildren > 1) return false;

    for (int i = 0; i < 256; i++) {
        if (root->children[i] == NULL) continue;

        return isSingularBranch(root->children[i]);
    }

    return true;
}

void Trie::erase(std::string word) {
    TrieNode * curr = this->ptr->root;
    TrieNode * pivotNode = curr;
    size_t pivotIdx = 0;

    for (size_t i = 0; i < word.size(); i++) {
        int idx = word[i];

        if (curr->children[idx] == NULL) return;

        int numChildren = getNumChildren(curr);
        if (numChildren > 1) {
            pivotNode = curr;
            pivotIdx = i;
        }

        curr = curr->children[idx];
    }

    if (!curr->terminal) return;

    curr->terminal = false;
    int numChildren = getNumChildren(curr);
    if (numChildren > 0) return;

    int idx = word[pivotIdx];
    if (!isSingularBranch(pivotNode->children[idx])) return;

    cleanup(pivotNode->children[idx]);
    pivotNode->children[idx] = NULL;
}