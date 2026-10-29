#include "binary_tree.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <windows.h>

template <typename T>
void print_tree(TreeNode<T>* root, const std::string& prefix = "", bool isLeft = true) {
    if (!root) return;

    print_tree(root->right, prefix + (isLeft ? "│   " : "    "), false);

    std::cout << prefix << (isLeft ? "└── " : "┌── ") << root->value << "\n";

    print_tree(root->left, prefix + (isLeft ? "    " : "│   "), true);
}

void print_menu() {
    std::cout << "\n1. Вставить элемент\n";
    std::cout << "2. Найти элемент\n";
    std::cout << "3. Удалить элемент\n";
    std::cout << "4. Вывести обходом (в строку)\n";
    std::cout << "5. Показать дерево (визуально)\n";
    std::cout << "6. Размер дерева\n";
    std::cout << "7. Извлечь поддерево\n";
    std::cout << "8. Проверить наличие поддерева\n";
    std::cout << "9. Сохранить в строку (Serialize)\n";
    std::cout << "10. Загрузить из строки (Deserialize)\n";
    std::cout << "11. Map (умножить все на 2)\n";
    std::cout << "12. Where (оставить только четные)\n";
    std::cout << "13. Reduce (сумма всех элементов)\n";
    std::cout << "0. Выход\n";
    std::cout << "Выбор: ";
}


int main() {
    SetConsoleOutputCP(65001);  
    SetConsoleCP(65001);    

    BinaryTree<int> tree;
    int choice;

    while (true) {
        print_menu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Ошибка ввода. Введите число.\n";
            continue;
        }

        try {
            if (choice == 1) {
                int val;
                std::cout << "Значение: ";
                std::cin >> val;
                tree.Insert(val);
                std::cout << "Добавлено " << val << ".\n";

            } else if (choice == 2) {
                int val;
                std::cout << "Значение: ";
                std::cin >> val;
                if (tree.Find(val)) std::cout << "Элемент найден!\n";
                else std::cout << "Элемент НЕ найден.\n";

            } else if (choice == 3) {
                int val;
                std::cout << "Значение: ";
                std::cin >> val;
                tree.Erase(val);
                std::cout << "Удалено.\n";

            } else if (choice == 4) {
                if (tree.IsEmpty()) {
                    std::cout << "Дерево пустое.\n";
                    continue;
                }
                std::cout << "Выберите обход:\n";
                std::cout << "1. КЛП (Pre-order)\n";
                std::cout << "2. КПЛ\n";
                std::cout << "3. ЛКП (In-order, сортировка)\n";
                std::cout << "4. ЛПК (Post-order)\n";
                std::cout << "5. ПЛК\n";
                std::cout << "6. ПКЛ\n";
                int trav_choice;
                std::cin >> trav_choice;

                auto print_visitor = [](int v) { std::cout << v << " "; };
                switch (trav_choice) {
                    case 1: tree.TraverseKLP(print_visitor); break;
                    case 2: tree.TraverseKPL(print_visitor); break;
                    case 3: tree.TraverseLKP(print_visitor); break;
                    case 4: tree.TraverseLPK(print_visitor); break;
                    case 5: tree.TraversePLK(print_visitor); break;
                    case 6: tree.TraversePKL(print_visitor); break;
                    default: std::cout << "Неверный выбор.\n"; continue;
                }
                std::cout << "\n";

            } else if (choice == 5) {
                if (tree.IsEmpty()) {
                    std::cout << "Дерево пустое.\n";
                } else {
                    std::cout << "\n";
                    print_tree(tree.Root());
                    std::cout << "\n";
                }

            } else if (choice == 6) {
                std::cout << "Размер: " << tree.Size() << "\n";

            } else if (choice == 7) {
                int val;
                std::cout << "Корень поддерева: ";
                std::cin >> val;
                auto sub = tree.ExtractSubtree(val);
                std::cout << "Извлечено поддерево размером " << sub.Size() << ":\n";
                sub.TraverseKLP([](int v) { std::cout << v << " "; });
                std::cout << "\n";

            } else if (choice == 8) {
                std::cout << "Введите корень поддерева для поиска: ";
                int val;
                std::cin >> val;
                BinaryTree<int> sub;
                sub.Insert(val);
                if (tree.ContainsSubtree(sub)) std::cout << "Поддерево найдено!\n";
                else std::cout << "Поддерево НЕ найдено.\n";

            } else if (choice == 9) {
                std::cout << "Строка: " << tree.Serialize() << "\n";

            } else if (choice == 10) {
                std::string s;
                std::cout << "Введите строку чисел через пробел: ";
                std::cin.ignore();
                std::getline(std::cin, s);
                tree.Deserialize(s);
                std::cout << "Загружено. Размер: " << tree.Size() << "\n";

            } else if (choice == 11) {
                auto mapped = tree.Map([](const int& v) { return v * 2; });
                std::cout << "Map (x2): ";
                mapped.TraverseKLP([](int v) { std::cout << v << " "; });
                std::cout << "\n";

            } else if (choice == 12) {
                auto filtered = tree.Where([](const int& v) { return v % 2 == 0; });
                std::cout << "Where (только четные): ";
                filtered.TraverseKLP([](int v) { std::cout << v << " "; });
                std::cout << "\n";

            } else if (choice == 13) {
                int sum = tree.Reduce([](int a, int b) { return a + b; }, 0);
                std::cout << "Сумма всех элементов: " << sum << "\n";

            } else if (choice == 0) {
                std::cout << "Выход.\n";
                break;

            } else {
                std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}


BinaryTree<int> tree2;
{
tree2.Insert(10);
tree2.Insert(5);
    tree2.Insert(15);
    tree2.Insert(3);
    tree2.Insert(7);
    auto tree4 = tree.Map([](const int& v){return v * 2;});
}