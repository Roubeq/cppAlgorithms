#include <iostream>
#include <fstream>

using namespace std;

struct Node {
    int value;                              // Значение узла
    Node** children;                        // Массив указателей на детей
    int childrenCount;                      // Текущее количество детей

    Node(int data) : value(data), children(nullptr), childrenCount(0) {}

    ~Node() {
        for (int i = 0; i < childrenCount; i++) {
            delete children[i];
        }
        delete[] children;
    }

    // Функция для добавления ребенка
    void addChild(Node* child) {
        Node** newChildren = new Node * [childrenCount + 1];      // Расширяем массив детей

        for (int i = 0; i < childrenCount; i++) {                 // Копируем старых детей в новый массив
            newChildren[i] = children[i];
        }

        newChildren[childrenCount] = child;                       // Добавляем нового ребенк

        delete[] children;                                        // Удаляем старый массив и переназначаем
        children = newChildren;
        childrenCount++;
    }
};

class ArbitraryTree {
public:
    Node* root;

    ArbitraryTree() : root(nullptr) {}

    ~ArbitraryTree() {
        deleteTree(root);
    }

    void deleteTree(Node* node) {
        if (!node) return;
        delete node;
    }

    void createRoot(int value) {
        root = new Node(value);
    }

    // Рекурсивная функция для вывода дерева
    void printTree(Node* node, int level = 0) {
        if (!node) return;

        for (int i = 0; i < level; i++) cout << "  ";       // Печать текущего узла с отступами по уровню
        cout << node->value << " -> ";

        for (int i = 0; i < node->childrenCount; i++) {     // Печать значений всех детей текущего узла
            cout << node->children[i]->value << " ";
        }
        cout << endl;
            
        for (int i = 0; i < node->childrenCount; i++) {     // Рекурсивный вызов для каждого ребенка
            printTree(node->children[i], level + 1);
        }
    }

    // Функция для поиска узла по значению
    Node* findNode(Node* node, int value) {
        if (!node) return nullptr;
        if (node->value == value) return node;

        for (int i = 0; i < node->childrenCount; i++) {
            Node* found = findNode(node->children[i], value);
            if (found) return found;
        }
        return nullptr;
    }

    // Функция для загрузки дерева из файла
    bool loadTree(ifstream& file) {
        int rootValue;

        if (!(file >> rootValue)) {
            cerr << "Ошибка: некорректный ввод для корня! Ожидалось число." << endl;
            exit(EXIT_FAILURE);
        }

        createRoot(rootValue);

        int parentValue, childValue;
        while (file) {
            if (!(file >> parentValue >> childValue)) {
                if (file.eof()) break;
                cerr << "Ошибка: некорректные данные! Ожидалось число." << endl;
                exit(1);
            }

            Node* parent = findNode(root, parentValue);
            if (parent) {
                Node* child = new Node(childValue);
                parent->addChild(child);
            }
        }

        return true;
    }

    // Рекурсивная функция для нахождения минимальной глубины листьев
    void findMinLeafDepth(Node* node, int currentDepth, int& minDepth) {
        if (!node) return;

        // Если это лист (узел без детей)
        if (node->childrenCount == 0) {
            if (minDepth == -1 || currentDepth < minDepth) {
                minDepth = currentDepth;  // Обновляем минимальную глубину
            }
        }

        // Рекурсивно проверяем всех детей
        for (int i = 0; i < node->childrenCount; i++) {
            findMinLeafDepth(node->children[i], currentDepth + 1, minDepth);
        }
    }

    // Рекурсивная функция для удаления листьев на минимальной глубине
    void removeLeavesAtMinDepth(Node*& node, int currentDepth, int minDepth) {
        if (!node) return;

        if (node->childrenCount == 0 && currentDepth == minDepth) {   // Если это лист на минимальной глубине, удаляем его
            cout << "Удаляем вершину: " << node->value << " " << currentDepth << " узла(ов)" << endl;
            delete node;
            node = nullptr;
            return;
        }

        for (int i = 0; i < node->childrenCount; i++) {               // Рекурсивно удаляем детей на минимальной глубине
            removeLeavesAtMinDepth(node->children[i], currentDepth + 1, minDepth);
        }

        int newCount = 0;                                             // Перестроение массива детей: удаляем nullptr-ы
        for (int i = 0; i < node->childrenCount; i++) {
            if (node->children[i]) {
                node->children[newCount++] = node->children[i];
            }
        }
        node->childrenCount = newCount;
    }

    // Основная функция для удаления листьев на минимальной глубине
    void removeClosestLeaves() {
        if (!root) return;

        int minDepth = -1;

        findMinLeafDepth(root, 0, minDepth);

        if (minDepth == -1) return;

        removeLeavesAtMinDepth(root, 0, minDepth);
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    ArbitraryTree tree;
    ifstream file("file.txt");

    if (!file) {
        cerr << "Файл не открыт!" << endl;
        return 1;
    }

    if (!tree.loadTree(file)) {
        file.close();
        return 1;
    }
    file.close();

    cout << "Исходное дерево:" << endl;
    tree.printTree(tree.root);

    tree.removeClosestLeaves();  // Удаляем листья на минимальной глубине

    cout << "Конечное дерево:" << endl;
    tree.printTree(tree.root);

    return 0;
}
