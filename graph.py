from node import Node
from common import INFINITY
from copy import deepcopy
from queue import Queue
from stack import Stack
import random


class Graph:
    """
    Interface for creating graphs.

    Attributes:
        vertices: Set that holds all vertices.
        edges: Dictionary that associates a vertex with its connected neighbors.
        edge_values: Dictionary that associated an edge (a, b) with an edge value.
    """

    def __init__(self, directed=False, weighted=False):
        """
        Default constructor that initializes the graph.
        """
        self._vertices = set([])
        self._edges = {}
        self._edge_values = {}
        self._directed = directed
        self._weighted = weighted

    def duplicate(self):
        """
        Creates a copy of the graph.

        :return: The duplicated graph.
        """

        new_G = Graph(self._directed, self._weighted)
        if self.empty():
            return new_G

        V = deepcopy(self.get_vertices())
        visited = set([])

        for v in V:
            queue = Queue()
            queue.push(v)
            while not queue.empty():
                curr = queue.pop()
                if curr in visited:
                    continue

                visited.add(curr)
                neighbors = self.get_neighbors(curr)
                for neighbor in neighbors:
                    queue.push(neighbor)
                    edge_value = None if not self._weighted else self._edge_values[(curr, neighbor)]
                    new_G.add_edge(curr, neighbor, edge_value)

                if len(neighbors) == 0:
                    new_G.add_vertex(curr)

        return new_G

    def reverse(self):
        """
        Flips all edges in the graph.

        :return: The reversed graph.
        """

        if not self._directed:
            return self.duplicate()

        rev_G = Graph(self._directed, self._weighted)
        V = deepcopy(self.get_vertices())
        visited = set([])

        for v in V:
            queue = Queue()
            queue.push(v)
            while not queue.empty():
                curr = queue.pop()
                if curr in visited:
                    continue

                visited.add(curr)
                neighbors = self.get_neighbors(curr)
                for neighbor in neighbors:
                    queue.push(neighbor)
                    edge_value = None if not self._weighted else self._edge_values[(curr, neighbor)]
                    rev_G.add_edge(neighbor, curr, edge_value)

                if len(neighbors) == 0:
                    rev_G.add_vertex(curr)
        
        return rev_G

    def empty(self):
        """
        Returns true if the graph has no vertices (i.e. empty).

        :return: Boolean value indiciating whether the graph is empty.
        """

        return len(self._vertices) == 0

    def get_vertices(self):
        """
        Grabs the set of vertices of the graph.

        :return: Set of vertices stored internally.
        """
        return list(self._vertices)

    def get_edges(self):
        """
        Grabs all edges of the graph.

        :return: Edge dictionary stored internally.
        """
        return self._edges

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
        if not self._directed:
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

        if not self._directed:
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

        return self._edges[vertex]

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
        return vertex in self._vertices

    def add_vertex(self, vertex):
        """
        Adds a vertex to the graph.

        :param vertex: Vertex to be checked.
        """
        if self.has_vertex(vertex):
            return

        self._vertices.add(vertex)
        self._edges[vertex] = set([])

    def add_edge(self, a, b, edge_value=None):
        """
        Adds an (un)directed edge (a, b) to the graph with an edge weight.

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        :param edge_value: Value to associate (a, b) with.
        """
        for vertex in [a, b]:
            self.add_vertex(vertex)

        self._edges[a].add(b)

        if not self._directed:
            self._edges[b].add(a)

        if self._weighted:
            self.set_edge_value(a, b, edge_value)
            if not self._directed:
                self.set_edge_value(b, a, edge_value)

    def remove_vertex(self, vertex):
        """
        Removes an existing vertex from the graph.

        :param vertex: Vertex to be removed.
        """
        assert self.has_vertex(vertex)

        incoming_neighbors = self.get_incoming_vertices(vertex)
        for neighbor in incoming_neighbors:
            self.remove_edge(neighbor, vertex)

        outgoing_neighbors = self.get_neighbors(vertex)
        for neighbor in outgoing_neighbors:
            self.remove_edge(vertex, neighbor)

        self._vertices = self._vertices.difference(set([vertex]))

    def remove_edge(self, a, b):
        """
        Removes an existing edge from the graph.

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        """
        assert self.has_vertex(a) and self.has_vertex(b)
        assert self.is_adjacent(a, b)

        self._edges[a] = self._edges[a].difference(set([b]))
        self._edges[b] = self._edges[b].difference(set([a]))

        if (a, b) in self._edge_values:
            del self._edge_values[(a, b)]
        if (b, a) in self._edge_values:
            del self._edge_values[(b, a)]

    def get_edge_value(self, a, b):
        """
        Gets the edge value associated with edge (a, b) from the graph.

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        :return: The associated edge value.
        """
        assert self.has_vertex(a) and self.has_vertex(b)
        assert self.is_adjacent(a, b)

        return self._edge_values[(a, b)]

    def set_edge_value(self, a, b, edge_value):
        """
        Sets an edge value of edge (a, b).

        :param a: Starting vertex of the edge.
        :param b: Ending vertex of the edge.
        :param edge_value: Edge value to associate (a, b) with.
        """
        assert self.has_vertex(a) and self.has_vertex(b)
        assert self.is_adjacent(a, b)
        assert self._weighted

        self._edge_values[(a, b)] = edge_value

        if not self._directed:
            self._edge_values[(b, a)] = edge_value

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
        visited = set([])

        stack = Stack()
        stack.push(v)
        while not stack.empty():
            curr = stack.pop()
            if curr in visited:
                continue

            visited.add(curr)
            vertices_traversed.append(curr)
            neighbors = list(self.get_neighbors(curr))
            random.shuffle(neighbors)

            for neighbor in neighbors:
                stack.push(neighbor)

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
        visited = set([])

        queue = Queue()
        queue.push(v)
        while not queue.empty():
            curr = queue.pop()
            if curr in visited:
                continue

            visited.add(curr)
            vertices_traversed.append(curr)
            neighbors = list(self.get_neighbors(curr))
            random.shuffle(neighbors)

            for neighbor in neighbors:
                queue.push(neighbor)

        return vertices_traversed

    def dijkstra(self, source):
        """
        Computes Dijkstra's algorithm (shortest path from 
        a source vertex to all other verticies). 
        
        Assumption(s):
            - "source" is in the graph.
            - The graph has no negative edge-weights.
            - The graph has no negative cycles.

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
            dist[v] = INFINITY
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

        def bellman_ford(self, source):
            """
            Computes the Bellman-Ford algorithm (shortest path from 
            a source vertex to all other verticies).

            Assumption(s):
                - "source" is in the graph.
                - The graph has no negative edge-weights.
                - The graph has no negative cycles.

            :param source: The vertex to perform the Bellman-Ford algorithm on.
            :return Two dictionaries:
                -A dictionary containing the cost to traverse from "source" to each
                reachable vertex in the graph.
                -A dictionary encoding the shortest path from "source" to each
                reachable vertex in the graph.
            """

            V, E = self.get_vertices(), self.get_edges()
            dist, prev = {}, {}

            assert source in V

            for v in V:
                dist[v] = INFINITY
                prev[v] = None
            dist[source] = 0

            for i in range(len(V) - 1):
                for v in E:
                    neighbors = self.get_neighbors(v)
                    for neighbor in neighbors:
                        alt = dist[v] + self.get_edge_value(v, neighbor)
                        if alt < dist[neighbor]:
                            dist[neighbor] = alt
                            prev[neighbor] = v

            return dist, prev

    def floyd_warshall(self):
        """
        Computes the shortest path for all possible pairs of vertices.
        Assumption(s):
            Graph is weighted and directed.

        :return: Dictionary that maps each pair of vertices to (shortest distance, path of vertices to take).
        """
        
        assert self._directed and self._weighted

        dist, next_vertex = {}, {}
        V = self.get_vertices()
        for i in range(len(V)):
            for j in range(i+1, len(V)):
                dist[(V[i], V[j])] = INFINITY
                dist[(V[j], V[i])] = INFINITY
                next_vertex[(V[i], V[j])] = None
                next_vertex[(V[j], V[i])] = None

        for u in V:
            neighbors = self.get_neighbors(u)
            for v in neighbors:
                dist[(u, v)] = self.get_edge_value(u, v)
                next_vertex[(u, v)] = v

        for v in V:
            dist[(v, v)] = 0
            next_vertex[(v, v)] = v

        for k in range(len(V)):
            for i in range(len(V)):
                for j in range(len(V)):
                    if dist[(V[i], V[j])] > dist[(V[i], V[k])] + dist[(V[k], V[j])]:
                        dist[(V[i], V[j])] = dist[(V[i], V[k])] + dist[(V[k], V[j])]
                        next_vertex[(V[i], V[j])] = next_vertex[(V[i], V[k])]

        def path(u, v):
            if next_vertex[(u, v)] is None:
                return []
            path = [u]
            while u != v:
                u = next_vertex[(u, v)]
                path.append(u)
            return path

        apsp = {}
        for i in range(len(V)):
            for j in range(i+1, len(V)):
                u, v = V[i], V[j]
                apsp[(u, v)] = (dist[(u, v)], path(u, v))
                apsp[(v, u)] = (dist[(v, u)], path(v, u))

        return apsp

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
                    edge_value = self.get_edge_value(visited_node, neighbor)
                    possible_edges.append((visited_node, neighbor, edge_value))

            if len(possible_edges) == 0:
                continue
            
            min_edge = min(possible_edges, key=lambda x: x[2])
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

        assert self._directed
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

    def strongly_connected_components(self):
        """
        Computes the graph's strongly connected components.
        Assumption(s):
            Graph is directed.

        :return: Dictionary containing each component.
        """
        
        assert self._directed
        
        rev_G = self.reverse()
        V = rev_G.get_vertices()
        L, components = [], {}
        visited = set([])

        if self.empty():
            return components
        
        def visit(v):
            if v in visited:
                return

            visited.add(v)
            neighbors = self.get_neighbors(v)
            for neighbor in neighbors:
                visit(neighbor)

            L.append(v)
            return
       
        for v in V:
            visit(v)
        
        for v in L:
            v.set_data("component", None)

        component_count = 0
        L = L[::-1]
        for u_ in L:
            stack = Stack()
            stack.push((u_, u_))
            while not stack.empty():
                root, u = stack.pop()
                if u.get_data()["component"] is None:
                    if u == root:
                        component_count += 1
                        u.set_data("component", component_count)
                    else:
                        u.set_data("component", root.get_data()["component"])
                    
                    neighbors = rev_G.get_neighbors(u)
                    for v in neighbors:
                        stack.push((root, v))
        
        for v in L:
            component = v.get_data()["component"]
            if component not in components:
                components[component] = set([])
            components[component].add(v)
            v.delete_data("component")

        return components
