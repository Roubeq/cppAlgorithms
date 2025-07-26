#include <iostream>

using namespace std;

struct Node {
    int data;
    Node* next;
};

Node* top = nullptr;

bool isEmpty() {
    if (top == nullptr) {
        return 1;
    }
    else {
        return 0;
    }
}

void push(int data) {
    Node* temp = new Node;
    temp->data = data;
    temp->next = top;

    top = temp;
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
    temp = nullptr;
    return val;
}

bool matchChar(char a, char b) {
    if (a == '(' && b == ')') {
        return 1;
    }
    if (a == '[' && b == ']') {
        return 1;
    }
    if (a == '{' && b == '}') {
        return 1;
    }
    return 0;
}

int checkNestedBrackets(char* s) {
    char temp;
    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '{' || s[i] == '[' || s[i] == '(') {
            push(s[i]);
        }
        if (s[i] == '}' || s[i] == ']' || s[i] == ')') {
            if (isEmpty()) {
                cout << "Right brackets are more than left brackets" << endl;
                return 0;
            }
            else {
                temp = pop();
                if (!matchChar(temp, s[i])) {
                    cout << "Mismatched char!" << endl;
                    return 0;
                }
            }
        }
    }
    if (isEmpty()) {
        cout << "Expression is valid!" << endl;
        return 1;
    }
    else {
        cout << "Invalid expression!" << endl;
        return 0;
    }
}

int main() {
    char expr[100];
    cout << "Please enter the expression: ";
    cin >> expr;
    int balanced = checkNestedBrackets(expr);
    if (balanced == 1) {
        cout << "The expression is valid";
    }
    else {
        cout << "The expression is invalid";
    }
}
