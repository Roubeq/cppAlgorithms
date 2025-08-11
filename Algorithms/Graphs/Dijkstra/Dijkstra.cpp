#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

using namespace std;

struct Node {
	int number;
	int weight;

	Node* next;

	Node(int num, int w) : number(num), weight(w), next(nullptr) {}
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
			if (current != nullptr) {
				Node* temp = current;
				current = current->next;
				temp = nullptr;
				delete temp;
			}
		}
		delete[] graph;
	}

	void addNode(int u, int v, int weight) {
		Node* newNode = new Node(v, weight);
		newNode->next = graph[u];
		graph[u] = newNode;

		Node* reverseNode = new Node(u, weight);
		reverseNode->next = graph[v];
		graph[v] = reverseNode;
	}

	/*
	1. Создаем массив расстояний(он будет содержать минимальные расстояния до точки)
	2. Добавляем стартовую точку в массив с весом 0, так как мы в ней уже находимся
	3. Создаем приоритетную очередь(минимальную кучу) пара чисел; в чем хранится; греатер для мин кучи(по дефолту макс куча)
	4. Добавляем в очередь первую вершину, это наша стартовая вершина
	5. И пока очередь не пустая, будем из нее вытаскивать вершины
	6. Берем текущий элемент очереди и извлекаем его из неё. 
	7. Оптимизируем алгоритм с помощью проверки(чтобы вершины, у которых вес больше чем уже есть, пропускались)
	8. Перебираем всех соседей и сравниваем вес с текущим, если он меньше - добавляем в очередь и меняем в массиве расстояние
	9. Выводим конечный массив
	*/

	void dijkstra(int start) {
		vector<int> distance(size, INT_MAX);
		distance[start] = 0;

		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> queue;
		queue.push({ 0,start });

		while (!queue.empty()) {
			int current = queue.top().second;
			int currentDistance = queue.top().first;
			queue.pop();

			if (currentDistance > distance[current]) {
				continue;
			}

			Node* neighbor = graph[current];
			while (neighbor != nullptr) {
				int nextNode = neighbor->number;
				int newDist = distance[current] + neighbor->weight;

				if (newDist < distance[nextNode]) {
					queue.push({ newDist, nextNode });
					distance[nextNode] = newDist;
				}
				neighbor = neighbor->next;
			}
		}

		cout << "Расстояние от " << start << endl;
		for (int i = 0; i < size; i++) {
			if (i != INT_MAX) {
				cout << "До " << i << ": " << distance[i] << endl;
			}
			else {
				cout << "Точка" << i << "недостижима" << endl;
			}
		}

	}
};

int main() {
	setlocale(LC_ALL, "Russian");
	Graph g(5);

	g.addNode(0, 1, 10);
	g.addNode(0, 2, 3);
	g.addNode(1, 2, 1);
	g.addNode(1, 3, 2);
	g.addNode(2, 3, 8);
	g.addNode(3, 4, 7);
	g.addNode(2, 4, 5);

	g.dijkstra(3);

	return 0;
}