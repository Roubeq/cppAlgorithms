#include <iostream>
#include <fstream>

struct Node {                                  // Узел дерева
    int nodeNumber;
    int childCount;
    Node** children;

    Node(int value, int numChildren) : nodeNumber(value), childCount(numChildren) {
        children = (numChildren > 0) ? new Node * [numChildren]() : nullptr;
    }

    ~Node() {
        delete[] children;
    }
};

class ArbitraryTree {                          // Класс произвольного дерева
public:
    Node* root;

    ArbitraryTree() : root(nullptr) {}

    void createRoot(int value, int numChildren) {                       // Создание корня дерева
        root = new Node(value, numChildren);
    }

    void addChild(Node* parent, int value, int numChildren) {           // Добавление потомков
        if (!parent || parent->childCount == 0) return;

        for (int i = 0; i < parent->childCount; i++) {
            if (parent->children[i] == nullptr) {
                parent->children[i] = new Node(value, numChildren);
                return;
            }
        }   
    }

    void updateValues(Node* node, int parentSum = 0) {                  // Обновление значений в узлах
        if (!node) return;                                              // (В каждую вершину вставляет сумму значений всех его предков)
        node->nodeNumber += parentSum;
        for (int i = 0; i < node->childCount; i++) {
            updateValues(node->children[i], node->nodeNumber);
        }
    }
    
    void printTree(Node* node, int level = 0) {                         // Вывод дерева
        if (!node) return;

        for (int i = 0; i < level; i++) std::cout << "  ";
        std::cout << node->nodeNumber << " -> ";

        for (int i = 0; i < node->childCount; i++) {
            if (node->children[i]) std::cout << node->children[i]->nodeNumber << " ";
        }
        std::cout << std::endl;

        for (int i = 0; i < node->childCount; i++) {
            printTree(node->children[i], level + 1);
        }
    }

    ~ArbitraryTree() {
        destroyTree(root);
    }

private:
    void destroyTree(Node* node) {
        if (!node) return;
        for (int i = 0; i < node->childCount; i++) {
            destroyTree(node->children[i]);
        }
        delete node;
    }
};

bool getValidNumber(std::ifstream& file, int& number) {            // Читаем число посимвольно и проверяем корректность    
    number = 0;
    bool foundDigit = false;
    char ch;

    while (file.get(ch)) {
        if (ch >= '0' && ch <= '9') {                              // Проверяем, цифра ли это
            foundDigit = true;
            number = number * 10 + (ch - '0');                     // Преобразуем в число
        }
        else if (ch == ' ' || ch == '\n' || ch == '\t') {
            if (foundDigit) break;                                 // Если нашли цифру, а потом пробел, то выходим
        }
        else {
            std::cerr << "Некорректный ввод!" << std::endl;
            return false;
            exit(1);
        }
    }

    return foundDigit;
}

Node* findNode(Node* node, int value) {                            // Функция поиска узла по значению
    if (!node) return nullptr;
    if (node->nodeNumber == value) return node;
    for (int i = 0; i < node->childCount; i++) {
        Node* found = findNode(node->children[i], value);
        if (found) return found;
    }
    return nullptr;
}

bool loadTree(std::ifstream& file, ArbitraryTree& tree) {          // Чтение дерева из файла с валидацией
    int rootValue, rootChildren;

    if (!getValidNumber(file, rootValue) || !getValidNumber(file, rootChildren)) {
        std::cerr << "Некорректный ввод!" << std::endl;
        return false;
    }

    tree.createRoot(rootValue, rootChildren);

    int parentValue, childValue, childCount;
    while (getValidNumber(file, parentValue) && getValidNumber(file, childValue) && getValidNumber(file, childCount)) {
        Node* parent = findNode(tree.root, parentValue);
        if (parent) {
            tree.addChild(parent, childValue, childCount);
        }
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::ifstream inputFile("file.txt");
    if (!inputFile) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }

    ArbitraryTree tree;
    if (!loadTree(inputFile, tree)) {
        inputFile.close();
        exit(1);
    }
    inputFile.close();

    std::cout << "Исходное дерево:" << std::endl;
    tree.printTree(tree.root);

    tree.updateValues(tree.root);

    std::cout << "\nДерево после обновления значений:" << std::endl;
    tree.printTree(tree.root);

    return 0;
}
