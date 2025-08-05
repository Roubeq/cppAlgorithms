#include <iostream>
#include <vector>

using namespace std;

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

int main() {
	setlocale(LC_ALL, "Russian");
	vector<int> arr = { 5235,12,4123,1,2,67,17,195,13 };
	
	cout << "Исходный массив: " << endl;
	cout << arr.size() << endl;
	print(arr);

	insertionSort(arr);

	cout << "Итоговый массив: " << endl;
	print(arr);
}