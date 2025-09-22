#include <iostream>
#include <vector>

using namespace std;


struct Node {
	int val;
	Node* next;
	Node() : val(0),next(nullptr) {};
	Node(int value) : val(value),next(nullptr){}
};


void addNode(Node*& head, int value) {
	Node* temp = new Node(value);
	temp->next = head;
	head = temp;
}


Node* insertionSortLL(Node* head) {
	if (head == nullptr) return head;

	Node* dummy = new Node(0);
	Node* prev = dummy;

	while (head != nullptr) {
		Node* next = head->next;

		if (prev->val >= head->val) {
			prev = dummy;
		}

		while (prev->next && prev->next->val < head->val) {
			prev = prev->next;
		}
		head->next = prev->next;
		prev->next = head;

		head = next;
	}

	return dummy->next;

}


// С помощью вектора
void insertionSort(vector<int>& arr) {
	int n = arr.size();
	for (int i = 1; i < n; i++) {
		int value = arr[i];
		int hole = i;
		while (hole > 0 && arr[hole - 1] > value) {
			arr[hole] = arr[hole - 1];
			hole = hole - 1;
		}
		arr[hole] = value;
	}
}

void print(vector<int>& arr) {
	for (int number : arr) {
		cout << number << " ";
	}
	cout << endl;
}

void printLL(Node* head) {
	Node* temp = head;
	while (temp != nullptr) {
		cout << temp->val << " ";
		temp = temp->next;
	}
	cout << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	vector<int> arr = { 5235,12,4123,1,2,67,17,195,13 };
	Node* head = nullptr;
	addNode(head, 1);
	addNode(head, 5);
	addNode(head, 3);
	addNode(head, 2);
	addNode(head, 4);
	
	cout << "Исходный массив: " << endl;
	cout << arr.size() << endl;
	print(arr);

	insertionSort(arr);

	cout << "Итоговый массив: " << endl;
	print(arr);


	cout << "Исходный связный список: " << endl;
	printLL(head);

	head = insertionSortLL(head);

	cout << "Итоговый связный список: " << endl;
	printLL(head);
}