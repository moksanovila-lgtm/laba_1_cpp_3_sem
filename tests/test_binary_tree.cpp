#include <gtest/gtest.h>
#include "binary_tree.hpp"
#include <vector>
#include <string>


TEST(BinaryTreeTest, BasicOperations) {
    BinaryTree<int> tree;
    EXPECT_TRUE(tree.IsEmpty());
    EXPECT_EQ(tree.Size(), 0);

    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(10); 

    EXPECT_FALSE(tree.IsEmpty());
    EXPECT_EQ(tree.Size(), 3);
    EXPECT_TRUE(tree.Find(10));
    EXPECT_TRUE(tree.Find(5));
    EXPECT_TRUE(tree.Find(15));
    EXPECT_FALSE(tree.Find(99)); 
}


TEST(BinaryTreeTest, EraseLeaf) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5);
    tree.Erase(5); 
    EXPECT_FALSE(tree.Find(5));
    EXPECT_EQ(tree.Size(), 1);
}

TEST(BinaryTreeTest, EraseNodeWithOneChild) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5); tree.Insert(3);
    tree.Erase(5); 
    EXPECT_FALSE(tree.Find(5));
    EXPECT_TRUE(tree.Find(3)); 
    EXPECT_EQ(tree.Size(), 2);
}

TEST(BinaryTreeTest, EraseNodeWithTwoChildren) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5); tree.Insert(15);
    tree.Insert(3); tree.Insert(7); tree.Insert(12); tree.Insert(20);
    
    tree.Erase(5); 
    EXPECT_FALSE(tree.Find(5));
    EXPECT_TRUE(tree.Find(3));
    EXPECT_TRUE(tree.Find(7));
    EXPECT_EQ(tree.Size(), 6);
}

TEST(BinaryTreeTest, EraseExceptions) {
    BinaryTree<int> tree;
    EXPECT_THROW(tree.Erase(99), std::out_of_range);
    
    tree.Insert(10);
    EXPECT_THROW(tree.Erase(99), std::out_of_range);
}


TEST(BinaryTreeTest, AllTraversals) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5); tree.Insert(15);
    std::vector<int> klp, kpl, lkp, lpk, plk, pkl;
    
    tree.TraverseKLP([&](int v) { klp.push_back(v); });
    tree.TraverseKPL([&](int v) { kpl.push_back(v); });
    tree.TraverseLKP([&](int v) { lkp.push_back(v); });
    tree.TraverseLPK([&](int v) { lpk.push_back(v); });
    tree.TraversePLK([&](int v) { plk.push_back(v); });
    tree.TraversePKL([&](int v) { pkl.push_back(v); });

    EXPECT_EQ(klp, std::vector<int>({10, 5, 15}));
    EXPECT_EQ(kpl, std::vector<int>({10, 15, 5}));
    EXPECT_EQ(lkp, std::vector<int>({5, 10, 15}));
    EXPECT_EQ(lpk, std::vector<int>({5, 15, 10}));
    EXPECT_EQ(plk, std::vector<int>({15, 5, 10}));
    EXPECT_EQ(pkl, std::vector<int>({15, 10, 5}));
}


TEST(BinaryTreeTest, MapWhereReduce) {
    BinaryTree<int> tree;
    tree.Insert(1); tree.Insert(2); tree.Insert(3);
    
    auto mapped = tree.Map([](const int& v) { return v * 2; });
    EXPECT_TRUE(mapped.Find(2));
    EXPECT_TRUE(mapped.Find(4));
    EXPECT_TRUE(mapped.Find(6));
    
    auto filtered = tree.Where([](const int& v) { return v % 2 == 0; });
    EXPECT_TRUE(filtered.Find(2));
    EXPECT_FALSE(filtered.Find(1));
    
    int sum = tree.Reduce([](int a, int b) { return a + b; }, 0);
    EXPECT_EQ(sum, 6);
}

TEST(BinaryTreeTest, ReduceExceptions) {
    BinaryTree<int> tree;
    EXPECT_THROW(tree.Reduce([](int a, int b) { return a + b; }, 0), std::logic_error);
    std::function<int(const int&, const int&)> empty_f;
    tree.Insert(1);
    EXPECT_THROW(tree.Reduce(empty_f, 0), std::invalid_argument);
}


TEST(BinaryTreeTest, ExtractSubtree) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5); tree.Insert(15); tree.Insert(3);
    
    auto sub = tree.ExtractSubtree(5);
    EXPECT_EQ(sub.Size(), 2); 
    EXPECT_TRUE(sub.Find(5));
    EXPECT_TRUE(sub.Find(3));
    
    EXPECT_THROW(tree.ExtractSubtree(99), std::out_of_range);
}


TEST(BinaryTreeTest, ContainsSubtree) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5); tree.Insert(15); tree.Insert(3);
    
    BinaryTree<int> sub;
    sub.Insert(5); sub.Insert(3);
    EXPECT_TRUE(tree.ContainsSubtree(sub));
    
    BinaryTree<int> badSub;
    badSub.Insert(5); badSub.Insert(4);
    EXPECT_FALSE(tree.ContainsSubtree(badSub));
}


TEST(BinaryTreeTest, SerializeDeserialize) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5); tree.Insert(15);
    
    std::string s = tree.Serialize();
    EXPECT_FALSE(s.empty());
    
    BinaryTree<int> restored;
    restored.Deserialize(s);
    EXPECT_EQ(restored.Size(), 3);
    EXPECT_TRUE(restored.Find(10));
    EXPECT_TRUE(restored.Find(5));
    EXPECT_TRUE(restored.Find(15));
}

TEST(BinaryTreeTest, DeserializeExceptions) {
    BinaryTree<int> tree;
    EXPECT_THROW(tree.Deserialize(""), std::invalid_argument);
}

TEST(BinaryTreeTest, CopyAndMove) {
    BinaryTree<int> tree;
    tree.Insert(10); tree.Insert(5);
    
    BinaryTree<int> copy = tree;
    EXPECT_EQ(copy.Size(), 2);
    copy.Insert(15);
    EXPECT_EQ(copy.Size(), 3);
    EXPECT_EQ(tree.Size(), 2); 
    
    BinaryTree<int> moved = std::move(tree);
    EXPECT_EQ(moved.Size(), 2);
    EXPECT_TRUE(tree.IsEmpty()); 
}

TEST(BinaryTreeTest, Clear) {
    BinaryTree<int> tree;
    tree.Insert(10);
    tree.Clear();
    EXPECT_TRUE(tree.IsEmpty());
    EXPECT_EQ(tree.Size(), 0);
}