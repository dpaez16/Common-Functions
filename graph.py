from node import Node
import random


class Graph:
    """
    Attributes:
        vertices: Set that holds all vertices.
        edges: Dictionary that associates a vertex with its connected neighbors.
        edge_values: Dictionary that associated an edge (a, b) with an edge value.
    """

    def __init__(self, directed=False, weighted=False):
        """
        Default constructor that initializes the graph
        """
        self.vertices = set([])
        self.edges = {}
        self.edge_values = {}
        self.directed = directed
        self.weighted = weighted

    def duplicate(self):
        """
        Creates a copy of the graph.
        """

        new_G = Graph(self.directed, self.weighted)
        new_G.vertices = self.vertices
        new_G.edges = self.edges
        new_G.edge_values = self.edge_values
        
        return new_G

    def reverse(self):
        """
        Flips all edges in the graph.

        :return: The reversed graph.
        """

        rev_G = self.duplicate()
        if not rev_G.directed:
            return rev_G

        edges = rev_G.get_edges()
        for v, neighbors in edges.items():
            for neighbor in neighbors:
                rev_G.flip_edge(v, neighbor)
        
        return rev_G

    def empty(self):
        """
        Returns true if the graph has no vertices (i.e. empty).

        :return: Boolean value indiciating whether the graph is empty.
        """

        return len(self.vertices) == 0

    def get_vertices(self):
        """
        Grabs the set of vertices of the graph.

        :return: Set of vertices stored internally.
        """
        return list(self.vertices)

    def get_edges(self):
        """
        Grabs all edges of the graph.

        :return: Edge dictionary stored internally.
        """
        return self.edges

    def is_adjacent(self, a, b):
        """
        Checks to see if two vertices are connected in the graph.
        Assumption(s):
            Both vertices (a, b) exist in the graph.

        :param a: Node to be checked.
        :param b: Other node to be checked.
        :return: True if there is an edge that connects a & b together
        """
        assert self.has_vertex(a) and self.has_vertex(b)

        edges = [(a, b)]
        if not self.directed:
            edges.append((b, a))

        for edge in edges:
            start_vertex, end_vertex = edge
            neighbors = self.get_neighbors(start_vertex)
            if end_vertex in neighbors:
                return True
        return False

    def flip_edge(self, a, b):
        """
        Flips the edge (a, b) in the graph.
        Assumption(s):
            Both vertices (a, b) are in the graph.
            The edge (a, b) exists in the graph.

        :param a: First vertex of the edge.
        :param b: Second vertex of the edge.
        """

        assert self.has_vertex(a) and self.has_vertex(b)
        assert self.is_adjacent(a, b)

        if not self.directed():
            return

        edge_val = self.get_edge_value(a, b)
        self.remove_edge(a, b)
        self.add_edge(b, a, edge_val)

        return

    def get_neighbors(self, vertex):
        """
        Grabs the neighboring vertices of a vertex.

        :param vertex: Vertex to be checked for neighbors.
        :return: Set of neighboring vertices.
        """
        assert self.has_vertex(vertex)

        return self.edges[vertex]

    def get_incoming_vertices(self, vertex):
        """
        Grabs the verticies that connect to the vertex.
        i.e. (a, vertex) for all vertices a
        Assumption(s):
            "vertex" exists in the graph.

        :param vertex: Node to be checked.
        :return: A list of all incoming vertices.
        """
        V = set(self.get_vertices())
        V = V.difference([vertex])
        incoming_vertices = map(lambda neighbor: (neighbor, self.is_adjacent(neighbor, vertex)), V)
        incoming_vertices = filter(lambda x: x[1], incoming_vertices)
        incoming_vertices = map(lambda x: x[0], incoming_vertices)

        return list(incoming_vertices)

    def has_vertex(self, vertex):
        """
        Checks to see if a vertex is in the graph.

        :param vertex: Vertex to be checked for existence.
        :return: True if the vertex is in the graph.
        """
        return vertex in self.vertices

    def add_vertex(self, vertex):
        """
        Adds a vertex to the graph.

        :param vertex: Vertex to be checked.
        """
        if self.has_vertex(vertex):
            return

        self.vertices.add(vertex)
        self.edges[vertex] = set([])

    def add_edge(self, a, b, edge_value=None):
        """
        Adds an (un)directed edge (a, b) to the graph with an edge weight.

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        :param edge_value: Value to associate (a, b) with.
        """
        for vertex in [a, b]:
            self.add_vertex(vertex)

        self.edges[a].add(b)

        if not self.directed:
            self.edges[b].add(a)

        if self.weighted:
            self.set_edge_value(a, b, edge_value)
            if not self.directed:
                self.set_edge_value(b, a, edge_value)

    def remove_vertex(self, vertex):
        """
        Removes an existing vertex from the graph.

        :param vertex: Vertex to be removed.
        """
        assert self.has_vertex(vertex)

        neighbors = self.get_neighbors(vertex)
        self.vertices = self.vertices.difference(set([vertex]))
        del self.edges[vertex]

        for neighbor in neighbors:
            del self.edge_values[(vertex, neighbor)]
            del self.edge_values[(neighbor, vertex)]

            self.edges[neighbor] = self.edges[neighbor].difference(set([vertex]))

    def remove_edge(self, a, b):
        """
        Removes an existing edge from the graph.

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        """
        assert self.has_vertex(a) and self.has_vertex(b)
        assert self.is_adjacent(a, b)

        self.edges[a] = self.edges[a].difference(set([b]))
        self.edges[b] = self.edges[b].difference(set([a]))

        if (a, b) in self.edge_values:
            del self.edge_values[(a, b)]
        if (b, a) in self.edge_values:
            del self.edge_values[(b, a)]

    def get_edge_value(self, a, b):
        """
        Gets the edge value associated with edge (a, b) from the graph.

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        :return: The associated edge value.
        """
        assert self.has_vertex(a) and self.has_vertex(b)
        assert self.is_adjacent(a, b)

        return self.edge_values[(a, b)]

    def set_edge_value(self, a, b, edge_value):
        """
        Sets an edge value of edge (a, b).

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        :param edge_value: Edge value to associate (a, b) with.
        """
        assert self.has_vertex(a) and self.has_vertex(b)
        assert self.is_adjacent(a, b)
        assert self.weighted

        self.edge_values[(a, b)] = edge_value

        if not self.directed:
            self.edge_values[(b, a)] = edge_value

    def dfs(self, v):
        """
        Performs a depth-first-search of the graph starting with
        the vertex v. Assumes that v is in the graph.

        :param  v: Starting vertex of the graph.
        :return:   A list of the verticies traveled.
        """

        assert self.has_vertex(v)

        vertices_traversed = []

        vertices = self.get_vertices()
        for vertex in [v] + vertices:
            vertex.set_data("visited", False)

        stack = [v]
        while len(stack) != 0:
            curr = stack[0]
            stack = stack[1:]

            if curr.get_data()["visited"]:
                continue

            curr.set_data("visited", True)
            vertices_traversed.append(curr)
            neighbors = list(self.get_neighbors(curr))
            random.shuffle(neighbors)

            for neighbor in neighbors:
                stack = [neighbor] + stack

        return vertices_traversed

    def bfs(self, v):
        """
        Performs a breadth-first-search of the graph starting with
        the vertex v. Assumes that v is in the graph.

        :param  v: Starting vertex of the graph.
        :return:   A list of the verticies traveled.
        """

        assert self.has_vertex(v)

        vertices_traversed = []

        vertices = self.get_vertices()
        for vertex in [v] + vertices:
            vertex.set_data("visited", False)

        queue = [v]
        while len(queue) != 0:
            curr = queue[0]
            queue = queue[1:]

            if curr.get_data()["visited"]:
                continue

            curr.set_data("visited", True)
            vertices_traversed.append(curr)
            neighbors = list(self.get_neighbors(curr))
            random.shuffle(neighbors)

            for neighbor in neighbors:
                queue.append(neighbor)

        return vertices_traversed

    def dijkstra(self, source):
        """
        Computes Dijkstra's algorithm (shortest path from 
        a source vertex to all other verticies). Assumes
        "source" is in the graph.

        :param source: The vertex to perform Dijkstra's on.
        :return Two dictionaries:
            -A dictionary containing the cost to traverse from "source" to each
            reachable vertex in the graph.
            -A dictionary encoding the shortest path from "source" to each
            reachable vertex in the graph.
        """

        Q, V = set([source]), self.get_vertices()
        dist, prev = {}, {}

        assert source in V

        for v in V:
            dist[v] = 1e99
            prev[v] = None
            Q.add(v)
        dist[source] = 0

        while len(Q) != 0:
            u = min(Q, key=lambda q: dist[q])

            Q = Q.difference(set([u]))

            neighbors = self.get_neighbors(u)
            neighbors = list(filter(lambda x: x in Q, neighbors))
            for neighbor in neighbors:
                alt = dist[u] + self.get_edge_value(u, neighbor)
                if alt < dist[neighbor]:
                    dist[neighbor] = alt
                    prev[neighbor] = u

        return dist, prev

    def mst(self):
        """
        Computes the minimum spanning tree of the graph using
        Prim's algorithm.

        :return: The minimum spanning tree (in a list of connecting edges).
        """

        V = self.get_vertices()
        random.shuffle(V)
        visited = set([V[0]])
        V = set(V).difference(set([V[0]]))
        mst = []

        while len(V) != 0:
            possible_edges = []
            for visited_node in visited:
                neighbors = self.get_neighbors(visited_node)
                neighbors = list(filter(lambda x: x in V, neighbors))
                for neighbor in neighbors:
                    possible_edges.append((visited_node, neighbor))

            if len(possible_edges) == 0:
                continue
            
            min_edge = min(possible_edges, key=lambda x: self.get_edge_value(x[0], x[1]))
            mst.append(min_edge)
            V = V.difference(set([min_edge[1]]))
            visited.add(min_edge[1])

        return mst

    def topological_sort(self):
        """
        Computes the topological sort of the graph.
        Will return None if no such sorting exists (i.e. there are cycles).
        Assumption(s):
            The graph is a directed and acyclic.
        
        :return: A list of the vertices sorted in topological order, if it exists.
        """

        assert self.directed
        new_G = self.duplicate()

        L = []
        
        V = set(new_G.get_vertices())
        S = set([])
        for v in V:
            incoming_neighbors = new_G.get_incoming_vertices(v)
            if len(incoming_neighbors) == 0:
                S.add(v)

        while len(S) != 0:
            n = S.pop()
            L.append(n)

            neighbors = new_G.get_neighbors(n)
            for neighbor in neighbors:
                new_G.remove_edge(n, neighbor)
                incoming_vertices = new_G.get_incoming_vertices(neighbor)
                if len(incoming_vertices) == 0:
                    S.add(neighbor)
    
        edges = new_G.get_edges()
        for v, neighbors in edges.items():
            if len(neighbors) != 0:
                return None
        
        return L

    def strong_components(self):
        """
        Computes the graph's strongly connected components.
        Assumption(s):
            Graph is directed.

        :return: None
        """
        
        assert self.directed

        return None
