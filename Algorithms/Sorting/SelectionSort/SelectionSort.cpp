#include <iostream>
#include <vector>

using namespace std;

struct Node {
	int val;
	Node* next;
};

void selectionSort(Node* head) {
	Node* current = head;
	while (current != nullptr) {
		Node* minNode = current;
		Node* temp = current->next;
		while (temp != nullptr) {
			if (minNode->val < temp->val) {
				minNode = head;
			}
			temp = temp->next;
		}

		swap(current->val, temp->val);
		current = head->next;
	}
}

void selectionSort(vector<int>& arr) {
	int n = arr.size();
	for (int i = 0; i < n - 1; i++) {
		int minNumber = i;
		for (int j = i + 1; j < n; j++) {
			if (arr[j] < arr[minNumber]) {
				minNumber = j;
			}
		}
		int temp = arr[i];
		arr[i] = arr[minNumber];
		arr[minNumber] = temp;
	}
}

void print(vector<int>& arr) {
	for (int i : arr) {
		cout << i << " ";
	}
	cout << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	vector<int> arr = {63,52,128,1,2,9,15,4,1240,24,3};
	cout << "Исходный массив: " << endl;
	print(arr);
	
	selectionSort(arr);

	cout << "Массив после сортировки: " << endl;
	print(arr);
}