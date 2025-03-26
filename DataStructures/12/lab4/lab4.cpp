#include <iostream>
#include <fstream>

using namespace std;

struct Node {
    int value;
    int childCount;
    Node** children;

    Node(int data) : value(data), childCount(0), children(nullptr) {}

    ~Node() {
        for (int i = 0; i < childCount; i++) {
            delete children[i];
        }
        delete[] children;
    }

    void appendChild(Node* child) {
        Node** tempChildren = new Node * [childCount + 1];
        for (int i = 0; i < childCount; i++) {
            tempChildren[i] = children[i];
        }
        tempChildren[childCount] = child;
        delete[] children;
        children = tempChildren;
        childCount++;
    }
};

class TreeStructure {
public:
    Node* root;

    TreeStructure() : root(nullptr) {}

    ~TreeStructure() {
        clearTree(root);
    }

    void clearTree(Node* node) {
        if (!node) return;
        delete node;
    }

    void setRoot(int value) {
        root = new Node(value);
    }

    void insertChild(Node* parent, int value) {
        if (!parent) return;
        parent->appendChild(new Node(value));
    }

    void printTree(Node* node, int depth = 0) {
        if (!node) return;

        // Отступы для визуального представления иерархии
        for (int i = 0; i < depth; i++) {
            cout << "  ";
        }

        // Вывод узла
        cout << node->value << endl;

        // Рекурсивный вызов для всех дочерних элементов
        for (int i = 0; i < node->childCount; i++) {
            printTree(node->children[i], depth + 1);
        }
    }

    void findLongestPaths(Node* node, int* path, int length, int& maxLength, int**& allPaths, int& pathCount, bool isNewPath) {
        if (!node) return;

        int* newPath;
        if (isNewPath) {
            newPath = new int[1];
            newPath[0] = node->value;
            length = 1;
        }
        else {
            newPath = new int[length + 1];
            for (int i = 0; i < length; i++) {
                newPath[i] = path[i];
            }
            newPath[length] = node->value;
            length++;
        }

        if (length > maxLength) {
            maxLength = length;
            for (int i = 0; i < pathCount; i++) {
                delete[] allPaths[i];
            }
            pathCount = 0;
        }

        if (length == maxLength) {
            int** tempPaths = new int* [pathCount + 1];
            for (int i = 0; i < pathCount; i++) {
                tempPaths[i] = allPaths[i];
            }
            tempPaths[pathCount] = new int[maxLength];
            for (int i = 0; i < maxLength; i++) {
                tempPaths[pathCount][i] = newPath[i];
            }
            delete[] allPaths;
            allPaths = tempPaths;
            pathCount++;
        }

        for (int i = 0; i < node->childCount; i++) {
            findLongestPaths(node->children[i], newPath, length, maxLength, allPaths, pathCount, node->childCount > 1);
        }

        delete[] newPath;
    }

    void processLongestPaths() {
        if (!root) return;

        int maxLength = 0;
        int** allPaths = nullptr;
        int pathCount = 0;
        int* path = nullptr;

        findLongestPaths(root, path, 0, maxLength, allPaths, pathCount, true);

        if (allPaths) {
            cout << "\nСамые длинные пути без ветвлений (длина: " << maxLength << "):" << endl;
            for (int i = 0; i < pathCount; i++) {
                for (int j = 0; j < maxLength; j++) {
                    cout << allPaths[i][j] << " ";
                }
                cout << endl;
                delete[] allPaths[i];
            }
            delete[] allPaths;
        }
    }
};

bool readNumber(ifstream& file, int& number) {
    number = 0;
    bool digitFound = false;
    char ch;

    while (file.get(ch)) {
        if (ch >= '0' && ch <= '9') {
            digitFound = true;
            number = number * 10 + (ch - '0');
        }
        else if (ch == ' ' || ch == '\n' || ch == '\t') {
            if (digitFound) break;
        }
        else {
            cerr << "Ошибка ввода!" << endl;
            exit(1);
            return false;
        }
    }
    return digitFound;
}

Node* locateNode(Node* node, int value) {
    if (!node) return nullptr;
    if (node->value == value) return node;

    for (int i = 0; i < node->childCount; i++) {
        Node* found = locateNode(node->children[i], value);
        if (found) return found;
    }
    return nullptr;
}

bool constructTree(ifstream& file, TreeStructure& tree) {
    int rootValue;
    if (!readNumber(file, rootValue)) {
        cerr << "Ошибка ввода!" << endl;
        return false;
    }

    tree.setRoot(rootValue);

    int parentValue, childValue;
    while (readNumber(file, parentValue) && readNumber(file, childValue)) {
        Node* parent = locateNode(tree.root, parentValue);
        if (parent) {
            tree.insertChild(parent, childValue);
        }
        else {
            cerr << "Ошибка: не найден родитель с значением " << parentValue << endl;
        }
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");
    TreeStructure tree;
    ifstream file("file.txt");

    if (!file) {
        cerr << "Не удалось открыть файл!" << endl;
        return 1;
    }

    if (!constructTree(file, tree)) {
        file.close();
        return 1;
    }
    file.close();

    // Вывод дерева
    cout << "Исходное дерево:\n";
    tree.printTree(tree.root);

    // Поиск самых длинных путей без ветвлений
    tree.processLongestPaths();

    return 0;
}
