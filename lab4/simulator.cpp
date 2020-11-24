#include <chrono>
#include <iostream>
#include <limits.h>
#include <list>
#include <stack>
#include <vector>

struct Node {
  int direction;
  int weight;
  Node(int direction, int weight) : direction(direction), weight(weight) {}
};

class Graph {
  int _size;
  std::vector<std::list<Node>> _routes;
  void _visit(int n, std::vector<bool> &visited, std::stack<int> &stack) {
    visited[n] = true;
    for (const auto &node : _routes[n])
      if (!visited[node.direction])
        _visit(node.direction, visited, stack);
    stack.push(n);
  };

public:
  Graph(const std::vector<std::vector<int>> &matrix) {
    _size = matrix.size();
    _routes.resize(_size);
    for (int n = 0; n < _size; ++n)
      for (int m = 0; m < _size; ++m)
        if (matrix[n][m])
          _routes[m].push_back(Node(n, matrix[n][m]));
  };
  std::vector<int> criticalPath() {
    int resLength = INT_MIN;
    std::vector<int> resPath;
    for (int s = 0; s < _size; ++s) {
      std::vector<int> distance;
      std::vector<std::vector<int>> paths;
      distance.resize(_size, INT_MIN);
      paths.resize(_size);
      distance[s] = 0;
      std::stack<int> stack;
      std::vector<bool> visited;
      visited.resize(_size, false);
      for (int i = 0; i < _size; i++)
        if (!visited[i])
          _visit(i, visited, stack);
      while (!stack.empty()) {
        int u = stack.top();
        stack.pop();
        if (distance[u] > INT_MIN) {
          if (distance[u] > resLength) {
            resLength = distance[u];
            resPath = paths[u];
          }
          for (const auto &node : _routes[u])
            if (distance[node.direction] < distance[u] + node.weight) {
              distance[node.direction] = distance[u] + node.weight;
              paths[node.direction].push_back(u);
            }
        }
      }
    }
    return resPath;
  };
};
Graph generateDAG(unsigned size, unsigned edges, unsigned max,
                  unsigned min = 1) {
  std::vector<std::vector<int>> matrix;
  matrix.resize(size);
  for (auto &line : matrix)
    line.resize(size);
  for (unsigned i = 0; i < edges; ++i) {
    int n = std::rand() % size;
    if (n) {
      int m = std::rand() % n;
      if (m) {
        if (!matrix[n][m])
          matrix[n][m] = std::rand() % (max - min + 1) + 1;
        else
          --i;
      } else
        --i;
    } else
      --i;
  }
  return Graph(matrix);
}
unsigned long long benchmark(Graph &&graph) {
  auto start = std::chrono::high_resolution_clock::now();
  graph.criticalPath();
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::high_resolution_clock::now() - start)
      .count();
}
int main() {
  while (true) {
    int size, edges, min, max;
    std::cin >> size >> edges >> min >> max;
    std::cout << benchmark(generateDAG(size, edges, max, min));
  }
  return 0;
}
