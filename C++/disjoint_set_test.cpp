#include <bits/stdc++.h>
#include <gtest/gtest.h>
#include "disjoint_set.h"
#include "graph.h"

using namespace std;

struct edge_hash {
    size_t operator() (const pair<string, string> & e) const {
		std::ostringstream out;
		out << e.first << ' ' << e.second;
		return std::hash<std::string>()(out.str());
	}
};

bool sortFunc(
    const pair<pair<string, string>, float> & a, 
    const pair<pair<string, string>, float> & b
) {
    if (a.second == b.second) {
        return a < b;
    }

    return a.second < b.second;
}

pair<string, string> getMSTEdge(pair<string, string> & e) {
    if (e.first > e.second) {
        return {e.second, e.first};
    }

    return e;
}

TEST(DisjointSet, KruskalIntegrationTest) {
    vector<string> vertices = {"1", "2", "3", "4", "5", "6"};
    Graph g(false, true);

    g.addEdge(vertices[0], vertices[1], 2);
    g.addEdge(vertices[1], vertices[2], 3);
    g.addEdge(vertices[2], vertices[3], 5);
    g.addEdge(vertices[3], vertices[0], 1);
    g.addEdge(vertices[1], vertices[3], 3);
    g.addEdge(vertices[0], vertices[4], 4);
    g.addEdge(vertices[3], vertices[4], 9);
    g.addEdge(vertices[1], vertices[5], 7);
    g.addEdge(vertices[2], vertices[5], 8);

    vector<pair<pair<string, string>, float>> edges;
    for (string u : vertices) {
        unordered_set<string> neighbors = g.getNeighbors(u);
        for (string v : neighbors) {
            float edgeWeight = g.getEdgeValue(u, v);
            edges.push_back({{u, v}, edgeWeight});
        }
    }

    sort(edges.begin(), edges.end(), sortFunc);
    DisjointSet dset(vertices);
    ASSERT_EQ(dset.size(), vertices.size());

    float cost = 0;
    unordered_set<pair<string, string>, edge_hash> mst;
    for (pair<pair<string, string>, float> edge : edges) {
        pair<string, string> e = edge.first;
        string u = e.first;
        string v = e.second;
        float edgeWeight = edge.second;

        if (dset.find(u) != dset.find(v)) {
            cost += edgeWeight;
            mst.insert(getMSTEdge(e));
            dset.setUnion(u, v);
        }
    }

    ASSERT_EQ(mst.size(), 5);
    ASSERT_TRUE(mst.find({vertices[0], vertices[1]}) != mst.end());
    ASSERT_TRUE(mst.find({vertices[1], vertices[2]}) != mst.end());
    ASSERT_TRUE(mst.find({vertices[1], vertices[5]}) != mst.end());
    ASSERT_TRUE(mst.find({vertices[0], vertices[3]}) != mst.end());
    ASSERT_TRUE(mst.find({vertices[0], vertices[4]}) != mst.end());

    ASSERT_EQ(cost, 17);
}

TEST(DisjointSet, unionFindTest) {
    DisjointSet dset;
    dset.insert("a");
    dset.insert("b");
    dset.insert("c");
    dset.insert("d");

    dset.setUnion("a", "b");
    dset.setUnion("a", "c");
    dset.setUnion("d", "a");

    ASSERT_EQ(dset.find("a"), "a");
    ASSERT_EQ(dset.find("b"), "a");
    ASSERT_EQ(dset.find("c"), "a");
    ASSERT_EQ(dset.find("d"), "a");
}

TEST(DisjointSet, CopyConstructorTest) {
    DisjointSet dset;
    ASSERT_TRUE(dset.empty());

    dset.insert("a");
    dset.insert("b");
    dset.insert("c");
    dset.insert("d");
    ASSERT_EQ(dset.size(), 4);

    DisjointSet dsetClone(dset);
    ASSERT_EQ(dset.size(), dsetClone.size());

    dset.setUnion("a", "b");
    dset.setUnion("a", "c");

    dsetClone.setUnion("c", "d");

    ASSERT_EQ(dset.find("a"), "a");
    ASSERT_EQ(dset.find("b"), "a");
    ASSERT_EQ(dset.find("c"), "a");
    ASSERT_EQ(dset.find("d"), "d");

    ASSERT_EQ(dsetClone.find("a"), "a");
    ASSERT_EQ(dsetClone.find("b"), "b");
    ASSERT_EQ(dsetClone.find("c"), "c");
    ASSERT_EQ(dsetClone.find("d"), "c");
}

TEST(DisjointSet, AssignmentOpTest) {
    DisjointSet dset;
    
    dset.insert("a");
    dset.insert("b");
    dset.insert("c");
    dset.insert("d");
    ASSERT_EQ(dset.size(), 4);

    DisjointSet dset2;
    dset2 = dset;
    ASSERT_EQ(dset.size(), dset2.size());

    dset.setUnion("a", "b");
    dset.setUnion("a", "c");

    dset2.setUnion("c", "d");

    ASSERT_EQ(dset.find("a"), "a");
    ASSERT_EQ(dset.find("b"), "a");
    ASSERT_EQ(dset.find("c"), "a");
    ASSERT_EQ(dset.find("d"), "d");

    ASSERT_EQ(dset2.find("a"), "a");
    ASSERT_EQ(dset2.find("b"), "b");
    ASSERT_EQ(dset2.find("c"), "c");
    ASSERT_EQ(dset2.find("d"), "c");
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
