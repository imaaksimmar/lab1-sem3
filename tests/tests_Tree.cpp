#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../include/TreeDictionary.hpp"


static TreeDictionary<int, int> MakeSample() {
    TreeDictionary<int, int> tree;
    for(int k : {8, 3, 10, 1, 6, 14}) { tree.Add(k, k); }
    return tree;
}

TEST(TreeBasic, EmptyTree) {
    TreeDictionary<int, int> tree;
    EXPECT_EQ(tree.GetCount(), 0u);
    EXPECT_FALSE(tree.ContainsKey(1));
    EXPECT_EQ(tree.Find(1), nullptr);
}

TEST(TreeBasic, AddAndGet) {
    TreeDictionary<int, int> tree = MakeSample();
    EXPECT_EQ(tree.GetCount(), 6u);
    EXPECT_EQ(tree.Get(6), 6);
}

TEST(TreeBasic, DuplicateKeyOverwritesValue) {
    TreeDictionary<int, int> tree;
    tree.Add(5, 100);
    tree.Add(5, 200);
    EXPECT_EQ(tree.GetCount(), 1u);
    EXPECT_EQ(tree.Get(5), 200);
}

TEST(TreeBasic, GetMissingThrows) {
    TreeDictionary<int, int> tree = MakeSample();
    EXPECT_THROW(tree.Get(7), std::out_of_range);
}

TEST(TreeBasic, ContainsKeyAndFind) {
    TreeDictionary<int, int> tree = MakeSample();
    EXPECT_TRUE(tree.ContainsKey(14));
    EXPECT_FALSE(tree.ContainsKey(7));
    ASSERT_NE(tree.Find(6), nullptr);
    EXPECT_EQ(*tree.Find(6), 6);
    EXPECT_EQ(tree.Find(7), nullptr);
}

TEST(TreeBasic, StringKeys) {
    TreeDictionary<std::string, int> tree;
    tree.Add("pear", 1);
    tree.Add("apple", 2);
    EXPECT_EQ(tree.Get("apple"), 2);
    EXPECT_FALSE(tree.ContainsKey("kiwi"));
}


TEST(TreeBasic, RemoveLeaf) {
    TreeDictionary<int, int> tree = MakeSample();
    tree.Remove(1);
    EXPECT_EQ(tree.GetCount(), 5u);
    EXPECT_FALSE(tree.ContainsKey(1));
}

TEST(TreeBasic, RemoveNodeWithOneChild) {
    TreeDictionary<int, int> tree = MakeSample();
    tree.Remove(10); 
    EXPECT_EQ(tree.GetCount(), 5u);
    EXPECT_FALSE(tree.ContainsKey(10));
}

TEST(TreeBasic, RemoveNodeWithTwoChildren) {
    TreeDictionary<int, int> tree = MakeSample();
    tree.Remove(3);  
    EXPECT_EQ(tree.GetCount(), 5u);
    EXPECT_FALSE(tree.ContainsKey(3));
}

TEST(TreeBasic, RemoveRoot) {
    TreeDictionary<int, int> tree = MakeSample();
    tree.Remove(8);
    EXPECT_EQ(tree.GetCount(), 5u);
    EXPECT_FALSE(tree.ContainsKey(8));

    tree.Add(8, 80);
    EXPECT_EQ(tree.Get(8), 80);
    EXPECT_EQ(tree.GetCount(), 6u);
}

TEST(TreeBasic, RemoveMissingThrows) {
    TreeDictionary<int, int> tree = MakeSample();
    EXPECT_THROW(tree.Remove(7), std::out_of_range);
    EXPECT_EQ(tree.GetCount(), 6u);
}

TEST(TreeBasic, RemoveAllKeys) {
    TreeDictionary<int, int> tree = MakeSample();
    for(int k : {8, 3, 10, 1, 6, 14}) { tree.Remove(k); }
    EXPECT_EQ(tree.GetCount(), 0u);
    EXPECT_FALSE(tree.ContainsKey(8));
    tree.Add(5, 50);
    EXPECT_EQ(tree.Get(5), 50);
}