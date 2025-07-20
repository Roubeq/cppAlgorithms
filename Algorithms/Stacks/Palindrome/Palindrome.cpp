#include <iostream>

using namespace std;



struct Node {
    int data;
    Node* next;
};

Node* head = nullptr;

bool isEmpty() {
    if (head == nullptr) {
        return 1;
    }
    else {
        return 0;
    }
}

void push(int data) {
    Node* temp = new Node;
    temp->data = data;
    temp->next = head;
    head = temp;
}

int pop() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    Node* temp = head;
    int val = temp->data;
    head = head->next;
    delete(temp);
    temp = nullptr;
    return val;
}

void palindromeCheck(char* s) {
    int i = 0;
    while (s[i] != 'X') {
        push(s[i]);
        i++;
    }
    i++;
    while (s[i]) {
        if (isEmpty() || s[i] != pop()) {
            cout << "Not a palindrome" << endl;
            exit(1);
        }
        i++;
    }
    if (isEmpty()) {
        cout << "Palindrome" << endl;
    }
    else {
        cout << "Not a palindrome!" << endl;
    }
}

int main()
{
    char s[100];
    cout << "Please enter the string: " << endl;
    cin >> s;
    palindromeCheck(s);
    return 0;
}
