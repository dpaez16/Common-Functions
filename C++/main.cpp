#include <bits/stdc++.h>
#include "fibheap.h"
#include "graph.h"
#include "trie.h"

using namespace std;

int main() {
    FibonacciHeap heap = FibonacciHeap(false);
    heap.push("a", 1);
    heap.push("b", 2);
    heap.push("c", 10);
    heap.push("d", -10);
    
    cout << heap.top() << endl;
    heap.pop();
    cout << heap.top() << endl;

    Trie t;
    t.insert("abc");
    t.insert("abcd");
    t.insert("abce");
    t.insert("abcf");
    t.erase("abce");

    Trie t2(t);
    Trie t3(t2);
    t3 = t;

    return 0;
}