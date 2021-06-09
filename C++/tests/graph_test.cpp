#include <bits/stdc++.h>
#include <gtest/gtest.h>
#include "graph.h"

using namespace std;

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

    pair<unordered_map<string, float>, unordered_map<string, string>> p = g.dijkstra("a");
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

    pair<unordered_map<string, float>, unordered_map<string, string>> p = g.bellmanFord("s");
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

    pair<vector<vector<float>>, vector<vector<int>>> p = g.floydWarshall();
    vector<vector<float>> dist = p.first;
    vector<vector<int>> next = p.second;

    vector<vector<float>> actualDist = {
        {0, -1, -2, 0},
        {4, 0, 2, 4},
        {5, 1, 0, 2},
        {3, -1, 1, 0}
    };

    vector<vector<int>> actualNext = {
        {0, 2, 2, 2},
        {0, 1, 0, 0},
        {3, 3, 2, 3},
        {1, 1, 1, 3}
    };

    int n = dist.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ASSERT_EQ(dist[i][j], actualDist[i][j]);
            ASSERT_EQ(next[i][j], actualNext[i][j]);
        }
    }
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
