#include <bits/stdc++.h>
#include "fibheap.h"
#include "graph.h"

using namespace std;

int main() {
    Graph g(true, false);

    g.addEdge("0", "1");
    g.addEdge("1", "2");
    g.addEdge("2", "3");
    g.addEdge("3", "0");

    g.addEdge("2", "4");

    g.addEdge("4", "5");
    g.addEdge("5", "6");
    g.addEdge("6", "4");
    
    g.addEdge("6", "7");
    
    vector<unordered_set<string>> scc = g.stronglyConnectedComponents();

    for (size_t i = 0; i < scc.size(); i++) {
        cout << i << ':' << endl;
        unordered_set<string> vertices = scc[i];
        for (string v : vertices) {
            cout << v << endl;
        }

        cout << endl;
    }

    return 0;
}