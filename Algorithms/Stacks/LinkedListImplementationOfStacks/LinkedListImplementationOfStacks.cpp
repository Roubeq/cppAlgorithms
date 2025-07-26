#include <iostream>

using namespace std;

struct Node {
    int data;
    Node* next;
};

Node *top = nullptr;


bool isEmpty() {
    if (top == nullptr) {
        return 1;
    }
    else {
        return 0;
    }
}

void push(int data) {
    Node* newNode = nullptr;
    newNode->data = data;
    if (isEmpty()) {
        top = newNode;
    }
    else {
        newNode->next = top;
        top = newNode;
    }
}

int peek() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    return top->data;
}

int pop() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    Node* temp = top;
    int val = temp->data;
    top = top->next;
    delete(temp);
    return val;
}

void print() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        return;
    }
    Node* temp = top;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;

}

int main()
{
    
}
