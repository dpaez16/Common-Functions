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

Trie::Trie() {
    this->ptr = new ClassVars;
    this->ptr->size = 0;
    this->ptr->root = createNewNode();
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