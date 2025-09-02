#include <iostream>

using namespace std;

int binarySearch(int arr[], int start, int end, int target) {
	while (start <= end) {
		int middle = (end + start) / 2;
		if (arr[middle] == target) {
			return middle;
		}
		else if (arr[middle] < target) {
			start = middle + 1;
		}
		else {
			end = middle - 1;
		}
	}
	return -1;
}

int main() {
	setlocale(LC_ALL, "Russian");
	int number = 7;
	int arr[7] = { 1,2,3,4,5,6,7 };

	cout << "Позиция числа " << number << " - " << binarySearch(arr, 0, 7, 5);
}