#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../include/TreeDictionary.hpp"

static TreeDictionary<int, int> MakeSample() {
    TreeDictionary<int, int> tree;
    for(int k : {8, 3, 10, 1, 6, 14}) { tree.Add(k, k); }
    return tree;
}

static int Double(int x) { return x*2; }
static int AddOne(int x) { return x+1; }
static int Negate(int x) { return -x; }

static bool IsEven(int x) { return x%2 == 0; }
static bool IsGreaterThan100(int x) { return x>100; }

static int AddToSum(int x, int acc) { return x+acc; }
static int MethodFunc(int x, int acc) { return 2*x + 3*acc; }



TEST(TreeMap, DoesNotChangeOriginal) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> doubled = tree.Map<int>(Double);
    EXPECT_EQ(doubled.Get(6), 12);
    EXPECT_EQ(tree.Get(6), 6);
}

TEST(TreeMap, EmptyTree) {
    TreeDictionary<int, int> tree;
    TreeDictionary<int, int> result = tree.Map<int>(AddOne);
    EXPECT_EQ(result.GetCount(), 0u);
}

TEST(TreeMap, KeysAreKept) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> result = tree.Map<int>(Negate);
    EXPECT_EQ(result.GetCount(), 6u);
    for(int k : {8, 3, 10, 1, 6, 14}) { EXPECT_EQ(result.Get(k), -k); }
}



TEST(TreeWhere, FiltersEven) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> even = tree.Where(IsEven);
    EXPECT_EQ(even.GetCount(), 4u);
    
    for(int k : {6, 8, 10, 14}) {
        ASSERT_TRUE(even.ContainsKey(k));
        EXPECT_EQ(even.Get(k), k);
    }
    for(int k : {1, 3}) { EXPECT_FALSE(even.ContainsKey(k)); }
}

TEST(TreeWhere, DoesNotChangeOriginal) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> even = tree.Where(IsEven);
    EXPECT_EQ(tree.GetCount(), 6u);
    EXPECT_TRUE(tree.ContainsKey(3));
}

TEST(TreeWhere, NothingMatches) {
    TreeDictionary<int, int> tree = MakeSample();
    TreeDictionary<int, int> result = tree.Where(IsGreaterThan100);
    EXPECT_EQ(result.GetCount(), 0u);
}



TEST(TreeReduce, Sum) {
    TreeDictionary<int, int> tree = MakeSample();
    int sum = tree.Reduce<int>(AddToSum, 0);
    EXPECT_EQ(sum, 42);
}

TEST(TreeReduce, EmptyTreeReturnsStartValue) {
    TreeDictionary<int, int> tree;
    int result = tree.Reduce<int>(AddToSum, 42);
    EXPECT_EQ(result, 42);
}

TEST(TreeReduce, MethodFunc) {
    TreeDictionary<int, int> tree;
    for(int k : {1, 2, 3}) { tree.Add(k, k); }
    int result = tree.Reduce<int>(MethodFunc, 4);
    EXPECT_EQ(result, 144);
}
