#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include <cstdlib>
#include <random>
#include <stdexcept>
#include "TreeDictionary.hpp"


int Double(int x) { return x*2; }
int AddOne(int x) { return x+1; }
int Negate(int x) { return -x; }

bool IsEven(int x) { return x%2==0; }
bool IsOdd(int x) { return x%2!=0; }
bool IsGreaterThan10(int x) { return x>10; }

int Sum(int x, int acc) { return acc+x; }
int Product(int x, int acc) { return acc*x; }


int ReadInt(const std::string& text) {
    int number;
    std::cout << text;
    while(!(std::cin >> number)) {
        if(std::cin.eof()) { std::exit(0); }   
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Необходимо ввести целое число: ";
    }
    return number;
}


void PrintNode(const Node<int, int>* node, int depth) {
    if(node == nullptr) { return; }

    PrintNode(node->right, depth + 1);
    std::cout << std::string(depth * 8, ' ') << node->key << "(" << node->value << ")\n";
    PrintNode(node->left, depth + 1);
}

void PrintTree(const TreeDictionary<int, int>& tree) {
    if(tree.GetCount() == 0) {
        std::cout << "Дерево пустое\n";
        return;
    }
    PrintNode(tree.GetRoot(), 0);
}

void PrintMenu() {
    std::cout << "\n===== Бинарное дерево поиска =====\n";
    std::cout << " 1. Добавить (ключ, значение)\n";
    std::cout << " 2. Удалить по ключу\n";
    std::cout << " 3. Найти значение по ключу\n";
    std::cout << " 4. Показать дерево\n";
    std::cout << " 5. Map\n";
    std::cout << " 6. Where\n";
    std::cout << " 7. Reduce\n";
    std::cout << " 8. Извлечь поддерево\n";
    std::cout << " 9. Проверить вхождение поддерева\n";
    std::cout << " 0. Выход\n";
}


void DoAdd(TreeDictionary<int, int>& tree) {
    int key = ReadInt("Ключ: ");
    int value = ReadInt("Значение: ");

    if(tree.ContainsKey(key)) {
        std::cout << "Ключ уже был, значение заменено\n";
    }
    tree.Add(key, value);
    PrintTree(tree);
}

void DoRemove(TreeDictionary<int, int>& tree) {
    int key = ReadInt("Ключ для удаления: ");

    try {
        tree.Remove(key);
        std::cout << "Удалено\n";
        PrintTree(tree);
    }
    catch(const std::out_of_range&) {
        std::cout << "Ошибка: такого ключа нет\n";
    }
}

void DoFind(TreeDictionary<int, int>& tree) {
    int key = ReadInt("Ключ для поиска: ");

    int* found = tree.Find(key);
    if(found == nullptr) {
        std::cout << "Такого ключа нет\n";
    }
    else {
        std::cout << "Найдено, значение: " << *found << "\n";
    }
}




void DoMap(const TreeDictionary<int, int>& tree) {
    std::cout << "1. x * 2   2. x + 1   3. -x\n";
    int choice = ReadInt("Выбор: ");
    if(choice < 1 || choice > 3) {
        std::cout << "Нет такого пункта\n";
        return;
    }

    std::cout << "Результат Map (исходное дерево не изменилось):\n";
    if(choice == 1) { PrintTree(tree.Map<int>(Double)); }
    else if(choice == 2) { PrintTree(tree.Map<int>(AddOne)); }
    else { PrintTree(tree.Map<int>(Negate)); }
}

void DoWhere(const TreeDictionary<int, int>& tree) {
    std::cout << "1. чётные   2. нечётные   3. больше 10\n";
    int choice = ReadInt("Выбор: ");
    if(choice < 1 || choice > 3) {
        std::cout << "Нет такого пункта\n";
        return;
    }

    std::cout << "Результат Where (исходное дерево не изменилось):\n";
    if(choice == 1) { PrintTree(tree.Where(IsEven)); }
    else if(choice == 2) { PrintTree(tree.Where(IsOdd)); }
    else { PrintTree(tree.Where(IsGreaterThan10)); }
}

void DoReduce(const TreeDictionary<int, int>& tree) {
    if(tree.GetCount() == 0) {
        std::cout << "Дерево пустое\n";
        return;
    }
    std::cout << "1. сумма   2. произведение\n";
    int choice = ReadInt("Выбор: ");

    int answer = 0;
    if(choice == 1) { answer = tree.Reduce<int>(Sum, 0); }
    else if(choice == 2) { answer = tree.Reduce<int>(Product, 1); }
    else {
        std::cout << "Нет такого пункта\n";
        return;
    }
    std::cout << "Результат: " << answer << "\n";
}

void DoExtract(const TreeDictionary<int, int>& tree) {
    int key = ReadInt("Ключ корня поддерева: ");

    try {
        TreeDictionary<int, int> sub = tree.ExtractSubtree(key);
        std::cout << "Извлечённое поддерево:\n";
        PrintTree(sub);
    }
    catch(const std::invalid_argument&) {
        std::cout << "Ошибка: такого ключа нет\n";
    }
}

void DoContains(TreeDictionary<int, int>& tree) {
    std::cout << "Введите ключи шаблона через пробел: ";
    std::cin.ignore(10000, '\n');
    std::string line;
    std::getline(std::cin, line);

    TreeDictionary<int, int> pattern;
    std::istringstream stream(line);
    int key;
    while(stream >> key) {
        int* found = tree.Find(key);
        if(found != nullptr) { pattern.Add(key, *found); }
        else { pattern.Add(key, key); }
    }
    if(!stream.eof()) {
        std::cout << "Ошибка: в строке должны быть только числа\n";
        return;
    }

    std::cout << "Шаблон:\n";
    PrintTree(pattern);
    if(tree.ContainsSubtree(pattern)) {
        std::cout << "Шаблон входит в дерево как поддерево\n";
    }
    else {
        std::cout << "Шаблон НЕ входит в дерево как поддерево\n";
    }
}



int main() {
    TreeDictionary<int, int> tree;

    while(true) {
        PrintMenu();
        int choice = ReadInt("Выбор: ");

        if(choice == 0) { break; }
        else if(choice == 1) { DoAdd(tree); }
        else if(choice == 2) { DoRemove(tree); }
        else if(choice == 3) { DoFind(tree); }
        else if(choice == 4) { PrintTree(tree); }
        else if(choice == 5) { DoMap(tree); }
        else if(choice == 6) { DoWhere(tree); }
        else if(choice == 7) { DoReduce(tree); }
        else if(choice == 8) { DoExtract(tree); }
        else if(choice == 9) { DoContains(tree); }
        else { std::cout << "Нет такого пункта\n"; }
    }
    return 0;
}