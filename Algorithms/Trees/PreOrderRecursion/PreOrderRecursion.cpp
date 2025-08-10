#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

struct Node {
	int data;
	Node* left;
	Node* right;

	Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class Tree {
private:
	Node* root;

    void preOrderTraversal(Node*& treeNode) {
        if (treeNode == nullptr) {
            return;
        }
        cout << treeNode->data << " ";
        preOrderTraversal(treeNode->left);
        preOrderTraversal(treeNode->right);
    }

    void inOrderTraversal(Node*& treeNode) {
        if (treeNode == nullptr) {
            return;
        }
        inOrderTraversal(treeNode->left);
        cout << treeNode->data << " ";
        inOrderTraversal(treeNode->right);
    }

    void postOrderTraversal(Node*& treeNode) {
        if (treeNode == nullptr) {
            return;
        }
        postOrderTraversal(treeNode->left);
        postOrderTraversal(treeNode->right);
        cout << treeNode->data << " ";
    }

    Node* insertRec(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (value < node->data) {
            node->left = insertRec(node->left, value);
        }
        else if (value > node->data) {
            node->right = insertRec(node->right, value);
        }

        return node;
    }
public:
	Tree() : root(nullptr) {}

    void insert(int value) {
        root = insertRec(root, value);
    }

    bool searchNode(int value) {
        Node* current = root;
        while (current != nullptr) {
            if (value == current->data) {
                return true;
            } 
            else if (value < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return false;
    }

    void preOrderTraversal() {
        preOrderTraversal(root);
    }

    void inOrderTraversal() {
        inOrderTraversal(root);
    }

    void postOrderTraversal() {
        postOrderTraversal(root);
    }
    
    void inOrderIterative() {
        Node* current = root;
        stack<Node*> stack;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push(current);
                current = current->left;
            }
            current = stack.top();
            cout << current->data << " ";
            stack.pop();
            current = current->right;
        }
    }

    void preOrderIterative() {
        stack<Node*> stack;
        stack.push(root);

        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();

            cout << current->data << " ";

            if (current->right) {
                stack.push(current->right);
            }
            if (current->left) {
                stack.push(current->left);
            }
        }
    }

    void postOrderIterative() {
        if (root == nullptr) return;

        stack<Node*> s1, s2;
        s1.push(root);

        while (!s1.empty()) {
            Node* current = s1.top();
            s1.pop();
            s2.push(current);

            if (current->left) {
                s1.push(current->left);
            }
            if (current->right) {
                s1.push(current->right);
            }
        }
        while (!s2.empty()) {
            cout << s2.top()->data << " ";
            s2.pop();
        }
    }

    void BFS() {
        if (root == nullptr) return;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            cout << current->data << " ";

            if (current->left) {
                q.push(current->left);
            }
            if (current->right) {
                q.push(current->right);

            }
        }
    }

    void DFS() {
        // это всё то что было реализовано до(in-order, pre-order, post-order)
    }

};

int main() {
    Tree tree;

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    cout << "Traversal: ";
    //tree.preOrderTraversal();
    //tree.inOrderTraversal();
    //tree.postOrderTraversal();
    //tree.inOrderIterative();
    //tree.preOrderIterative();
    //tree.postOrderIterative();
    tree.BFS();

    return 0;
}