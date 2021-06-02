#include <functional>
#include <sstream>
#include <stack>
#include <queue>
#include <limits>
#include <algorithm>
#include <assert.h>

#include "fibheap.h"
#include "graph.h"

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
    if (this == &other) return *this;

    this->~Graph();

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

void Graph::addEdge(vertex a, vertex b, float edgeValue) {
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
    vertex_set incomingNeighbors;

    for (vertex u : vertices) {
        if (!isAdjacent(u, v)) continue;

        incomingNeighbors.insert(u);
    }
    
    return incomingNeighbors;
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
    assert(this->ptr->directed);

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

std::pair<
    std::unordered_map<vertex, float>, 
    std::unordered_map<vertex, vertex>
>
Graph::dijkstra(vertex v) {
    assert(hasVertex(v));
    assert(this->ptr->weighted);

    std::unordered_map<vertex, float> dist;
    std::unordered_map<vertex, vertex> prev;

    FibonacciHeap pq = FibonacciHeap();
    vertex_set vertices = getVertices();
    float inf = std::numeric_limits<float>::max();

    for (vertex u : vertices) {
        dist[u] = u != v ? inf : 0;
        pq.push(u, dist[u]);
    }

    while (!pq.empty()) {
        vertex u = pq.top();
        pq.pop();

        vertex_set neighbors = getNeighbors(u);
        for (vertex neighbor : neighbors) {
            float alt = dist[u] + getEdgeValue(u, neighbor);
            if (alt >= dist[neighbor]) continue;

            dist[neighbor] = alt;
            prev[neighbor] = u;

            if (!pq.contains(neighbor)) pq.push(neighbor, alt);
            else pq.decreaseKey(neighbor, alt);
        }
    }

    return std::pair<
        std::unordered_map<vertex, float>, 
        std::unordered_map<vertex, vertex>
    >(dist, prev);
}

std::pair<
    std::unordered_map<vertex, float>, 
    std::unordered_map<vertex, vertex>
>
Graph::bellmanFord(vertex v) {
    assert(hasVertex(v));
    assert(this->ptr->weighted);

    std::unordered_map<vertex, float> dist;
    std::unordered_map<vertex, vertex> prev;

    vertex_set vertices = getVertices();
    for (vertex u : vertices) {
        dist[u] = std::numeric_limits<float>::max();
    }
    dist[v] = 0;

    for (size_t it = 0; it < vertices.size() - 1; it++) {
        for (vertex u : vertices) {
            vertex_set neighbors = getNeighbors(u);
            for (vertex neighbor : neighbors) {
                float alt = dist[u] + getEdgeValue(u, neighbor);
                if (alt >= dist[neighbor]) continue; 

                dist[neighbor] = alt;
                prev[neighbor] = u;
            }
        }
    }

    return std::pair<
        std::unordered_map<vertex, float>, 
        std::unordered_map<vertex, vertex>
    >(dist, prev);
}

std::pair<
    std::vector<std::vector<float>>,
    std::vector<std::vector<int>>
>
Graph::floydWarshall() {
    assert(this->ptr->directed && this->ptr->weighted);
    
    vertex_set vertices = getVertices();
    vertex_list verticesList(vertices.begin(), vertices.end());
    std::sort(verticesList.begin(), verticesList.end());
    size_t n = vertices.size();

    float inf = std::numeric_limits<float>::max();
    std::vector<std::vector<float>> dist(n, std::vector<float>(n, inf));
    std::vector<std::vector<int>> next(n, std::vector<int>(n, -1));

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; i++) {
            vertex a = verticesList[i];
            vertex b = verticesList[j];

            if (!isAdjacent(a, b)) continue;
            
            dist[i][j] = getEdgeValue(a, b);
            next[i][j] = j;
        }

        dist[i][i] = 0;
        next[i][i] = i;
    }

    for (size_t k = 0; k < n; k++) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                float alt = dist[i][k] + dist[k][j];
                if (dist[i][j] <= alt) continue;

                dist[i][j] = alt;
                next[i][j] = next[i][k];
            }
        }
    }

    return std::pair<
        std::vector<std::vector<float>>,
        std::vector<std::vector<int>>
    >(dist, next);
}

std::vector<edge> Graph::mst() {
    assert(this->ptr->weighted);

    vertex_set vertices = getVertices();
    FibonacciHeap heap = FibonacciHeap();
    
    float inf = std::numeric_limits<float>::max();
    std::unordered_map<vertex, float> cost;
    std::unordered_map<vertex, vertex> prev;

    for (vertex v : vertices) {
        cost[v] = inf;
    }

    vertex v = *vertices.begin();
    cost[v] = 0;

    for (vertex u : vertices) {
        heap.push(u, cost[u]);
    }

    while (!heap.empty()) {
        vertex u = heap.top();
        heap.pop();

        vertex_set neighbors = getNeighbors(u);
        for (vertex v : neighbors) {
            float edgeWeight = getEdgeValue(u, v);
            if (heap.contains(v) && cost[v] > edgeWeight) {
                cost[v] = edgeWeight;
                prev[v] = u;
                heap.decreaseKey(v, cost[v]);
            }
        }
    }

    std::unordered_set<edge, edge_hash> mstSet;
    for (vertex v : vertices) {
        if (prev.find(v) == prev.end()) continue;

        edge e = prev[v] < v ? edge(prev[v], v) : edge(v, prev[v]);
        mstSet.insert(e);
    }

    return std::vector<edge>(mstSet.begin(), mstSet.end());
}

vertex_list Graph::topologicalSort() {
    assert(this->ptr->directed);

    Graph clone(*this);
    vertex_list tps;
    vertex_set vertices = getVertices();
    std::stack<vertex> stk;

    for (vertex v : vertices) {
        if (!isSourceVertex(v)) continue;

        stk.push(v);
    }

    while (!stk.empty()) {
        vertex sourceVertex = stk.top();
        stk.pop();
        tps.push_back(sourceVertex);

        vertex_set neighbors = clone.getNeighbors(sourceVertex);
        for (vertex neighbor : neighbors) {
            clone.removeEdge(sourceVertex, neighbor);
            if (!clone.isSourceVertex(neighbor)) 
                continue;

            stk.push(neighbor);
        }
    }

    for (vertex v : vertices) {
        if (clone.getNeighbors(v).size() == 0) continue;

        return {};
    }

    return tps;
}

void dfsHelper(Graph * g, vertex & v, vertex_list & L) {
    static vertex_set visited;

    std::stack<vertex> stk;
    std::stack<vertex> dfs;
    stk.push(v);

    while (!stk.empty()) {
        vertex u = stk.top();
        stk.pop();

        if (visited.find(u) != visited.end()) 
            continue;

        dfs.push(u);
        visited.insert(u);

        vertex_set neighbors = g->getNeighbors(u);
        for (vertex neighbor : neighbors) {
            stk.push(neighbor);
        }
    }

    while (!dfs.empty()) {
        L.push_back(dfs.top());
        dfs.pop();
    }
}

std::vector<vertex_set> Graph::stronglyConnectedComponents() {
    assert(this->ptr->directed);

    if (empty()) return {};

    Graph revG = reverse();
    vertex_set vertices = getVertices();
    vertex_list L;
    vertex_set visited;
    std::vector<vertex_set> components;

    for (vertex v : vertices)
        dfsHelper(this, v, L);

    std::unordered_map<vertex, size_t> componentMap;

    size_t componentCount = 0;
    for (auto it = L.rbegin(); it != L.rend(); it++) {
        vertex u = *it;
        std::stack<std::pair<vertex, vertex>> stk;
        stk.push({u, u});

        while (!stk.empty()) {
            std::pair<vertex, vertex> p = stk.top();
            vertex root = p.first;
            u = p.second;
            stk.pop();

            if (componentMap.find(u) != componentMap.end())
                continue;

            componentMap[u] = (u == root) ? componentCount++ : componentMap[root];

            vertex_set neighbors = revG.getNeighbors(u);
            for (vertex neighbor : neighbors) {
                stk.push({root, neighbor});
            }
        }
    }

    components.resize(componentCount);
    for (vertex v : L) {
        size_t component = componentMap[v];
        components[component].insert(v);
    }

    return components;
}
