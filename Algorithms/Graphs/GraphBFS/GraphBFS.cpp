#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <stack>

using namespace std;

class Graph {
    int value;
    vector<vector<int>> adj;

public:
    Graph(int v) : value(v), adj(value) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void BFS(int start) {
        queue<int> q;
        unordered_set<int> visited;

        q.push(start);
        visited.insert(start);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            cout << current << " ";
            for (int neighbor : adj[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
    }

    void DFS(int start) {
        stack<int> stack;
        unordered_set<int> visited;

        stack.push(start);
        visited.insert(start);

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            cout << current << " ";
            for (auto it = adj[current].rbegin(); it != adj[current].rend(); it++) {
                int neighbor = *it;
                if (visited.find(neighbor) == visited.end())
                    stack.push(neighbor);
                    visited.insert(neighbor);
            }
        }

    }

};

int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);

    cout << "BFS: ";
    g.BFS(0);
    cout << "\n" << "DFS: ";
    g.DFS(0);

    return 0;
}