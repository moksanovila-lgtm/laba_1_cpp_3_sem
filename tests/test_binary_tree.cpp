#include <gtest/gtest.h>
#include "binary_tree.hpp"
#include <vector>
#include <string>
#include <utility>

void insert_key(BinaryTree<int, std::string>& tree, int key) {
    tree.Insert(key, ""); 
}

TEST(BinaryTreeTest, BasicOperations) {
    BinaryTree<int, std::string> tree;
    EXPECT_TRUE(tree.IsEmpty());
    EXPECT_EQ(tree.Size(), 0);
    tree.Insert(10, "a");
    tree.Insert(5, "b");
    tree.Insert(15, "c");
    tree.Insert(10, "d"); 

    EXPECT_FALSE(tree.IsEmpty());
    EXPECT_EQ(tree.Size(), 3);
    EXPECT_TRUE(tree.Find(10));
    EXPECT_TRUE(tree.Find(5));
    EXPECT_TRUE(tree.Find(15));
    EXPECT_FALSE(tree.Find(99));
}

TEST(BinaryTreeTest, EraseLeaf) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10); insert_key(tree, 5);
    tree.Erase(5);
    EXPECT_FALSE(tree.Find(5));
    EXPECT_EQ(tree.Size(), 1);
}

TEST(BinaryTreeTest, EraseNodeWithOneChild) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10); insert_key(tree, 5); insert_key(tree, 3);
    tree.Erase(5);
    EXPECT_FALSE(tree.Find(5));
    EXPECT_TRUE(tree.Find(3));
    EXPECT_EQ(tree.Size(), 2);
}

TEST(BinaryTreeTest, EraseNodeWithTwoChildren) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10); insert_key(tree, 5); insert_key(tree, 15);
    insert_key(tree, 3); insert_key(tree, 7); insert_key(tree, 12); insert_key(tree, 20);
    tree.Erase(5);
    EXPECT_FALSE(tree.Find(5));
    EXPECT_TRUE(tree.Find(3));
    EXPECT_TRUE(tree.Find(7));
    EXPECT_EQ(tree.Size(), 6);
}

TEST(BinaryTreeTest, EraseExceptions) {
    BinaryTree<int, std::string> tree;
    EXPECT_THROW(tree.Erase(99), std::out_of_range);
    tree.Insert(10, "x");
    EXPECT_THROW(tree.Erase(99), std::out_of_range);
}

TEST(BinaryTreeTest, AllTraversals) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10); insert_key(tree, 5); insert_key(tree, 15);
    std::vector<int> klp, kpl, lkp, lpk, plk, pkl;
    auto push_k = [](std::vector<int>& vec) {
        return [&vec](const int& k, const std::string&) { vec.push_back(k); };
    };
    tree.Traverse(TraversalOrder::KLP, push_k(klp));
    tree.Traverse(TraversalOrder::KPL, push_k(kpl));
    tree.Traverse(TraversalOrder::LKP, push_k(lkp));
    tree.Traverse(TraversalOrder::LPK, push_k(lpk));
    tree.Traverse(TraversalOrder::PLK, push_k(plk));
    tree.Traverse(TraversalOrder::PKL, push_k(pkl));
    EXPECT_EQ(klp, std::vector<int>({10, 5, 15}));
    EXPECT_EQ(kpl, std::vector<int>({10, 15, 5}));
    EXPECT_EQ(lkp, std::vector<int>({5, 10, 15}));
    EXPECT_EQ(lpk, std::vector<int>({5, 15, 10}));
    EXPECT_EQ(plk, std::vector<int>({15, 5, 10}));
    EXPECT_EQ(pkl, std::vector<int>({15, 10, 5}));
}

TEST(BinaryTreeTest, MapWhereReduce) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 1); insert_key(tree, 2); insert_key(tree, 3);
    auto mapped = tree.Map([](const int& k, const std::string& v) {
        return std::make_pair(k * 2, v);
    });
    EXPECT_TRUE(mapped.Find(2));
    EXPECT_TRUE(mapped.Find(4));
    EXPECT_TRUE(mapped.Find(6));
    auto filtered = tree.Where([](const int& k, const std::string&) {
        return k % 2 == 0;
    });
    EXPECT_TRUE(filtered.Find(2));
    EXPECT_FALSE(filtered.Find(1));
    int sum = tree.Reduce(
        [](int acc, const std::string&) { return acc + 1; }, 
        0
    );
    EXPECT_EQ(sum, 3);
}

TEST(BinaryTreeTest, ReduceExceptions) {
    BinaryTree<int, std::string> tree;
    EXPECT_THROW(tree.Reduce([](int a, const std::string&) { return a; }, 0), std::logic_error);
    insert_key(tree, 1);
}

TEST(BinaryTreeTest, ExtractSubtree) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10); insert_key(tree, 5); insert_key(tree, 15); insert_key(tree, 3);
    auto sub = tree.ExtractSubtree(5);
    EXPECT_EQ(sub.Size(), 2);
    EXPECT_TRUE(sub.Find(5));
    EXPECT_TRUE(sub.Find(3));
    EXPECT_THROW(tree.ExtractSubtree(99), std::out_of_range);
}

TEST(BinaryTreeTest, ContainsSubtree) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10); insert_key(tree, 5); insert_key(tree, 15); insert_key(tree, 3);
    BinaryTree<int, std::string> sub;
    insert_key(sub, 5); insert_key(sub, 3);
    EXPECT_TRUE(tree.ContainsSubtree(sub));
    BinaryTree<int, std::string> badSub;
    insert_key(badSub, 5); insert_key(badSub, 4);
    EXPECT_FALSE(tree.ContainsSubtree(badSub));
}

TEST(BinaryTreeTest, SerializeDeserializePairs) {
    BinaryTree<int, std::string> tree;
    tree.Insert(10, "a");
    tree.Insert(5, "b");
    tree.Insert(15, "c");
    std::string s = tree.SerializePairs();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find("10:a"), std::string::npos);
    EXPECT_NE(s.find("5:b"), std::string::npos);
    EXPECT_NE(s.find("15:c"), std::string::npos);
    BinaryTree<int, std::string> restored;
    restored.DeserializePairs(s);
    EXPECT_EQ(restored.Size(), 3);
    EXPECT_TRUE(restored.Find(10));
    EXPECT_TRUE(restored.Find(5));
    EXPECT_TRUE(restored.Find(15));
}

TEST(BinaryTreeTest, DeserializeExceptions) {
    BinaryTree<int, std::string> tree;
    EXPECT_THROW(tree.DeserializePairs(""), std::invalid_argument);
    EXPECT_THROW(tree.DeserializePairs("10 5 15"), std::runtime_error);
}

TEST(BinaryTreeTest, CopyAndMove) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10); insert_key(tree, 5);
    BinaryTree<int, std::string> copy = tree;
    EXPECT_EQ(copy.Size(), 2);
    insert_key(copy, 15);
    EXPECT_EQ(copy.Size(), 3);
    EXPECT_EQ(tree.Size(), 2);
    BinaryTree<int, std::string> moved = std::move(tree);
    EXPECT_EQ(moved.Size(), 2);
    EXPECT_TRUE(tree.IsEmpty());
}

TEST(BinaryTreeTest, Clear) {
    BinaryTree<int, std::string> tree;
    insert_key(tree, 10);
    tree.Clear();
    EXPECT_TRUE(tree.IsEmpty());
    EXPECT_EQ(tree.Size(), 0);
}