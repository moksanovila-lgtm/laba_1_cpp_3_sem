#include "binary_tree.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <numeric>
#include <algorithm>
#include <windows.h>

using Clock = std::chrono::high_resolution_clock;

double measure_us(std::function<void()> f) {
    auto start = Clock::now();
    f();
    auto end = Clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

void bench_insert(std::size_t n) {
    BinaryTree<int> tree;
    
    std::vector<int> values(n);
    std::iota(values.begin(), values.end(), 0);           
    std::shuffle(values.begin(), values.end(), std::mt19937(42)); 

    double us = measure_us([&]() {
        for (int v : values) {
            tree.Insert(v);
        }
    });
    std::cout << "  Вставка " << n << ": " << us << " мкс\n";
}


void bench_find(std::size_t n) {
    BinaryTree<int> tree;
    
    std::vector<int> values(n);
    std::iota(values.begin(), values.end(), 0);
    std::shuffle(values.begin(), values.end(), std::mt19937(42));

    for (int v : values) tree.Insert(v);

    double us = measure_us([&]() {
        for (int v : values) tree.Find(v);
    });
    std::cout << "  Поиск   " << n << ": " << us << " мкс\n";
}


void bench_erase(std::size_t n) {
    BinaryTree<int> tree;
    
    std::vector<int> values(n);
    std::iota(values.begin(), values.end(), 0);
    std::shuffle(values.begin(), values.end(), std::mt19937(42));

    for (int v : values) tree.Insert(v);

    double us = measure_us([&]() {
        for (int v : values) tree.Erase(v);
    });
    std::cout << "  Удаление " << n << ": " << us << " мкс\n";
}


int main() {
    SetConsoleOutputCP(65001);

    std::vector<std::size_t> sizes = {1000, 10000, 100000, 1000000};

    for (std::size_t n : sizes) {
        std::cout << "\n N = " << n << " \n";
        bench_insert(n);
        bench_find(n);
        bench_erase(n);
    }

    return 0;
}