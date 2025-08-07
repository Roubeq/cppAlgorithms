#include <iostream>
#include <vector>

using namespace std;

int partition(vector<int>& arr, int start, int end) {
	int pivot = arr[end];
	int pIndex = start;
	for (int i = start; i < end; i++) {
		if (arr[i] <= pivot) {
			swap(arr[i], arr[pIndex]);
			pIndex++;
		}
	}
	swap(arr[end], arr[pIndex]);
	return pIndex;
}

int randomPartition(vector<int>& arr, int start, int end) {
	int pivotIndex = start + rand() % (end - start + 1);
	swap(arr[pivotIndex], arr[end]);
	int pIndex = partition(arr, start, end);
	return pIndex;
}

void quickSort(vector<int>& arr, int start, int end) {
	if (start < end) {
		int pIndex = randomPartition(arr, start, end);
		quickSort(arr, start, pIndex - 1);
		quickSort(arr, pIndex + 1, end);
	}
}

void print(vector<int>& numbers) {
	for (int number : numbers) {
		cout << number << " ";
	}
	cout << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");

	vector<int> numbers = { 5135,13,561,6234,46,23,8346,12,1,2,888,5 };

	cout << "Массив до сортировки: " << endl;
	print(numbers);

	quickSort(numbers, 0, numbers.size() - 1);
	cout << "Массив после сортировки: " << endl;
	print(numbers);

	return 0;
}