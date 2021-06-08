#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

class Trie {
public:
    Trie();
    Trie(const Trie &);
    Trie& operator=(const Trie &);
    void insert(std::string word);
    void erase(std::string word);
    std::vector<std::string> query(std::string prefix);
    size_t size();
    bool empty();
    ~Trie();

private:
    struct ClassVars;
    ClassVars * ptr;
};

#endif