#include <iostream>
#include <vector> 

using namespace std;

class MaxHeap {
private:
	vector<int> heap;

	int parent(int i) { return (i - 1) / 2; }
	int left(int i) { return 2 * i + 1; }
	int right(int i) { return 2 * i + 2; }

	void siftDown(int index) {
		int maxIndex = index;
		int size = heap.size();

		int parentNode = parent(index);
		int leftNode = left(index);
		int rightNode = right(index);

		if (leftNode < size && heap[leftNode] > heap[maxIndex]) {
			maxIndex = leftNode;
		}

		if (rightNode < size && heap[rightNode] > heap[maxIndex]) {
			maxIndex = rightNode;
		}

		if (index != maxIndex) {
			swap(heap[index], heap[maxIndex]);
			siftDown(maxIndex);
		}

	}

	int top() {
		return heap[0];
	}

	int back() {
		return heap.back();
	}

	void siftUp(int index) {
		while (index > 0 && heap[parent(index)] < heap[index]) {
			swap(heap[parent(index)], heap[index]);
			index = parent(index);
		}
	}
public:
	void insert(int value) {
		heap.push_back(value);
		siftUp(heap.size() - 1);
	}

	int pop() {
		int size = heap.size();
		if (size == 0) {
			cout << "Куча пустая" << endl;
			return -1;
		}

		int value = heap[0];
		heap[0] = heap.back();
		heap.pop_back();

		siftDown(0);
		return value;
	}

	bool empty() {
		if (heap.size() == 0) {
			return 1;
		}
		else return 0;
	}

	void heapSort(vector<int>& arr) {
		MaxHeap heap;
		for (int num : arr) {
			heap.insert(num);
		}
		for (int i = arr.size() - 1; i >= 0; --i) {
			arr[i] = heap.pop();
		}
	}

};

void print(vector<int>& arr) {
	for (int number : arr) {
		cout << number << " ";
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	vector<int> numbers = { 1,2,150,2145,62,691,3,6,7,1456,123 };
	MaxHeap heap;
	cout << "Создание макс бин кучи..." << endl;

	print(numbers);
	heap.heapSort(numbers);
	cout << endl;


	cout << "Массив после сортировки кучей: " << endl;
	print(numbers);
	return 0;
}