// g++ -pthread -std=c++14 -o gabow.out gabow.cpp -D LOCAL_DEFINE
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
#include <chrono>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <thread>
/*
 * my includes
 * */

/** namespaces **/
using ::std::make_pair;
using ::std::pair;
using ::std::thread;
using ::std::max;
using ::std::priority_queue;
using ::std::queue;
using ::std::string;
using ::std::vector;
using ::std::ref;
using ::std::chrono::steady_clock;
using ::std::chrono::milliseconds;
using ::std::chrono::duration_cast;

#define trace2(x, y)                                                           \
  std::cerr << #x << ": " << x << " | " << #y << ": " << y << ::std::endl;
#define trace3(x, y, z)                                                        \
  std::cerr << #x << ": " << x << " | " << #y << ": " << y << " | " << #z      \
            << ": " << z << ::std::endl;
#define trace4(a, b, c, d)                                                     \
  std::cerr << #a << ": " << a << " | " << #b << ": " << b << " | " << #c      \
            << ": " << c << " | " << #d << ": " << d << ::std::endl;
#define trace5(a, b, c, d, e)                                                  \
  std::cerr << #a << ": " << a << " | " << #b << ": " << b << " | " << #c      \
            << ": " << c << " | " << #d << ": " << d << " | " << #e << ": "    \
            << e << ::std::endl;
#

/* CONSTANTS */
const int INF = 1e9 + 9;

inline int right_shift(int x, int &i) {
  x = x >> i;
  return x;
}

struct vertex {
  int id; // Keeps the vertices sequenced
  int dist;
  int extra_dist;
  vector<pair<int, int>> adjList;
  // constructor
  vertex(int x, int y , int z)
      : id(x), dist(y), extra_dist(z) {}
  // insert an edge u--->v with a weight d
  void insert_edge(int &v, int &d) {
    this->adjList.push_back(make_pair(v, d));
    return;
  }

  // Prints vertex
  void print() {
    trace3(id, dist, extra_dist);
  }

  // Get the value of dist
  int get_dis() const { return this->dist; }

  // Get the value of id
  int get_id() const { return this->id; }

  // Get the value of extra_dist
  int get_extra_dist() const { return this->extra_dist; }
};

// Comparators
struct compareVertexDist {
  bool operator()(vertex const &l, vertex const &r) { return l.dist > r.dist; }
};

struct compareVertexExtraDist {
  bool operator()(vertex const &l, vertex const &r) { return l.extra_dist > r.extra_dist; }
};


/* GRAPH */
vector<vertex>* construct_graph(int &edges, int &vertices, int &max_weight) {
  vector<vertex> *graph = new vector<vertex>;
  graph->resize(vertices,
                vertex(0, 0, 0)); // this'll work even if vertices start from 1
  for (int i = 0; i < vertices; ++i) {
    graph->at(i).id = i;
  }
  for (int i = 0; i < edges; ++i) {
    int u;
    int v;
    int d;
    std::cin >> u >> v >> d;
    max_weight = max(max_weight, d);
    if(u >= vertices or v >=vertices) {
      continue;
    }
    //trace2(u, v);
    graph->at(u).insert_edge(v, d);
  }
  return graph;
}

void print_graph(vector<vertex> *graph) {
  for (auto &v : *(graph)) {
    v.print();
  }
}

void Gabow_SSSP(int &src, int &vertices, int &max_weight, vector<vertex>* graph_poi) {
  vector<vertex> graph = *graph_poi;
  for (int i = 0; i < vertices; ++i) {
    graph[i].dist = INF;
  }
  priority_queue<vertex, vector<vertex>, compareVertexDist>
      pq;                                // priority queue (priority = min heap)
  /*// Debug priority queue behaviour*/
  //for (int i = 0; i < 5; ++i) {
    //vertex temp(i, i*i, 0);
    //pq.push(temp);
  //}
  //pq.push(vertex(6, -1, 0));
  //pq.push(vertex(7, 2, 0));
  //pq.push(vertex(8, 5, 0));
  //for (int i = 0; i < 8; ++i) {
  //std::cout << pq.top().get_dis() << " id : "<< pq.top().get_id() << std::endl;
    //pq.pop();
  /*}*/
  //int max_iterations = log2(max_weight); // Length of bitset of largest weight
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(6s);
  graph[src].dist = 0;
  pq.push(graph[src]);
  //print_graph();
  // First iteration with all zero weights
  while (!pq.empty()) {
    vertex u = pq.top();
    int u_id = u.get_id();
    int u_dis = graph[u_id].get_dis();
    // trace2(u_id, u_dis);
    pq.pop();
    for (auto &pai : graph[u_id].adjList) {
      int v = pai.first;
      int edge_weight = pai.second; // edge_weigth(u, v)
      int v_dis = graph[v].get_dis();
      //trace3(v, edge_weight, wi);
      if (v_dis > u_dis + edge_weight) {
        graph[v].dist = u_dis + edge_weight;
        pq.push(graph[v]);
      }
    }
  }
  // Print Distance
  for (auto &v : graph) {
    std::cout << "Distace from " << src << " to " << v.get_id() << " = "
              << v.dist << ::std::endl;
  }

}

int main() {
/**
  Code goes here
*/
#ifdef LOCAL_DEFINE
  freopen("WikiTalk_gabow.txt", "rt", stdin);
  freopen("dijkstra_output.txt","w",stdout);
#endif

  int vertices; // vertices
  int edges;    // edges
  int max_weight = -1;
  std::cin >> vertices >> edges;
  // constructs graph
  vector<vertex>* graph = construct_graph(edges, vertices, max_weight);
  trace3(vertices, edges, max_weight);
  auto start = steady_clock::now();
  int src = 0;
  thread t(Gabow_SSSP, ref(src), ref(vertices), ref(max_weight), graph);
  t.join();
  auto end = steady_clock::now();
#ifdef LOCAL_DEFINE
  ::std::cerr << "Time elapsed: " << duration_cast<milliseconds>(end - start).count() << "ms\n";
  // cin>>t;
#endif

  return 0;
}
