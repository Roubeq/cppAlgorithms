#include <iostream>
#include <vector>

using namespace std;

class MinHeap {
	vector<int> heap;

	int parent(int i) {
		return (i - 1) / 2;
	};
	int left(int i) {
		return 2 * i + 1;
	};
	int right(int i) {
		return 2 * i + 2;
	};

	void siftUp(int i) {
		while (i > 0 && heap[parent(i)] > heap[i]) {
			swap(heap[parent(i)], heap[i]);
			i = parent(i);
		}
	}

	void siftDown(int i) {
		int minIndex = i;
		int rightNode = right(i);
		int leftNode = left(i);
		int size = heap.size();

		if (leftNode < size && heap[leftNode] < heap[minIndex]) {
			minIndex = leftNode;
		}

		if (rightNode < size && heap[rightNode] < heap[minIndex]) {
			minIndex = rightNode;
		}

		if (i != minIndex) {
			std::swap(heap[i], heap[minIndex]);
			siftDown(minIndex);
		}
	}
public:
	void removeNode(int i) {
		if (heap.size() == 0) {
			cout << "Heap is empty!" << endl;
		}
		heap[i] = getMin() - 1;
		siftUp(i);
		deleteNode();
	}
	void insert(int value) {
		heap.push_back(value);
		siftUp(heap.size() - 1);
	}

	int deleteNode() {
		if (heap.empty()) {
			throw std::out_of_range("Heap is empty");
		}
		int value = heap[0];
		heap[0] = heap.back();
		heap.pop_back();

		siftDown(0);
		return value;
	}
	
	void print() {
		for (int i = 0; i < heap.size(); i++) {
			cout << heap[i] << " ";
		}
		cout << endl;
	}

	int getMin() const {
		if (heap.empty()) {
			throw std::out_of_range("Heap is empty");
		}
		return heap[0];
	}

	int find(int value) const {
		for (int i = 0; i < heap.size(); ++i) {
			if (heap[i] == value) {
				return i;
			}
		}
		return -1;
	}

	
};

int main() {
	MinHeap heap;

	heap.insert(5);
	heap.insert(3);
	heap.insert(8);
	heap.insert(1);
	heap.insert(4);

	std::cout << "Min heap: ";
	heap.print();

	std::cout << "Min element: " << heap.getMin() << std::endl;

	std::cout << "Extracted min: " << heap.deleteNode() << std::endl;
	std::cout << "Heap after extraction: ";
	heap.print();


	heap.removeNode(3);
	std::cout << "Heap after removal: ";
	heap.print();

	return 0;
}
