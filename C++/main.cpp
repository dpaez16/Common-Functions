#include <bits/stdc++.h>
#include "fibheap.h"
#include "graph.h"
#include "trie.h"

using namespace std;

int main() {
    Graph g(true, true);
    g.addEdge("a", "b", 4);
    g.addEdge("a", "c", 2);

    g.addEdge("b", "c", 3);
    g.addEdge("b", "d", 2);
    g.addEdge("b", "e", 3);

    g.addEdge("c", "b", 1);
    g.addEdge("c", "d", 4);
    g.addEdge("c", "e", 5);

    g.addEdge("e", "d", 1);

    pair<unordered_map<string, float>, unordered_map<string, string>> p = g.dijkstra("a");
    unordered_map<string, float> dist = p.first;
    vector<string> vertices = {"a", "b", "c", "d", "e"};

    for (string v : vertices) {
        cout << "a -> " << v << ": " << dist[v] << endl;
    }

    return 0;
}