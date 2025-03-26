#include <iostream>
#include <fstream>

// Узел бинарного дерева
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Класс бинарного дерева
class BinaryTree {
public:
    Node* root;

    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
        destroyTree(root);
    }

    // Функция для вставки узлов вручную по родителю
    void insert(int parent, int child) {
        if (!root) {
            root = new Node(parent);
        }

        Node* parentNode = findOrCreateNode(root, parent);
        if (!parentNode) {
            std::cerr << "Ошибка: Родитель " << parent << " не найден!" << std::endl;
            exit(1);
        }

        if (child < parentNode->data) {
            if (!parentNode->left) {
                parentNode->left = new Node(child);
            }
            else {
                std::cerr << "Ошибка: Левый потомок уже существует!" << std::endl;
                exit(1);
            }
        }
        else {
            if (!parentNode->right) {
                parentNode->right = new Node(child);
            }
            else {
                std::cerr << "Ошибка: Правый потомок уже существует!" << std::endl;
                exit(1);
            }
        }

    }

    // Удаление узлов и их поддеревьев
    Node* removeNodes(Node* node, int* values, int size) {
        if (!node) return nullptr;

        // Проверяем, нужно ли удалить текущий узел
        for (int i = 0; i < size; i++) {
            if (node->data == values[i]) {
                std::cout << "Удаляем узел: " << node->data << " и его поддерево" << std::endl;
                destroyTree(node);  // Полностью удаляем узел и все его поддеревья
                return nullptr;
            }
        }

        // Рекурсивно обрабатываем поддеревья (только если узел не удалён)
        node->left = removeNodes(node->left, values, size);
        node->right = removeNodes(node->right, values, size);

        return node;
    }

    // Вывод дерева в виде списка родитель -> потомок
    void printTree(Node* node) {
        if (!node) return;
        if (node->left) std::cout << node->data << " " << node->left->data << std::endl;
        if (node->right) std::cout << node->data << " " << node->right->data << std::endl;
        printTree(node->left);
        printTree(node->right);
    }




    // Выводит информацию, если дерево пустое
    void printTreeOrEmpty() {
        if (!root) {
            std::cout << "Дерево пустое." << std::endl;
        }
        else {
            printTree(root);
            if (!root->left && !root->right) {
                std::cout << "Корень: " << root->data << " (без потомков)" << std::endl;
            }
        }
    }

private:
    // Поиск узла или создание нового, если его нет
    Node* findOrCreateNode(Node* node, int value) {
        if (!node) return nullptr;  // Не создаём новый узел здесь

        if (node->data == value) return node;

        Node* found = findOrCreateNode(node->left, value);
        if (!found) found = findOrCreateNode(node->right, value);

        return found;
    }

    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
};

// Находит первый узел с потомками, но не корень
Node* findFirstSubtree(Node* node) {
    if (!node) return nullptr;

    if (node->left) return node->left; // Берём левое поддерево, если оно есть
    if (node->right) return node->right; // Иначе правое поддерево

    return nullptr;
}

// Выводит найденное поддерево (если оно есть)
void findAndPrintFirstSubtree(BinaryTree& tree) {
    if (!tree.root) {
        std::cout << "Дерево пустое." << std::endl;
        return;
    }

    Node* subtreeRoot = findFirstSubtree(tree.root);
    if (subtreeRoot) {
        std::cout << "\nПервое найденное поддерево:\n";
        tree.printTree(subtreeRoot);
    }
    else {
        std::cout << "Нет подходящего поддерева.\n";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    BinaryTree tree;
    std::ifstream inputFile("input.txt");

    if (!inputFile) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }

    int parent, child;
    while (inputFile >> parent >> child) {
        if (inputFile.fail()) {
            std::cerr << "Ошибка: Найден некорректный символ в файле!" << std::endl;
            return 1;
        }
        tree.insert(parent, child);

    }

    // Пропуск символа |
    char separator;
    inputFile.clear();
    inputFile >> separator;

    if (!isdigit(separator) && separator != '|') {
        std::cerr << "Ошибка: Найден некорректный символ в файле!" << std::endl;
        return 1;
    }

    int count;
    inputFile >> count;

    int* valuesToRemove = new int[count];
    for (int i = 0; i < count; i++) {
        inputFile >> valuesToRemove[i];
        if (inputFile.fail()) {
            std::cerr << "Ошибка: Найден некорректный символ в файле!" << std::endl;
            delete[] valuesToRemove;
            return 1;
        }
    }
    inputFile.close();

    std::cout << "Исходное дерево:" << std::endl;
    tree.printTreeOrEmpty();

    tree.root = tree.removeNodes(tree.root, valuesToRemove, count);
    delete[] valuesToRemove;

    std::cout << "\nДерево после удаления:" << std::endl;
    tree.printTreeOrEmpty();

    findAndPrintFirstSubtree(tree);

    return 0;
}