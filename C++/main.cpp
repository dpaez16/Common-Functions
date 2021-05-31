#include <bits/stdc++.h>
#include "fibheap.h"
#include "graph.h"

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

    return 0;
}