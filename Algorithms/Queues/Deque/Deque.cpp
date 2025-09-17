#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
};

struct Deque {
private:
    Node* front, * rear;
    
public:
    Deque() : front(nullptr), rear(nullptr) {}

    bool isEmpty() {
        return (front == nullptr && rear == nullptr);
    }

    void pushFront(int value) {
        Node* temp = new Node;
        temp->data = value;
        temp->next = front;
        
        if (isEmpty()) {
            front = rear = temp;
        } else {
            front = temp;
        }
    }

    void pushBack(int value) {
        Node* temp = new Node;
        temp->data = value;
        temp->next = nullptr;
        
        if (isEmpty()) {
            front = rear = temp;
        } else {
            rear->next = temp;
            rear = temp;
        }
    }

    int popFront() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return -1;
        }
        
        Node* temp = front;
        int value = front->data;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        return value;
    }

    int popBack() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return -1;
        }
        
        int value = rear->data;
        
        if (front == rear) {
            delete front;
            front = rear = nullptr;
            return value;
        }
        
        Node* temp = front;
        while (temp->next != rear) {
            temp = temp->next;
        }
        
        delete rear;
        rear = temp;
        rear->next = nullptr;
        
        return value;
    }

    int getFront() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return -1;
        }
        return front->data;
    }

    int getBack() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return -1;
        }
        return rear->data;
    }

    void display() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return;
        }
        
        Node* temp = front;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    ~Deque() {
        while (!isEmpty()) {
            popFront();
        }
    }
};

int main() {
    Deque dq;
    
    dq.pushFront(10);
    dq.pushBack(20);
    dq.pushFront(5);
    dq.pushBack(30);
    
    cout << "Deque: ";
    dq.display();
    
    cout << "Front: " << dq.getFront() << endl;
    cout << "Back: " << dq.getBack() << endl;
    
    cout << "Pop front: " << dq.popFront() << endl;
    cout << "Pop back: " << dq.popBack() << endl;
    
    cout << "Deque after operations: ";
    dq.display();
    
    return 0;
}
