#include <iostream>
#include <fstream>

using namespace std;

struct Node {
    int value;
    int actualChildrenCount;
    Node** children;

    Node(int data) : value(data), actualChildrenCount(0), children(nullptr) {}

    ~Node() {
        for (int i = 0; i < actualChildrenCount; i++) {
            delete children[i];
        }
        delete[] children;
    }

    void addChild(Node* child) {
        Node** newChildren = new Node * [actualChildrenCount + 1];
        for (int i = 0; i < actualChildrenCount; i++) {
            newChildren[i] = children[i];
        }
        newChildren[actualChildrenCount] = child;
        delete[] children;
        children = newChildren;
        actualChildrenCount++;
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

    void addChild(Node* parent, int value) {
        if (!parent) return;
        parent->addChild(new Node(value));
    }

    void printTree(Node* node) {
        if (!node) return;

        for (int i = 0; i < node->actualChildrenCount; i++) {
            cout << node->value << " " << node->children[i]->value << endl;
            printTree(node->children[i]);
        }
    }

    void findMaxBranches(Node* node, int& maxBranches) {
        if (!node) return;

        if (node->actualChildrenCount > maxBranches) {
            maxBranches = node->actualChildrenCount;
        }

        for (int i = 0; i < node->actualChildrenCount; i++) {
            findMaxBranches(node->children[i], maxBranches);
        }
    }

    void printMaxBranches(Node* node, int maxBranches) {
        if (!node) return;

        if (node->actualChildrenCount == maxBranches) {
            cout << "Ветвь от узла " << node->value << ":\n";
            printTree(node);
            cout << endl;
        }

        for (int i = 0; i < node->actualChildrenCount; i++) {
            printMaxBranches(node->children[i], maxBranches);
        }
    }

    void printMaxBranches() {
        if (!root) {
            cout << "Дерево пустое или не содержит ветвлений." << endl;
            return;
        }

        int maxBranches = 0;
        findMaxBranches(root, maxBranches);

        if (maxBranches == 0) {
            cout << "Дерево пустое или не содержит ветвлений." << endl;
        }
        else {
            cout << "Все ветви с максимальным числом детей (" << maxBranches << "):\n";
            printMaxBranches(root, maxBranches);
        }
    }
};

bool getValidNumber(ifstream& file, int& number) {
    number = 0;
    bool foundDigit = false;
    char ch;

    while (file.get(ch)) {
        if (ch >= '0' && ch <= '9') {
            foundDigit = true;
            number = number * 10 + (ch - '0');
        }
        else if (ch == ' ' || ch == '\n' || ch == '\t') {
            if (foundDigit) break;
        }
        else {
            cerr << "Некорректный ввод!" << endl;
            exit(1);
            return false;
        }
    }
    return foundDigit;
}

Node* findNode(Node* node, int value) {
    if (!node) return nullptr;

    if (node->value == value) return node;

    for (int i = 0; i < node->actualChildrenCount; i++) {
        Node* found = findNode(node->children[i], value);
        if (found) return found;
    }
    return nullptr;
}

bool loadTree(ifstream& file, ArbitraryTree& tree) {
    int rootValue;

    if (!getValidNumber(file, rootValue)) {
        cerr << "Некорректный ввод!" << endl;
        return false;
    }

    tree.createRoot(rootValue);

    int parentValue, childValue;
    while (getValidNumber(file, parentValue) && getValidNumber(file, childValue)) {
        Node* parent = findNode(tree.root, parentValue);
        if (parent) {
            tree.addChild(parent, childValue);
        }
        else {
            cerr << "Ошибка: не найден родитель с значением " << parentValue << endl;
        }
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ArbitraryTree tree;
    ifstream file("file.txt");

    if (!file) {
        cerr << "Файл не открыт!" << endl;
        return 1;
    }

    if (!loadTree(file, tree)) {
        file.close();
        return 1;
    }
    file.close();

    cout << "Исходное дерево:\n";
    tree.printTree(tree.root);

    tree.printMaxBranches();

    return 0;
}
