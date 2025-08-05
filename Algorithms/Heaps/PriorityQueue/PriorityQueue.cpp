#include <iostream>
#include <queue>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	priority_queue<int> queue;
	queue.push(15);
	queue.push(2);
	queue.push(128);
	queue.push(156);
	queue.push(15124);
	queue.push(15);
	queue.push(13);
	queue.push(1);

	cout << "Top element - " << queue.top() << endl;

	cout << "All elements - ";
	while (!queue.empty()) {
		cout << queue.top() << " ";
		queue.pop();
	}

	return 0;
}