#include <iostream>

using namespace std;

struct Node {
	int data;
	Node* next;
};


struct Queue {
private:
	Node* front, * rear;
public:
	Queue() : front(nullptr), rear(nullptr) {}

	bool isEmpty() {
		if (front == nullptr && rear == nullptr) {
			return 1;
		}
		return 0;
	}
	
	void enqueue(int value) {
		Node* temp = new Node;
		temp->data = value;
		if (isEmpty()) {
			front = rear = temp;
			front->data = rear->data = value;
		}
		else {
			rear->next = temp;
			rear = temp;
		}
 	}

	void dequeue() {
		if (isEmpty()) {
			cout << "Queue is empty" << endl;
			exit(1);
		}
		else {
			Node* temp = nullptr;
			temp = front;
			front = front->next;
			if (front == nullptr) rear = nullptr;
		}
	}

	int getFront() {
		if (isEmpty()) {
			cout << "Queue is empty!\n";
			return -1;
		}
		return front->data;
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
