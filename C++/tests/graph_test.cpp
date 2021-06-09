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

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
