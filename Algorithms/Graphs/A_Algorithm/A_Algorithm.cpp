#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node {
	int to;
	int weight;

	Node* next;

	Node(int t, int w) : to(t), weight(w), next(nullptr) {}
};

struct Vertex {
	int x, y;
	Node* edges;
};

class Graph {
private:
	Vertex* vertices;
	int size;
public:
	Graph(int s) : size(s) {
		vertices = new Vertex[size];
		for (int i = 0; i < size; i++) {
			vertices[i].edges = nullptr;
			vertices[i].x = vertices[i].y = 0;
		}
	}

	~Graph() {
		for (int i = 0; i < size; i++) {
			Node* current = vertices[i].edges;
			while (current != nullptr) {
				Node* temp = current;
				current = current->next;
				delete temp;
			}
		}
		delete[] vertices;
	}

	void setCoordinates(int node, int x, int y) {
		vertices[node].x = x;
		vertices[node].y = y;
	}

	void addNode(int a, int b, int weight) {
		Node* newNode = new Node(a, weight);
		newNode->next = vertices[b].edges;
		vertices[b].edges = newNode;

		Node* newNode2 = new Node(b, weight);
		newNode2->next = vertices[a].edges;
		vertices[a].edges = newNode2;
	}

	double heuristic(int from, int to) const {
		int dx = vertices[from].x - vertices[to].x;
		int dy = vertices[from].y - vertices[to].y;
		return sqrt(dx * dx + dy * dy);
	}

	vector<int> aStar(int start, int goal) {
		priority_queue<pair<double, int>,
			vector<pair<double, int>>,
			greater<pair<double, int>>> openSet;

		vector<double> gScore(size, INFINITY);
		vector<double> fScore(size, INFINITY);
		vector<int> cameFrom(size, -1);
		gScore[start] = 0;
		fScore[start] = heuristic(start, goal);
		openSet.push({ fScore[start],start });

		while (!openSet.empty()) {
			int current = openSet.top().second;
			openSet.pop();

			if (current == goal) {
				vector<int> path;
				while (current != -1) {
					path.push_back(current);
					current = cameFrom[current];
				}
				reverse(path.begin(), path.end());
				return path;
			}

			Node* neighbor = vertices[current].edges;
			while (neighbor != nullptr) {
				int sosed = neighbor->to;
				double tentativeGScore = gScore[current] + neighbor->weight;

				if (tentativeGScore < gScore[sosed]) {
					cameFrom[sosed] = current;
					gScore[sosed] = tentativeGScore;
					fScore[sosed] = gScore[sosed] + heuristic(sosed, goal);
					openSet.push({ fScore[sosed], sosed });
				}

				neighbor = neighbor->next;
			}

		}
		return {};
	}
};

int main() {
	setlocale(LC_ALL, "Russian");
	Graph g(5);

	g.setCoordinates(0, 0, 0);
	g.setCoordinates(1, 2, 3);
	g.setCoordinates(2, 4, 1);
	g.setCoordinates(3, 5, 4);
	g.setCoordinates(4, 1, 2);

	g.addNode(0, 1, 4);
	g.addNode(0, 2, 2);
	g.addNode(1, 3, 5);
	g.addNode(2, 3, 1);
	g.addNode(3, 4, 3);

	vector<int> path = g.aStar(0, 4);

	cout << "Путь: ";
	for (int node : path) {
		cout << node << " ";
	}
	cout << endl;

	return 0;
}