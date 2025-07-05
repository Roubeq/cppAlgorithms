#include <iostream>

using namespace std;

struct Node {
    int data;
    struct Node* next;
};

struct Node* addNode(struct Node* head, int val) {
    struct Node* newNode = new Node;
    newNode->data = val;
    newNode->next = head;
    head = newNode;

    return head;
}

struct Node* addingTwoNumbers(struct Node* head1, struct Node* head2) {
    if (head1 == nullptr || head2 == nullptr) {
        return head1;
    }
    struct Node* tempHead1 = head1;
    struct Node* tempHead2 = head2;
    struct Node* head3 = nullptr;
    int value, temp = 0;

    while (tempHead1 != nullptr || tempHead2 != nullptr) {
        value = 0;
        if (tempHead1 != nullptr) {
            value += tempHead1->data;
        }
        if (tempHead2 != nullptr) {
            value += tempHead2->data;
        }
        value += temp;
        if (value > 9) {
            temp = value / 10;
            value = value % 10;
        }
        else {
            temp = 0;
        }
        head3 = addNode(head3, value);
        if (tempHead1 != nullptr) {
            tempHead1 = tempHead1->next;
        }
        if (tempHead2 != nullptr) {
            tempHead2 = tempHead2->next;
        }
    }
    if (temp != 0) {
        head3 = addNode(head3, temp);
    }
    return head3;
}

struct Node* reverseLL(struct Node* head) {
    if (head == nullptr) {
        return head;
    }
    struct Node* next = nullptr;
    struct Node* current = nullptr;

    next = head->next;
    head->next = nullptr;

    while (next != nullptr) {
        current = next;
        next = current->next;
        current->next = head;
        head = current;
    }

    return head;
}

struct Node* createLL(struct Node* head, int val) {
    while (val != 0) {
        head = addNode(head, val % 10);
        val = val / 10;
    }
    return head;
}

void print(struct Node* head) {
    struct Node* temp = head;
    if (head == nullptr) {
        cout << "head is not have";
    }
    else {
        while (temp->next != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << temp->data << endl;
    }
    
}

void deleteList(struct Node* head) {
    while (head != nullptr) {
        struct Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main()
{
    int a, b;
    cout << "Enter the two numbers: ";
    cin >> a >> b;

    cout << "Linked list representation of first number: \n";
    struct Node* head1 = nullptr;
    head1 = createLL(head1, a);
    head1 = reverseLL(head1);
    print(head1);

    cout << "Linked list representation of second number: \n";
    struct Node* head2 = nullptr;
    head2 = createLL(head2, b);
    head2 = reverseLL(head2);
    print(head2);

    struct Node* head3 = addingTwoNumbers(head1, head2);
    print(head3);

    deleteList(head1);
    deleteList(head2);
    deleteList(head3);

    return 0;
}
