#include <bits/stdc++.h>
#include "graph.h"

using namespace std;

int main() {
    Graph g(true, false);
    g.addEdge("a", "b", 0);
    g.addEdge("b", "c", 0);

    unordered_set<string> incomingNeighbors = g.getIncomingNeighbors("b");
    for (string v : incomingNeighbors) {
        cout << v << endl;
    }

    cout << endl;

    unordered_set<string> vertices = g.getVertices();
    for (string v : vertices) {
        cout << v << endl;
    }

    return 0;
}