#include <bits/stdc++.h>
#include <gtest/gtest.h>
#include "trie.h"

using namespace std;

TEST(Trie, InsertTest) {
	Trie t;
    vector<string> words = {"cat", "cats", "char"};
    for (string word : words) {
        t.insert(word);
    }

    vector<string> query = t.query("ca");
    unordered_set<string> actualQuery = {"cat", "cats"};

    ASSERT_EQ(query.size(), actualQuery.size());
    for (string word : query) {
        ASSERT_TRUE(actualQuery.find(word) != actualQuery.end());
    }

    query = t.query("d");
    ASSERT_EQ(query.size(), 0);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
