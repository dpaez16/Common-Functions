#include <bits/stdc++.h>
#include <gtest/gtest.h>
#include "disjoint_set.h"

using namespace std;

TEST(DisjointSet, foo) {
    vector<string> v = {"a", "b", "c", "d"};
	DisjointSet d(v);

    d.setUnion(v[0], v[1]);
    d.setUnion(v[2], v[3]);
    d.setUnion(v[0], v[2]);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
