#include <iostream>

using namespace std;

const int MAX = 5;

int stackArr[MAX];
int top = -1;

bool isFull() {
    if (top == MAX - 1) {
        return 1;
    }
    else {
        return 0;
    }
}

bool isEmpty() {
    if (top == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

int peek() {
    if (isEmpty()) {
        cout << "Stack is empty";
        exit(1);
    }
    return stackArr[top];
}

void push(int value) {

    if (isFull()) {
        cout << "Stack overflow" << endl;
        return;
    }
    top = top + 1;
    stackArr[top] = value;
}

int pop() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    int value;
    value = stackArr[top];
    top = top - 1;
    return value;
}

void printStack() {
    if (top == -1) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    for (int i = top; i >= 0; i--) {
        cout << pop() << " ";
    }
}

int main() {
    push(1);
    push(2);
    push(3);
    pop();
    printStack();
    return 0;
}
