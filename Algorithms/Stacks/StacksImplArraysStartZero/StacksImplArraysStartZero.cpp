#include <iostream>

using namespace std;

const int MAX = 5;
int first = -1;
int stackArrZero[MAX];


bool isEmpty() {
    if (first == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

bool isFull() {
    if (first == MAX - 1) {
        return 1;
    }
    else {
        return 0;
    }
}

void push(int data) {
    if (isFull()) {
        cout << "Stack overflow" << endl;
        return;
    }
    first = first + 1;
    for (int i = first; i > 0; i--) {
        stackArrZero[i] = stackArrZero[i-1];
    }
    stackArrZero[0] = data;
}

int peek() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    else {
        return stackArrZero[0];
    }
}

int pop() {
    int i, value;
    value = stackArrZero[0];
    for (i = 0; i <= first; i++) {
        stackArrZero[i] = stackArrZero[i + 1];
    }
    first -= 1;
    return value;
}

void print() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        return;
    }
    for (int i = 0; i <= first; i++) {
        cout << stackArrZero[i];
    }
    cout << endl;
}

int main()
{
    push(1);
    push(2);
    push(3);
    int data = pop();
    print();
}
