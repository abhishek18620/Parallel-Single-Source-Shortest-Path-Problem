/**
 * @file gabow.cpp
 *
 * @brief Sequential version of Gabow's algorithm for SSSP
 *
 * @author Abhishek Rawat (abhishek18620@gmail.com)
 */

/*
 * sys includes
 * */
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

/*
 * my includes
 * */

/** namespaces **/
using ::std::log2;
using ::std::make_pair;
using ::std::max;
using ::std::pair;
using ::std::priority_queue;
using ::std::string;
using ::std::vector;

/* CONSTANTS */
const int INF = 1e9 + 9;

inline int right_shift(int x, int &i) {
  // x = x >> i;
  return (x = x >> i);
}

struct vertex {
  int id; // Keeps the vertices sequenced
  int dist;
  int extra_dist;
  vector<pair<int, int>> adjList;
  // constructor
  vertex(int id, int dist, int extra_dist)
      : id(id), dist(dist), extra_dist(extra_dist) {}
  // copy constructor
  vertex(const vertex &obj) {
    this->id = obj.get_id();
    this->dist = obj.get_dis();
    this->adjList = obj.adjList;
  }
  // insert an edge u--->v with a weight d
  void insert_edge(int &v, int &d) {
    this->adjList.push_back(make_pair(v, d));
    return;
  }

  // Get the value of dist
  int get_dis() const { return this->dist; }

  // Get the value of id
  int get_id() const { return this->id; }

  // Get the value of extra_dist
  int get_extra_dist() const { return this->extra_dist; }
};

// Comparator
struct compareVertex {
  bool operator()(vertex const &l, vertex const &r) { return l.dist < r.dist; }
};

/* GRAPH */
vector<vertex> graph;

void Gabow_SSSP(int src, int &vertices, int &max_weight) {
  for (int i = 0; i < vertices; ++i) {
    graph[i].dist = INF;
  }
  priority_queue<vertex, vector<vertex>, compareVertex>
      pq;                                // priority queue (priority = min heap)
  int max_iterations = log2(max_weight); // Length of bitset of largest weight
  graph[src].dist = 0;
  pq.push(graph[src]);
  // First iteration with all zero weights
  while (!pq.empty()) {
    vertex u = pq.top();
    int u_id = u.get_id();
    int u_dis = graph[u_id].get_dis();
    pq.pop();
    for (auto pai : graph[u_id].adjList) {
      int v = pai.first;
      int edge_weight = pai.second; // edge_weigth(u, v)
      int v_dis = graph[v].get_dis();
      int wi = right_shift(edge_weight, max_iterations);
      if (v_dis > u_dis + wi) {
        graph[v].dist = u_dis + wi;
        pq.push(graph[v]);
      }
    }
  }
  // Rest
  while (max_iterations) {
    max_iterations--;
    for (auto v : graph) {
      v.extra_dist = INF;
    }
    graph[src].extra_dist = 0;
    pq.push(graph[src]);
    while (!pq.empty()) {
      vertex u = pq.top();
      pq.pop();
      int u_id = u.get_id();
      int u_dis = u.get_dis();
      int u_extra_dist = u.get_extra_dist();
      for (auto pai : graph[u_id].adjList) {
        int v = pai.first;
        int edge_weight = pai.second; // edge_weigth(u, v)
        int v_dis = graph[v].get_dis();
        int v_extra_dist = graph[v].get_extra_dist();
        int Ii = right_shift(edge_weight, max_iterations) +
                 2 * (u_dis - v_dis); // refined weight+ potentiol across it
        if (v_extra_dist > u_extra_dist + Ii) {
          graph[v].extra_dist = u_extra_dist + Ii;
          pq.push(graph[v]);
        }
      }
    }
    for (auto v : graph) {
      if (v.get_dis() < INF) {
        v.dist = 2 * v.get_dis() + v.get_extra_dist();
      }
    }
  }
}

int main() {
  /**
    Code goes here
  */
  int vertices; // vertices
  int edges;    // edges
  int max_weight = -1;
  std::cin >> vertices >> edges;
  graph.resize(vertices + 1,
               vertex(0, 0, 0)); // this'll work even if vertices start from 1
  for (int i = 0; i < vertices; ++i) {
    graph[i].id = i;
  }
  for (int i = 0; i < edges; ++i) {
    int u;
    int v;
    int d;
    max_weight = max(max_weight, d);
    std::cin >> u >> v >> d;
    graph[u].insert_edge(v, d);
  }
  int src = 0;
  Gabow_SSSP(src, vertices, max_weight);
  // Print Distance
  for (auto v : graph) {
    std::cout << "Distace from " << src << " to " << v.get_id() << " = "
              << v.get_dis() << ::std::endl;
  }
  return 0;
}
