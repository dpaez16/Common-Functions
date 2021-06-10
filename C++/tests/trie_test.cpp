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

TEST(Trie, eraseTest) {
    Trie t;
    vector<string> words = {"cat", "cats", "char"};
    for (string word : words) {
        t.insert(word);
    }

    t.erase("cat");
    vector<string> query = t.query("cat");
    unordered_set<string> actualQuery = {"cats"};

    ASSERT_EQ(query.size(), actualQuery.size());
    for (string word : query) {
        ASSERT_TRUE(actualQuery.find(word) != actualQuery.end());
    }

    t.erase("cats");
    t.erase("char");
    ASSERT_TRUE(t.empty()) << t.size();
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
