#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>

class Graph {
public:
    Graph(bool directed, bool weighted);
    Graph(const Graph &);
    Graph& operator=(const Graph &);
    Graph reverse();
    bool empty();
    std::unordered_set<std::string> getVertices();
    std::unordered_map<std::string, std::unordered_set<std::string>> getEdges();
    bool isAdjacent(std::string a, std::string b);
    void flipEdge(std::string a, std::string b);
    std::unordered_set<std::string> getNeighbors(std::string v);
    std::unordered_set<std::string> getIncomingNeighbors(std::string v);
    bool isSourceVertex(std::string v);
    bool isSinkVertex(std::string v);
    bool hasVertex(std::string v);
    void addVertex(std::string v);
    void addEdge(std::string a, std::string b, float edgeValue);
    void removeVertex(std::string v);
    void removeEdge(std::string a, std::string b);
    float getEdgeValue(std::string a, std::string b);
    void setEdgeValue(std::string a, std::string b, float edgeValue);
    std::vector<std::string> dfs(std::string v);
    std::vector<std::string> bfs(std::string v);
    // std::pair<std::unordered_map<std::string, float>, std::unordered_map<std::string, std::string>> dijkstra(std::string v);
    std::pair<std::unordered_map<std::string, float>, std::unordered_map<std::string, std::string>> bellmanFord(std::string v);
    std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> floydWarshall();
    // std::unordered_set<std::pair<std::string, std::string>> mst();
    // std::vector<std::string> topologicalSort();
    // std::vector<std::unordered_set<std::string>> stronglyConnectedComponents();
    ~Graph();

private:
    struct ClassVars;
    ClassVars * ptr;
};

#endif