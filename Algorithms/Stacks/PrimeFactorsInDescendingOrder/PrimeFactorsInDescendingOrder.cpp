#include <iostream>

using namespace std;

const int MAX = 20;
int stack[MAX];
int first = -1;
int primeArr[8] = { 2,3,5,7,11,13,17,21 };


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

int peek() {
	if (isEmpty()) {
		cout << "Stack is empty" << endl;
		exit(1);
	}
	else {
		return stack[0];
	}
}

void push(int data) {
	if (isFull()) {
		cout << "Stack overflow" << endl;
		return;
	}
	first += 1;
	for (int i = first; i > 0; i--) {
		stack[i] = stack[i - 1];
	}
	stack[0] = data;
}

int pop() {
	if (isEmpty()) {
		cout << "Stack is empty" << endl;
		exit(1);
	}
	int value = stack[0];
	for (int i = 0; i <= first; i++) {
		stack[i] = stack[i + 1];
	}
	first -= 1;

	return value;
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

void primeFact(int num) {
	for (int i = 0; i < 8 - 1; i++) {
		while (num % primeArr[i] == 0) {
			push(primeArr[i]);
			num = num / primeArr[i];
		}
	}
}

int main()
{
	primeFact(450);
	print();
	
}

