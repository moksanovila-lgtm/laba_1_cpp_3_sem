#include "binary_tree.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <windows.h>

template <typename Key, typename Value>
void print_tree(TreeNode<Key, Value>* root, const std::string& prefix = "", bool isLeft = true) {
    if (!root) return;
    print_tree(root->right, prefix + (isLeft ? "│   " : "    "), false);
    std::cout << prefix << (isLeft ? "└── " : "┌── ") << root->value << "\n";
    print_tree(root->left, prefix + (isLeft ? "    " : "│   "), true);
}

void print_menu() {
    std::cout << "\n1. Вставить элемент\n";
    std::cout << "2. Найти элемент по ключу\n";
    std::cout << "3. Удалить элемент по ключу\n";
    std::cout << "4. Вывести обходом\n";
    std::cout << "5. Показать дерево (визуально)\n";
    std::cout << "6. Размер дерева\n";
    std::cout << "7. Извлечь поддерево (по ключу)\n";
    std::cout << "8. Проверить наличие поддерева (по ключу)\n";
    std::cout << "9. Сохранить в строку (Serialize)\n";
    std::cout << "10. Загрузить из строки (Deserialize)\n";
    std::cout << "11. Map (умножить ключи на 2)\n";
    std::cout << "12. Where (оставить только чётные ключи)\n";
    std::cout << "13. Reduce (сумма для чисел/сумма длин для строк)\n";
    std::cout << "0. Выход\n";
    std::cout << "Выбор: ";
}


int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Выберите режим работы:\n";
    std::cout << "1. Числа (вводим только число, ключ = значение)\n";
    std::cout << "2. Объекты (вводим ключ и значение отдельно)\n";
    std::cout << "Ваш выбор: ";
    int mode;
    std::cin >> mode;

    if (mode == 1) {
        BinaryTree<int, int> tree;
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
                    std::cout << "Число: ";
                    std::cin >> val;
                    tree.Insert(val, val); 
                    std::cout << "Добавлено: " << val << "\n";
                } else if (choice == 2) {
                    int val;
                    std::cout << "Ключ: ";
                    std::cin >> val;
                    if (tree.Find(val)) std::cout << "Найдено!\n";
                    else std::cout << "НЕ найдено.\n";
                } else if (choice == 3) {
                    int val;
                    std::cout << "Ключ: ";
                    std::cin >> val;
                    tree.Erase(val);
                    std::cout << "Удалено.\n";
                } else if (choice == 4) {
                    if (tree.IsEmpty()) { std::cout << "Пусто.\n"; continue; }
                    std::cout << "Выберите обход:\n1. КЛП\n2. КПЛ\n3. ЛКП\n4. ЛПК\n5. ПЛК\n6. ПКЛ\n";
                    int trav_choice;
                    std::cin >> trav_choice;
                    auto visitor = [](const int&, const int& v) { std::cout << v << " "; };
                    switch (trav_choice) {
                        case 1: tree.Traverse(visitor, TraversalStrategy::KLP); break;
                        case 2: tree.Traverse(visitor, TraversalStrategy::KPL); break;
                        case 3: tree.Traverse(visitor, TraversalStrategy::LKP); break;
                        case 4: tree.Traverse(visitor, TraversalStrategy::LPK); break;
                        case 5: tree.Traverse(visitor, TraversalStrategy::PLK); break;
                        case 6: tree.Traverse(visitor, TraversalStrategy::PKL); break;
                        default: std::cout << "Неверный выбор.\n"; continue;
                    }
                    std::cout << "\n";
                } else if (choice == 5) {
                    if (tree.IsEmpty()) std::cout << "Пусто.\n";
                    else { std::cout << "\n"; print_tree(tree.Root()); std::cout << "\n"; }
                } else if (choice == 6) {
                    std::cout << "Размер: " << tree.Size() << "\n";
                } else if (choice == 7) {
                    int key;
                    std::cout << "Ключ корня поддерева: ";
                    std::cin >> key;
                    auto sub = tree.ExtractSubtree(key);
                    std::cout << "Извлечено поддерево размером " << sub.Size() << ":\n";
                    sub.Traverse([](const int&, const int& v) {
                        std::cout << v << " ";
                    }, TraversalStrategy::KLP);
                    std::cout << "\n";
                } else if (choice == 8) {
                    int key;
                    std::cout << "Ключ для поиска: ";
                    std::cin >> key;
                    BinaryTree<int, int> sub;
                    sub.Insert(key, key);
                    if (tree.ContainsSubtree(sub)) std::cout << "Найдено!\n";
                    else std::cout << "НЕ найдено.\n";
                } else if (choice == 9) {
                    std::cout << "Строка: " << tree.Serialize(true) << "\n";
                } else if (choice == 10) {
                    std::string s;
                    std::cout << "Введите числа через пробел (например: 10 5 15): ";
                    std::cin.ignore();
                    std::getline(std::cin, s);
                    tree.Deserialize(s, true);
                    std::cout << "Загружено. Размер: " << tree.Size() << "\n";
                } else if (choice == 11) {
                    auto mapped = tree.Map([](const int& k, const int& v) {
                        return std::make_pair(k * 2, v * 2); 
                    });
                    std::cout << "Map (x2): ";
                    mapped.Traverse([](const int&, const int& v) {
                        std::cout << v << " ";
                    }, TraversalStrategy::KLP);
                    std::cout << "\n";
                } else if (choice == 12) {
                    auto filtered = tree.Where([](const int& k, const int&) {
                        return k % 2 == 0;
                    });
                    std::cout << "Where (чётные): ";
                    filtered.Traverse([](const int&, const int& v) {
                        std::cout << v << " ";
                    }, TraversalStrategy::KLP);
                    std::cout << "\n";
                } else if (choice == 13) {
                    int sum = tree.Reduce<int>([](int acc, const int& v) { return acc + v; }, 0);
                    std::cout << "Сумма: " << sum << "\n";
                } else if (choice == 0) {
                    break;
                } else {
                    std::cout << "Неверный пункт.\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << "\n";
            }
        }
    } else {
        BinaryTree<int, std::string> tree;
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
                    int key;
                    std::string value;
                    std::cout << "Ключ (число): "; std::cin >> key;
                    std::cout << "Значение (строка): "; std::cin >> value;
                    tree.Insert(key, value);
                    std::cout << "Добавлено: " << key << " -> " << value << "\n";
                } else if (choice == 2) {
                    int key;
                    std::cout << "Ключ: "; std::cin >> key;
                    if (tree.Find(key)) std::cout << "Найдено!\n";
                    else std::cout << "НЕ найдено.\n";
                } else if (choice == 3) {
                    int key;
                    std::cout << "Ключ: "; std::cin >> key;
                    tree.Erase(key);
                    std::cout << "Удалено.\n";
                } else if (choice == 4) {
                    if (tree.IsEmpty()) { std::cout << "Пусто.\n"; continue; }
                    std::cout << "Выберите обход:\n1. КЛП\n2. КПЛ\n3. ЛКП\n4. ЛПК\n5. ПЛК\n6. ПКЛ\n";
                    int trav_choice;
                    std::cin >> trav_choice;                   
                    auto visitor = [](const int&, const std::string& v) { std::cout << v << " "; };
                    switch (trav_choice) {
                        case 1: tree.Traverse(visitor, TraversalStrategy::KLP); break;
                        case 2: tree.Traverse(visitor, TraversalStrategy::KPL); break;
                        case 3: tree.Traverse(visitor, TraversalStrategy::LKP); break;
                        case 4: tree.Traverse(visitor, TraversalStrategy::LPK); break;
                        case 5: tree.Traverse(visitor, TraversalStrategy::PLK); break;
                        case 6: tree.Traverse(visitor, TraversalStrategy::PKL); break;
                        default: std::cout << "Неверный выбор.\n"; continue;
                    }
                    std::cout << "\n";
                } else if (choice == 5) {
                    if (tree.IsEmpty()) std::cout << "Пусто.\n";
                    else { std::cout << "\n"; print_tree(tree.Root()); std::cout << "\n"; }
                } else if (choice == 6) {
                    std::cout << "Размер: " << tree.Size() << "\n";
                } else if (choice == 7) {
                    int key;
                    std::cout << "Ключ корня поддерева: "; std::cin >> key;
                    auto sub = tree.ExtractSubtree(key);
                    std::cout << "Извлечено: ";
                    sub.Traverse([](const int&, const std::string& v) {
                        std::cout << v << " ";
                    }, TraversalStrategy::KLP);
                    std::cout << "\n";
                } else if (choice == 8) {
                    int key;
                    std::cout << "Ключ: "; std::cin >> key;
                    BinaryTree<int, std::string> sub;
                    sub.Insert(key, "");
                    if (tree.ContainsSubtree(sub)) std::cout << "Найдено!\n";
                    else std::cout << "НЕ найдено.\n";
                } else if (choice == 9) {
                    std::cout << "Строка: " << tree.Serialize(false) << "\n";
                } else if (choice == 10) {
                    std::string s;
                    std::cout << "Введите пары (например: 10:Иван 5:Петр): ";
                    std::cin.ignore();
                    std::getline(std::cin, s);
                    tree.Deserialize(s, false);
                    std::cout << "Загружено. Размер: " << tree.Size() << "\n";
                } else if (choice == 11) {
                    auto mapped = tree.Map([](const int& k, const std::string& v) {
                        return std::make_pair(k * 2, v);
                    });
                    std::cout << "Map: ";
                    mapped.Traverse([](const int& k, const std::string& v) {
                        std::cout << k << ": " << v << " | ";  
                    }, TraversalStrategy::KLP);
                    std::cout << "\n";
                } else if (choice == 12) {
                    auto filtered = tree.Where([](const int& k, const std::string&) {
                        return k % 2 == 0;
                    });
                    std::cout << "Where: ";
                    filtered.Traverse([](const int&, const std::string& v) {
                        std::cout << v << " ";
                    }, TraversalStrategy::KLP);
                    std::cout << "\n";
                } else if (choice == 13) {
                    int sum = tree.Reduce<int>([](int acc, const std::string& v) { return acc + v.length(); }, 0);
                    std::cout << "Сумма длин строк: " << sum << "\n";
                } else if (choice == 0) {
                    break;
                } else {
                    std::cout << "Неверный пункт.\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << "\n";
            }
        }
    }
    return 0;
}