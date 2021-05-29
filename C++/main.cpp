#include <bits/stdc++.h>
#include "graph.h"

using namespace std;

int main() {
    Graph g(true, false);
    g.addEdge("0", "1");
    g.addEdge("0", "2");
    
    g.addEdge("1", "2");
    g.addEdge("1", "5");

    g.addEdge("2", "3");

    g.addEdge("5", "3");
    g.addEdge("5", "4");

    g.addEdge("6", "1");
    g.addEdge("6", "5");
    
    vector<string> tps = g.topologicalSort();

    for (string v : tps) {
        cout << v << endl;
    }

    return 0;
}