#include <iostream>

using namespace std;

struct Node {
	int data;
	Node* next;
};

struct Deque {
private:
	int data;
	Node* front, * rear;
public:
	
	Deque() : front(nullptr), rear(nullptr) {}

	bool isEmpty() {
		if (front == nullptr && rear == nullptr) return 1;
	}
	void pushFront(int value) {
		Node* temp = new Node;
		temp->data = value;
		if (isEmpty()) {
			front = rear = temp;
		} 
		else {
			temp->next = front;
			front = temp;
		}
	}
	void pushFront(int value) {
		Node* temp = new Node;
		temp->data = value;
		if (isEmpty()) {
			front = rear = temp;
		}
		else {
			rear->next = temp;
			rear = temp;
		}
	}
}


int main() {

}
