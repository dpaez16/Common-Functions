#include "graph.h"

#include <iostream>
#include <functional>
#include <sstream>

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
