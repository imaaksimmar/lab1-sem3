#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../include/TreeDictionary.hpp"

static TreeDictionary<int, int> MakeSample() {
    TreeDictionary<int, int> tree;
    for(int k : {8, 3, 10, 1, 6, 14}) { tree.Add(k, k); }
    return tree;
}

static std::string InOrder(TreeDictionary<int, int>& t) {
    return t.Reduce<std::string>(
        [](int x, std::string acc) { return acc + std::to_string(x) + ","; },
        std::string(""));
}


TEST(TreeExtract, CopiesSubtree) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> sub = tree.ExtractSubtree(3);
    EXPECT_EQ(sub.GetCount(), 3u);
    EXPECT_EQ(InOrder(sub), "1,3,6,");
}

TEST(TreeExtract, LeafGivesSingleNode) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> sub = tree.ExtractSubtree(14);
    EXPECT_EQ(sub.GetCount(), 1u);
    EXPECT_EQ(sub.Get(14), 14);
}

TEST(TreeExtract, RootGivesWholeTree) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> sub = tree.ExtractSubtree(8);
    EXPECT_EQ(InOrder(sub), "1,3,6,8,10,14,");
}

TEST(TreeExtract, CopyIsIndependentFromOriginal) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> sub = tree.ExtractSubtree(3);
    sub.Remove(3);
    sub.Add(100, 100);
    EXPECT_TRUE(tree.ContainsKey(3));
    EXPECT_FALSE(tree.ContainsKey(100));
    EXPECT_EQ(tree.GetCount(), 6u);
}

TEST(TreeExtract, MissingKeyThrows) {
    TreeDictionary<int, int> tree = MakeSample();
    EXPECT_THROW(tree.ExtractSubtree(7), std::invalid_argument);
}

TEST(TreeExtract, FromEmptyThrows) {
    TreeDictionary<int, int> tree;
    EXPECT_THROW(tree.ExtractSubtree(1), std::invalid_argument);
}




TEST(TreeSubtree, FoundWithMatchingShape) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> subTree;
    for(int k : {3, 1, 6}) { subTree.Add(k, k); }
    EXPECT_TRUE(tree.ContainsSubtree(subTree));
}

TEST(TreeSubtree, NotFoundWhenChildIsMissingInPattern) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> subTree;
    for(int k : {3, 1}) { subTree.Add(k, k); } 
    EXPECT_FALSE(tree.ContainsSubtree(subTree));
}

TEST(TreeSubtree, NotFoundWhenPatternHasExtraChild) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> subTree;
    for(int k : {10, 9, 14}) { subTree.Add(k, k); }  
    EXPECT_FALSE(tree.ContainsSubtree(subTree));
}

TEST(TreeSubtree, NotFoundWhenValueDiffers) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> subTree;
    subTree.Add(3, 999);
    EXPECT_FALSE(tree.ContainsSubtree(subTree));
}

TEST(TreeSubtree, NotFoundWhenRootKeyIsMissing) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> subTree;
    subTree.Add(7, 7);
    EXPECT_FALSE(tree.ContainsSubtree(subTree));
}

TEST(TreeSubtree, EmptyTreeDoesNotContainN) {
    TreeDictionary<int, int> tree;
    TreeDictionary<int, int> subTree;
    subTree.Add(1, 1);
    EXPECT_FALSE(tree.ContainsSubtree(subTree));
}

TEST(TreeSubtree, WholeTree) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> copy = tree.ExtractSubtree(8);
    EXPECT_TRUE(tree.ContainsSubtree(copy));
}
