#include <iostream>
#define MAX 100

using namespace std;

struct Queue {
private:
	int queue[MAX];

	int front, rear;
public:
	Queue() {
		front = -1;
		rear = -1;
	}

	bool isEmpty() {
		if (front == -1 && rear == -1) {
			return 1;
		}
		return 0;
	}

	bool isFull() {
		if (rear == MAX - 1) {
			return 1;
		}
		return 0;
	}

	void enqueue(int value) {
		if (isFull()) {
			cout << "Queue is full" << endl;
			exit(1);
		}
		if (isEmpty()) {
			front = rear = 0;
		}
		else {
			rear++;
		}
		queue[rear] = value;
	}

	void dequeue() {
		if (isEmpty()) {
			cout << "Queue is empty" << endl;
			exit(1);
		}
		if (front == rear) {
			front = rear = -1;
		}
		front++;
	}

	int getFront() {
		if (isEmpty()) {
			std::cout << "Queue is empty!\n";
			return -1;
		}
		return queue[front];
	}
};

int main() {
	Queue q;
	q.enqueue(10);
	q.enqueue(20);
	q.enqueue(30);
	std::cout << q.getFront() << "\n";
	q.dequeue();
	std::cout << q.getFront() << "\n";
	return 0;
}
