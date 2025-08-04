#include <iostream>
#include <vector>

using namespace std;

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