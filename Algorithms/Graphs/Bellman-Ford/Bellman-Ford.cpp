#include <iostream>

using namespace std;


struct Node {
	int value;
	int weight;
	Node* next;

	Node(int val, int w) : value(val), weight(w), next(nullptr) {}
};


class Graph {
private:
	Node** graph;
	int size;
public:
	Graph(int s) : size(s) {
		graph = new Node * [size];
		for (int i = 0; i < size; i++) {
			graph[i] = nullptr;
		}
	}

	~Graph() {
		for (int i = 0; i < size; i++) {
			Node* current = graph[i];
			while (current != nullptr) {
				Node* temp = current;
				temp = nullptr;
				current = current->next;
				delete temp;
			}
		}
		delete[] graph;
	}

	void addNode(int nodeA, int nodeB, int weight) {
		Node* newNode = new Node(nodeB, weight);
		newNode->next = graph[nodeA];
		graph[nodeA] = newNode;

		
	}

	void bellmanFord(int start) {
		int* distance = new int[size];
		for (int i = 0; i < size; i++) {
			distance[i] = INT_MAX;
		}
		distance[start] = 0;

		int* predecessor = new int[size];
		for (int i = 0; i < size; i++) {
			predecessor[i] = -1;
		}

		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < size; j++) {
				Node* current = graph[j];
				while (current != nullptr) {
					int v = current->value;
					int w = current->weight;
					
					if (distance[j] != INT_MAX && distance[j] + w < distance[v]) {
						distance[v] = distance[j] + w;
						predecessor[v] = j;
					}
					current = current->next;
				}
			}
		}

		bool hasNegative = false;
		for (int i = 0; i < size; i++) {
			Node* current = graph[i];
			while (current != nullptr) {
				int v = current->value;
				int w = current->weight;

				if (distance[i] != INT_MAX && distance[v] > distance[i] + w) {
					hasNegative = true;
					break;
				}

				current = current->next;
			}
			if (hasNegative) break;
		}

		if (hasNegative) {
			cout << "Граф содержит отрицательный цикл!" << endl;
		}
		else {
			cout << "Кратчайшие расстояния от вершины " << start << ":" << endl;
			for (int i = 0; i < size; i++) {
				if (distance[i] != INT_MAX) {
					cout << "До " << i << ": " << distance[i] << endl;
				}
				else {
					cout << "До " << i << ": недостижима" << endl;
				}
			}
		}
	}
};

int main() {
	setlocale(LC_ALL, "Russian");
	Graph graph(5);

	graph.addNode(0, 1, -1);
	graph.addNode(0, 2, 4);
	graph.addNode(1, 2, 3);
	graph.addNode(1, 3, 2);
	graph.addNode(1, 4, 2);
	graph.addNode(3, 2, 5);
	graph.addNode(3, 1, 1);
	graph.addNode(4, 3, -3);

	graph.bellmanFord(0);

	return 0;
}