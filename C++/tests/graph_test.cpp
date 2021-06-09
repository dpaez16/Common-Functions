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

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
