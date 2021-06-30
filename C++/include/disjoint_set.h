#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <string>
#include <vector>

class DisjointSet {
public:
    DisjointSet();
    DisjointSet(std::vector<std::string> & vec);
    //DisjointSet(const DisjointSet &);
    //DisjointSet& operator=(const DisjointSet &);
    void insert(std::string x);
    //void remove(std::string x); 
    std::string find(std::string x);
    bool contains(std::string x);
    void setUnion(std::string x, std::string y);
    size_t size();
    bool empty();
    ~DisjointSet();

private:
    struct ClassVars;
    ClassVars * ptr;
};

#endif