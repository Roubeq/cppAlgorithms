#include <iostream>

using namespace std;

struct Node {
    int data;
    Node* next;
};


bool isEmpty(Node * topSS) {
    if (topSS == nullptr) {
        return 1;
    }
    else {
        return 0;
    }
}

void push(int data, Node*& tops) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = tops;

    tops = newNode;
}

int peek(Node * topSS) {
    if (isEmpty(topSS)) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    return topSS->data;
}

int pop(Node*& topSS) {
    if (isEmpty(topSS)) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    Node* temp = topSS;
    int val = temp->data;
    topSS = topSS->next;
    delete(temp);
    return val;
}

void print(Node* topSS) {
    if (isEmpty(topSS)) {
        cout << "Stack is empty" << endl;
        return;
    }
    Node* temp = topSS;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;

}

void reverseStack(Node*& topSS) {
    Node* top2 = nullptr;
    Node* top3 = nullptr;
    while (topSS != nullptr) {
        push(pop(topSS), top2);
    }
    while (top2 != nullptr) {
        push(pop(top2), top3);
    }
    while (top3 != nullptr) {
        push(pop(top3), topSS);
    }
}
int main()
{
    Node* top = nullptr;
    push(1,top);
    push(2,top);
    push(3,top);
    reverseStack(top);
    print(top);
 }
