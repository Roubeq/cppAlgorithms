#include <iostream>
#define MAX_SIZE 100

using namespace std;

class CircularQueue {
private:
    int arr[MAX_SIZE];
    int front, rear;

public:
    CircularQueue() {
        front = -1;
        rear = -1;
    }

    bool isEmpty() {
        return (front == -1);
    }

    bool isFull() {
        return ((rear + 1) % MAX_SIZE == front);
    }

    void enqueue(int x) {
        if (isFull()) {
            std::cout << "Queue is full!\n";
            return;
        }
        if (isEmpty()) {
            front = rear = 0;
        }
        else {
            rear = (rear + 1) % MAX_SIZE;
        }
        arr[rear] = x;
    }

    void dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty!\n";
            return;
        }
        if (front == rear) {
            front = rear = -1;
        }
        else {
            front = (front + 1) % MAX_SIZE;
        }
    }
   
    int getFront() {
        if (isEmpty()) {
            cout << "Queue is empty" << endl;
            exit(1);
        }
        return front;
    }
};

int main() {
    CircularQueue q;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    std::cout << q.getFront() << "\n";
    q.dequeue();
    std::cout << q.getFront() << "\n";
    return 0;
}