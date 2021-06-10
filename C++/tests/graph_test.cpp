#include <bits/stdc++.h>
#include <gtest/gtest.h>
#include "graph.h"

using namespace std;

bool equalSets(const unordered_set<string> & lhs, const unordered_set<string> & rhs) {
    if (lhs.size() != rhs.size()) return false;

    for (string elem : lhs) {
        if (rhs.find(elem) == rhs.end()) return false;
    }

    return true;
}

TEST(Graph, DijkstraTest) {
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

    const pair<unordered_map<string, float>, unordered_map<string, string>> p = g.dijkstra("a");
    unordered_map<string, float> dist = p.first;
    unordered_map<string, string> prev = p.second;
    
    ASSERT_EQ(dist["a"], 0);
    ASSERT_EQ(dist["b"], 3);
    ASSERT_EQ(dist["c"], 2);
    ASSERT_EQ(dist["d"], 5);
    ASSERT_EQ(dist["e"], 6);

    ASSERT_TRUE(prev.find("a") == prev.end());
    ASSERT_EQ(prev["b"], "c");
    ASSERT_EQ(prev["c"], "a");
    ASSERT_EQ(prev["d"], "b");
    ASSERT_EQ(prev["e"], "b");
}

TEST(Graph, BellmanFordTest) {
    Graph g(true, true);

    g.addEdge("s", "e", 8);
    g.addEdge("s", "a", 10);

    g.addEdge("e", "d", 1);

    g.addEdge("d", "a", -4);
    g.addEdge("d", "c", -1);

    g.addEdge("c", "b", -2);

    g.addEdge("b", "a", 1);

    g.addEdge("a", "c", 2);

    const pair<unordered_map<string, float>, unordered_map<string, string>> p = g.bellmanFord("s");
    unordered_map<string, float> dist = p.first;
    unordered_map<string, string> prev = p.second;

    ASSERT_EQ(dist["s"], 0);
    ASSERT_EQ(dist["a"], 5);
    ASSERT_EQ(dist["b"], 5);
    ASSERT_EQ(dist["c"], 7);
    ASSERT_EQ(dist["d"], 9);
    ASSERT_EQ(dist["e"], 8);

    ASSERT_TRUE(prev.find("s") == prev.end());
    ASSERT_EQ(prev["a"], "d");
    ASSERT_EQ(prev["b"], "c");
    ASSERT_EQ(prev["c"], "a");
    ASSERT_EQ(prev["d"], "e");
    ASSERT_EQ(prev["e"], "s");
}

TEST(Graph, FloydWarshallTest) {
    Graph g(true, true);

    g.addEdge("0", "2", -2);

    g.addEdge("1", "0", 4);
    g.addEdge("1", "2", 3);

    g.addEdge("2", "3", 2);

    g.addEdge("3", "1", -1);

    const pair<vector<vector<float>>, vector<vector<int>>> p = g.floydWarshall();
    const vector<vector<float>> dist = p.first;
    const vector<vector<int>> next = p.second;

    const vector<vector<float>> actualDist = {
        {0, -1, -2, 0},
        {4, 0, 2, 4},
        {5, 1, 0, 2},
        {3, -1, 1, 0}
    };

    const vector<vector<int>> actualNext = {
        {0, 2, 2, 2},
        {0, 1, 0, 0},
        {3, 3, 2, 3},
        {1, 1, 1, 3}
    };

    const int n = dist.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ASSERT_EQ(dist[i][j], actualDist[i][j]);
            ASSERT_EQ(next[i][j], actualNext[i][j]);
        }
    }
}

TEST(Graph, mstTest) {
    Graph g(false, true);

    g.addEdge("a", "b", 2);
    g.addEdge("a", "c", 3);
    g.addEdge("a", "d", 3);

    g.addEdge("b", "c", 4);
    g.addEdge("b", "e", 3);

    g.addEdge("c", "d", 5);
    g.addEdge("c", "f", 6);
    g.addEdge("c", "e", 1);

    g.addEdge("d", "f", 7);
    
    g.addEdge("f", "e", 8);
    g.addEdge("f", "g", 9);

    const vector<pair<string, string>> tree = g.mst();
    const vector<pair<string, string>> actualTree = {
        {"a", "b"},
        {"a", "d"},
        {"c", "f"},
        {"b", "e"},
        {"c", "e"},
        {"f", "g"}
    };

    const int n = tree.size();
    for (int i = 0; i < n; i++) {
        const pair<string, string> edge = actualTree[i];
        bool found = false;

        for (int j = 0; j < n; j++) {
            if (tree[j] != edge) continue;

            found = true;
            break;
        }

        ASSERT_TRUE(found);
    }
}

TEST(Graph, topologicalSortNoCycleTest) {
    Graph g(true, false);

    g.addEdge("1", "2");
    g.addEdge("1", "3");

    g.addEdge("2", "3");
    g.addEdge("2", "4");

    g.addEdge("3", "4");
    g.addEdge("3", "5");

    const vector<string> tps = g.topologicalSort();
    const vector<string> actualTPS = {"1", "2", "3", "4", "5"};
    const int n = tps.size();

    for (int i = 0; i < n; i++) {
        ASSERT_EQ(tps[i], actualTPS[i]);
    }
}

TEST(Graph, topologicalSortCycleTest) {
    Graph g(true, false);

    g.addEdge("1", "2");
    g.addEdge("2", "3");
    g.addEdge("3", "1");
    const vector<string> tps = g.topologicalSort();

    ASSERT_EQ(tps.size(), 0);
}

TEST(Graph, stronglyConnectedComponentsTest) {
    Graph g(true, false);

    g.addEdge("0", "2");
    g.addEdge("2", "1");
    g.addEdge("1", "0");

    g.addEdge("0", "3");
    g.addEdge("3", "4");

    const vector<unordered_set<string>> scc = g.stronglyConnectedComponents();
    const vector<unordered_set<string>> actualSCC = {{"0", "1", "2"}, {"3"}, {"4"}};

    ASSERT_EQ(scc.size(), actualSCC.size());

    const int n = actualSCC.size();
    for (int i = 0; i < n; i++) {
        const unordered_set<string> component = scc[i];
        bool found = false;

        for (int j = 0; j < n; j++) {
            if (!equalSets(component, actualSCC[j])) continue;

            found = true;
            break;
        }

        ASSERT_TRUE(found);
    }
}

TEST(Graph, dfsTest) {
    Graph g(true, false);

    g.addEdge("0", "1");
    g.addEdge("0", "2");
    g.addEdge("0", "3");

    g.addEdge("1", "4");
    g.addEdge("4", "5");
    g.addEdge("5", "6");

    const vector<string> dfs = g.dfs("0");
    const vector<string> actualDFS = {"0", "1", "4", "5", "6", "3", "2"};

    ASSERT_EQ(dfs.size(), actualDFS.size());

    const int n = dfs.size();
    for (int i = 0; i < n; i++) {
        ASSERT_EQ(dfs[i], actualDFS[i]);
    }
}

TEST(Graph, bfsTest) {
    Graph g(true, false);

    g.addEdge("0", "1");
    g.addEdge("0", "2");
    g.addEdge("0", "3");

    g.addEdge("1", "4");
    g.addEdge("4", "5");
    g.addEdge("5", "6");

    const vector<string> bfs = g.bfs("0");
    const vector<string> actualBFS = {"0", "2", "3", "1", "4", "5", "6"};

    ASSERT_EQ(bfs.size(), actualBFS.size());

    const int n = bfs.size();
    for (int i = 0; i < n; i++) {
        ASSERT_EQ(bfs[i], actualBFS[i]);
    }
}

TEST(Graph, sinkSourceVertexTest) {
    Graph g(true, false);

    g.addEdge("a", "b");
    g.addEdge("a", "c");

    ASSERT_TRUE(g.isSourceVertex("a"));
    ASSERT_TRUE(g.isSinkVertex("b"));
    ASSERT_TRUE(g.isSinkVertex("c"));
}

TEST(Graph, assignmentOpTest) {
    Graph g(true, true);

    g.addEdge("a", "b", 10);
    g.addEdge("a", "c", 1);
    g.addEdge("b", "a", -1);
    g.addEdge("b", "c", 2);

    Graph gClone(false, false);
    gClone = g;

    gClone.removeEdge("b", "c");

    ASSERT_TRUE(g.isAdjacent("b", "c"));
    ASSERT_FALSE(gClone.isAdjacent("b", "c"));
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
