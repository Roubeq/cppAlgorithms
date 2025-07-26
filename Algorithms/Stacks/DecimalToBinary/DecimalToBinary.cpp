#include <iostream>

using namespace std;

const int MAX = 100;
int stack[MAX];
int first = -1;


bool isFull() {
    if (first == MAX - 1) {
        return 1;
    }
    else {
        return 0;
    }
}

bool isEmpty() {
    if (first == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

void push(int data) {
    if (isFull()) {
        cout << "Stack is full" << endl;
        return;
    }
    first = first + 1;
    for (int i = first; i > 0; i--) {
        stack[i] = stack[i - 1];
    }
    stack[0] = data;
}

void print() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        return;
    }
    for (int i = 0; i <= first; i++) {
        cout << stack[i] << " ";
    }
    cout << endl;
}

void dec2Bin(int data) {
    if (isFull()) {
        cout << "Stack is full" << endl;
        return;
    }
    int value;
    while (data > 0) {
        push(data % 2);
        data = data / 2;
    }

}

int main()
{
    dec2Bin(77);
    print();
}