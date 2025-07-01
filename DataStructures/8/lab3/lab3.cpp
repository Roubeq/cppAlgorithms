#include <iostream>
#include <fstream>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {} // Конструктор узла
};

// Класс бинарного дерева
class BinaryTree {
public:
    Node* root;

    BinaryTree() : root(nullptr) {} // Конструктор дерева

    void addNode(int parent, int child) {
        if (parent == child) {
            std::cerr << "Ошибка: Цикл в дереве! Узел " << parent << " не может быть своим потомком." << std::endl;
            exit(1);
        }
        if (!root) {
            root = new Node(parent);
        }

        Node* parentNode = findNode(root, parent);
        if (!parentNode) {
            std::cerr << "Ошибка: Родитель " << parent << " не найден в дереве!" << std::endl;
            exit(1);
        }


        // Проверка на дубликат
        if ((parentNode->left && parentNode->left->data == child) ||
            (parentNode->right && parentNode->right->data == child)) {
            std::cerr << "Ошибка: Дубликат ребенка " << child << " у родителя " << parent << std::endl;
            exit(1); // Немедленно завершаем программу
        }

        if (!parentNode->left) parentNode->left = new Node(child);
        else if (!parentNode->right) parentNode->right = new Node(child);
    }



    void searchSubtree(Node* subRoot) {
        if (!subRoot) return;
        checkSubtree(root, subRoot);
    }

    void print() {
        if (!root) {
            std::cout << "Дерево пустое." << std::endl;
        }
        else {
            printTree(root);
        }
    }

    ~BinaryTree() {
        destroyTree(root); // Очистка памяти
    }

private:
    Node* findNode(Node* node, int value) {
        if (!node) return nullptr;
        if (node->data == value) return node;           // Если нашли нужный узел, возвращаем его
        Node* found = findNode(node->left, value);      // Ищем в левом поддереве
        return found ? found : findNode(node->right, value); // Если не нашли, ищем в правом
    }

    bool isIdentical(Node* t1, Node* t2) {
        if (!t1 && !t2) return true;                    // Оба узла NULL → одинаковые
        if (!t1 || !t2 || t1->data != t2->data) return false; // Один NULL или данные разные → не совпадают
        return isIdentical(t1->left, t2->left) && isIdentical(t1->right, t2->right); // Рекурсивная проверка поддеревьев
    }

    bool isStructurallyIdentical(Node* t1, Node* t2) {
        if (!t1 && !t2) return true;
        if (!t1 || !t2) return false;
        return isStructurallyIdentical(t1->left, t2->left) && isStructurallyIdentical(t1->right, t2->right);
    }

    void checkSubtree(Node* mainNode, Node* subRoot) {
        if (!mainNode) return;
        if (isStructurallyIdentical(mainNode, subRoot)) {
            std::cout << "Найдено совпадение в узле: " << mainNode->data << std::endl;
            printTree(mainNode);
        }
        checkSubtree(mainNode->left, subRoot);          // Проверяем левое поддерево
        checkSubtree(mainNode->right, subRoot);         // Проверяем правое поддерево
    }

    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);                        // Удаляем левое поддерево
        destroyTree(node->right);                       // Удаляем правое поддерево
        delete node;                                    // Удаляем сам узел
    }

    void printTree(Node* node) {
        if (!node) return;

        if (node->left) std::cout << node->data << " " << node->left->data << std::endl;
        if (node->right) std::cout << node->data << " " << node->right->data << std::endl;

        printTree(node->left);                          // Выводим левое поддерево
        printTree(node->right);                         // Выводим правое поддерево
    }
};

// Чтение дерева из файла с проверкой на корректность ввода
void loadTree(std::ifstream& file, BinaryTree& tree) {
    int nodeCount;
    char ch;

    file >> ch;
    if (!isdigit(ch)) {
        std::cerr << "Ошибка: некорректный ввод!" << std::endl;
        exit(1);
    }
    file.putback(ch);                   // Возвращаем символ обратно в поток
    file >> nodeCount;

    int parent, child;
    char separator;
    while (file >> ch) {
        if (!isdigit(ch)) {             // Проверяем, не является ли символ буквой
            std::cerr << "Ошибка: некорректный ввод!" << std::endl;
            exit(1);
        }

        file.putback(ch);               // Возвращаем символ обратно в поток
        file >> parent >> child;

        tree.addNode(parent, child);

        file >> separator;
        if (separator == '.') break;    // Если '.', значит, конец ввода дерева
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }

    BinaryTree mainTree, subTree;

    loadTree(inputFile, mainTree);
    loadTree(inputFile, subTree);
    std::cout << "Основное дерево: " << std::endl;
    mainTree.print();
    std::cout << "Поддерево: " << std::endl;
    subTree.print();
    inputFile.close();

    std::cout << "Ищем совпадающие поддеревья..." << std::endl;
    mainTree.searchSubtree(subTree.root);

    return 0;
}