#include "graph.h"

#include <iostream>
#include <functional>
#include <sstream>
#include <stack>
#include <queue>
#include <assert.h>

typedef std::string vertex;
typedef std::pair<vertex, vertex> edge;
typedef std::vector<vertex> vertex_list;
typedef std::unordered_set<vertex> vertex_set;

struct edge_hash {
    size_t operator() (const edge & e) const {
		std::ostringstream out;
		out << e.first << ' ' << e.second;
		return std::hash<std::string>()(out.str());
	}
};

struct Graph::ClassVars {
    bool directed;
    bool weighted;
    vertex_set vertices;
    std::unordered_map<vertex, vertex_set> neighborsMap;
    std::unordered_map<edge, float, edge_hash> edgeValueMap;    
};

Graph::Graph(bool directed, bool weighted) {
    this->ptr = new ClassVars;
    this->ptr->directed = directed;
    this->ptr->weighted = weighted;
}

Graph::Graph(const Graph & other) {
    this->ptr = new ClassVars;
    this->ptr->directed = other.ptr->directed;
    this->ptr->weighted = other.ptr->weighted;
    this->ptr->vertices = vertex_set(other.ptr->vertices);
    this->ptr->neighborsMap = std::unordered_map<vertex, vertex_set>(other.ptr->neighborsMap);
    this->ptr->edgeValueMap = std::unordered_map<edge, float, edge_hash>(other.ptr->edgeValueMap);
}

Graph & Graph::operator=(const Graph & other) {
    this->ptr->directed = other.ptr->directed;
    this->ptr->weighted = other.ptr->weighted;
    this->ptr->vertices = vertex_set(other.ptr->vertices);
    this->ptr->neighborsMap = std::unordered_map<vertex, vertex_set>(other.ptr->neighborsMap);
    this->ptr->edgeValueMap = std::unordered_map<edge, float, edge_hash>(other.ptr->edgeValueMap);

    return *this;
}

Graph::~Graph() {
    delete this->ptr;
}

bool Graph::empty() {
    return this->ptr->vertices.size() == 0;
}

vertex_set Graph::getVertices() {
    return this->ptr->vertices;
}
std::unordered_map<vertex, vertex_set> Graph::getEdges() {
    return this->ptr->neighborsMap;
}

bool Graph::isAdjacent(vertex a, vertex b) {
    if (!hasVertex(a) || !hasVertex(b)) return false;

    return this->ptr->neighborsMap[a].find(b) != this->ptr->neighborsMap[a].end();
}

bool Graph::hasVertex(vertex v) {
    return this->ptr->vertices.find(v) != this->ptr->vertices.end();
}

void Graph::flipEdge(vertex a, vertex b) {
    if (!this->ptr->directed || !isAdjacent(a, b)) return;

    float edgeValue = this->ptr->weighted ? getEdgeValue(a, b) : 0;
    removeEdge(a, b);
    addEdge(a, b, edgeValue);
}

void Graph::addVertex(vertex v) {
    if (hasVertex(v)) return;

    this->ptr->vertices.insert(v);
    this->ptr->neighborsMap[v] = vertex_set();
}

void Graph::setEdgeValue(vertex a, vertex b, float edgeValue) {
    assert(this->ptr->weighted);
    assert(isAdjacent(a, b));

    this->ptr->edgeValueMap[edge(a, b)] = edgeValue;
    if (!this->ptr->directed) this->ptr->edgeValueMap[edge(b, a)] = edgeValue;
}

void Graph::addEdge(vertex a, vertex b, float edgeValue = 0) {
    addVertex(a);
    addVertex(b);

    this->ptr->neighborsMap[a].insert(b);
    if (!this->ptr->directed) this->ptr->neighborsMap[b].insert(a);

    if (!this->ptr->weighted) return;

    setEdgeValue(a, b, edgeValue);
}

void Graph::removeEdge(vertex a, vertex b) {
    if (!isAdjacent(a, b)) return;

    this->ptr->neighborsMap[a].erase(b);
    this->ptr->edgeValueMap.erase(edge(a, b));

    if (!this->ptr->directed) {
        this->ptr->neighborsMap[b].erase(a);
        this->ptr->edgeValueMap.erase(edge(b, a));
    }
}

float Graph::getEdgeValue(vertex a, vertex b) {
    assert(this->ptr->weighted);
    assert(isAdjacent(a, b));

    return this->ptr->edgeValueMap[edge(a, b)];
}

vertex_set Graph::getNeighbors(vertex v) {
    assert(hasVertex(v));
    return this->ptr->neighborsMap[v];
}

vertex_set Graph::getIncomingNeighbors(vertex v) {
    vertex_set vertices = getVertices();
    
    for (vertex u : vertices) {
        if (isAdjacent(u, v)) continue;

        vertices.erase(u);
    }

    return vertices;
}

bool Graph::isSourceVertex(vertex v) {
    assert(hasVertex(v));
    return getIncomingNeighbors(v).size() == 0;
}

bool Graph::isSinkVertex(vertex v) {
    assert(hasVertex(v));
    return getNeighbors(v).size() == 0;
}

void Graph::removeVertex(vertex v) {
    if (!hasVertex(v)) return;

    this->ptr->vertices.erase(v);
    vertex_set incomingNeighbors = getIncomingNeighbors(v);
    for (vertex u : incomingNeighbors) {
        removeEdge(u, v);
    }

    vertex_set neighbors = getNeighbors(v);
    for (vertex u : neighbors) {
        removeEdge(v, u);
    }
}

Graph Graph::reverse() {
    assert(!this->ptr->directed);

    Graph revG(this->ptr->directed, this->ptr->weighted);
    vertex_set vertices = getVertices();
    float edgeValue = 0;

    for (vertex u : vertices) {
        revG.addVertex(u);
        vertex_set neighbors = getNeighbors(u);
        for (vertex v : neighbors) {
            if (this->ptr->weighted) 
                edgeValue = getEdgeValue(u, v);

            revG.addEdge(v, u, edgeValue);
        }
    }

    return revG;
}

vertex_list Graph::dfs(vertex v) {
    vertex_list traversalList;
    std::stack<vertex> stk;
    vertex_set visited;
    stk.push(v);

    while (!stk.empty()) {
        vertex u = stk.top();
        stk.pop();

        if (visited.find(u) != visited.end())
            continue;

        visited.insert(u);
        traversalList.push_back(u);

        vertex_set neighbors = getNeighbors(u);
        for (vertex neighbor : neighbors) {
            stk.push(neighbor);
        }
    }

    return traversalList;
}

vertex_list Graph::bfs(vertex v) {
    vertex_list traversalList;
    std::queue<vertex> q;
    vertex_set visited;
    q.push(v);

    while (!q.empty()) {
        vertex u = q.front();
        q.pop();

        if (visited.find(u) != visited.end())
            continue;

        visited.insert(u);
        traversalList.push_back(u);

        vertex_set neighbors = getNeighbors(u);
        for (vertex neighbor : neighbors) {
            q.push(neighbor);
        }
    }

    return traversalList;
}
