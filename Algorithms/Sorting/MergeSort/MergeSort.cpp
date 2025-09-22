#include <iostream>
#include <vector>

using namespace std;

struct Node {
	int val;
	Node* next;
};

Node* findMiddle(Node* head) {
	if (head == nullptr) return;

	Node* slow = head;
	Node* fast = head->next;

	while (fast->next != nullptr && fast != nullptr) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}


Node* merge(Node* left, Node* right) {
	Node* dummy = new Node();
	Node* temp = dummy;
	
	while (left != nullptr && right != nullptr) {
		if (left->val <= right->val) {
			temp->next = left;
			left = left->next;
		}
		else {
			temp->next = right;
			right = right->next;
		}
	}

	if (left != nullptr) {
		temp->next = left;
	}
	else {
		temp->next = right;
	}

	return dummy->next;

}

Node* mergeLL(Node* head) {
	Node* middle = findMiddle(head);
	Node* left = head;
	Node* right = middle->next;
	middle->next = nullptr;

	left = mergeLL(left);
	right = mergeLL(right);

	merge(left, right);
}

void mergeArrays(vector<int>& left, vector<int>& right, vector<int>& result) {
	int sizeLeft = left.size();
	int sizeRight = right.size();
	int i = 0;
	int j = 0;
	int k = 0;
	while (i < sizeLeft && j < sizeRight) {
		if (left[i] <= right[j]) {
			result[k] = left[i];
			i++;
		}
		else if (left[i] > right[j]) {
			result[k] = right[j];
			j++;
		}
		k++;
	}
	while (i < sizeLeft) {
		result[k] = left[i];
		i++;
		k++;
	}
	while (j < sizeRight) {
		result[k] = right[j];
		j++;
		k++;
	}
}

void mergeSort(vector<int>& arr) {
	int size = arr.size();
	if (size < 2) {
		return;
	}
	int mid = size / 2;

	vector<int> left(arr.begin(), arr.begin() + mid);
	vector<int> right(arr.begin() + mid, arr.end());

	mergeSort(left);
	mergeSort(right);
	mergeArrays(left, right, arr);

}

void print(vector<int>& numbers) {
	for (int number : numbers) {
		cout << number << " ";
	}
	cout << endl;

}
int main() {
	setlocale(LC_ALL, "Russian");

	vector<int> numbers = { 624, 135, 7235, 1, 2562,35, 5,7,32546,23,13 };

	cout << "Массив до сортировки : " << endl;
	print(numbers);

	mergeSort(numbers);
	cout << "Массив после сортировки: " << endl;
	print(numbers);

	return 0;
}
