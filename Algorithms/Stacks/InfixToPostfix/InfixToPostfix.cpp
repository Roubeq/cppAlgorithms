#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct Node {
    int data;
    Node* next;
};

bool isEmpty(Node* stack) {
    return stack == nullptr;
}

void push(Node*& stack, int data) {
    Node* temp = new Node;
    temp->data = data;
    temp->next = stack;
    stack = temp;
}

int pop(Node*& stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    Node* temp = stack;
    stack = stack->next;
    int val = temp->data;
    delete temp;
    return val;
}

void print(Node* stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return;
    }
    Node* temp = stack;
    while (temp != nullptr) {
        cout << (char)temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

bool isOperand(int data) {
    char c = (char)data;
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isOperator(int data) {
    char c = (char)data;
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int priority(int data) {
    char c = (char)data;
    if (c == '^') return 4;
    if (c == '*' || c == '/') return 3;
    if (c == '+' || c == '-') return 2;
    return 0;
}

bool isRightAssociative(int data) {
    char c = (char)data;
    return c == '^';
}

Node* infixToPostfix(Node*& input) {
    Node* postfix = nullptr;
    Node* operators = nullptr;


    while (!isEmpty(input)) {
        int data = pop(input);

        if (isOperand(data)) {
            push(postfix, data);
        }
        else if ((char)data == '(') {
            push(operators, data);
        }
        else if ((char)data == ')') {
            while (!isEmpty(operators) && (char)operators->data != '(') {
                push(postfix, pop(operators));
            }
            if (!isEmpty(operators)) pop(operators);
        }
        else if (isOperator(data)) {
            while (!isEmpty(operators) &&
                (char)operators->data != '(' &&
                (priority(operators->data) > priority(data) ||
                    (priority(operators->data) == priority(data) && !isRightAssociative(data)))) {
                push(postfix, pop(operators));
            }
            push(operators, data);
        }
    }

    while (!isEmpty(operators)) {
        push(postfix, pop(operators));
    }

    Node* result = nullptr;
    while (!isEmpty(postfix)) {
        push(result, pop(postfix));
    }

    return result;
}

int evaluatePostfix(Node* postfix) {
    Node* stack = nullptr;
    Node* temp = postfix;

    Node* reversed = nullptr;
    while (!isEmpty(temp)) {
        push(reversed, pop(temp));
    }
    temp = reversed;

    while (!isEmpty(temp)) {
        int data = pop(temp);
        if (isOperand(data) && (char)data >= '0' && (char)data <= '9') {
            push(stack, data - '0');
        }
        else if (isOperator(data)) {
            int b = pop(stack);
            int a = pop(stack);
            switch ((char)data) {
            case '+': push(stack, a + b); break;
            case '-': push(stack, a - b); break;
            case '*': push(stack, a * b); break;
            case '/': push(stack, a / b); break;
            case '^': push(stack, (int)pow(a, b)); break;
            }
        }
    }

    int result = pop(stack);
    if (!isEmpty(stack)) {
        cout << "Invalid expression" << endl;
        exit(1);
    }
    return result;
}

int main() {
    string expression;
    cout << "Enter an infix expression (e.g., (3+1)*2): ";
    getline(cin, expression);

    Node* input = nullptr;
    for (int i = expression.length() - 1; i >= 0; i--) {
        if (expression[i] != ' ') {
            push(input, expression[i]);
        }
    }

    Node* postfix = infixToPostfix(input);
    cout << "Postfix notation: ";
    print(postfix);

    Node* postfixCopy = nullptr;
    Node* temp = postfix;
    while (!isEmpty(temp)) {
        push(postfixCopy, pop(temp));
    }

    cout << "Result: " << evaluatePostfix(postfixCopy) << endl;

    return 0;
}