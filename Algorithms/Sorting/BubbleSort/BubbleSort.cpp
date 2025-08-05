#include <iostream>
#include <vector>

using namespace std;

void bubbleSort(vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < arr.size() - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
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
    vector<int> numbers = { 64, 34, 25, 12, 22, 11, 90 };

    cout << "Исходный массив: " << endl;
    print(numbers);

    bubbleSort(numbers);

    cout << "Массив после сортировки: " << endl;
    print(numbers);

    return 0;
}