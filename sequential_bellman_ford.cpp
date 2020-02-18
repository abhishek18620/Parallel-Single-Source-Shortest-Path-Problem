// g++ -pthread -std=c++14 -o gabow.out gabow.cpp -D LOCAL_DEFINE
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>
#include <thread>

/** namespaces **/
using ::std::make_pair;
using ::std::pair;
using ::std::thread;
using ::std::queue;
using ::std::string;
using ::std::vector;
using ::std::ref;
using ::std::chrono::steady_clock;
using ::std::chrono::milliseconds;
using ::std::chrono::duration_cast;

#define trace2(x, y)                                                           \
  std::cerr << #x << ": " << x << " | " << #y << ": " << y << std::endl;
#define trace3(x, y, z)                                                        \
  std::cerr << #x << ": " << x << " | " << #y << ": " << y << " | " << #z      \
            << ": " << z << std::endl;
#define trace4(a, b, c, d)                                                     \
  std::cerr << #a << ": " << a << " | " << #b << ": " << b << " | " << #c      \
            << ": " << c << " | " << #d << ": " << d << std::endl;
#define trace5(a, b, c, d, e)                                                  \
  std::cerr << #a << ": " << a << " | " << #b << ": " << b << " | " << #c      \
            << ": " << c << " | " << #d << ": " << d << " | " << #e << ": "    \
            << e << std::endl;

const int INF = 1e9 + 9;

void set_stack_limit() {
  const rlim_t kStackSize = 1000 * 1024 * 1024; // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0) {
    if (rl.rlim_cur < kStackSize) {
      rl.rlim_cur = kStackSize;
      result = setrlimit(RLIMIT_STACK, &rl);
      if (result != 0) {
        fprintf(stderr, "setrlimit returned result = %d\n", result);
      }
    }
  }
}

struct vertex {
  int id; // Keeps the vertices sequenced
  int dist;
  vector<pair<int, int>> adjList;
  // constructor
  vertex(int x, int y) : id(x), dist(y) {}
  // insert an edge u--->v with a weight d
  void insert_edge(int &v, int &d) {
    this->adjList.push_back(make_pair(v, d));
    return;
  }

  // Prints vertex
  void print() { trace2(id, dist); }

  // Get the value of dist
  int get_dis() const { return this->dist; }

  // Get the value of id
  int get_id() const { return this->id; }
};

vector<vertex>* construct_graph(int &edges, int &vertices) {
  vector<vertex> *graph = new vector<vertex>;
  graph->resize(vertices,
                vertex(0, 0)); // this'll work even if vertices start from 1
  for (int i = 0; i < vertices; ++i) {
    graph->at(i).id = i;
  }
  for (int i = 0; i < edges; ++i) {
    int u;
    int v;
    int d;
    std::cin >> u >> v >> d;
    if(u >= vertices or v >=vertices) {
      continue;
    }
    //trace2(u, v);
    graph->at(u).insert_edge(v, d);
  }
  return graph;
}

void bellman_ford(int &src, int &vertices, vector<vertex>* graph_poi) {
  vector<vertex> graph = *graph_poi;
  bool negative_cycle = false;
  for (auto &v : graph) {
    v.dist = INF;
  }
  // source vertex
  graph[src].dist = 0;

  int current_queue = 0;
  vector<bool> present_in_queue(vertices, false);
  vector<int> queue_counter(vertices,
                            0); // queue doesn't give the index of element
  std::queue<vertex> queue;
  queue.push(graph[src]);
  using namespace std::chrono_literals;
  present_in_queue[src] = true;
  std::this_thread::sleep_for(7s);
  queue_counter[0] = 1;

  while (!queue.empty() && !negative_cycle) {
    vertex u = queue.front();
    int u_id = u.get_id();
    queue.pop();
    present_in_queue[u_id] = false;
    for (int i = 0; i < u.adjList.size(); ++i) {
      auto pai = u.adjList[i];
      int v = pai.first;
      int weight = pai.second;
      // already calculated once
      if (weight < INF) {
        int possible_dist = weight + u.get_dis();
        if (possible_dist < graph[v].get_dis()) {
          graph[v].dist = possible_dist;
          queue_counter[v]++;
          if (!present_in_queue[v]) {
            // if v is not present in queue
            present_in_queue[v] = true;
            if (queue_counter[v] >= vertices) {
              negative_cycle = true;
            }
            { queue.push(graph[v]); }
          }
        }
      }
    }
  }

  // Print Distance
  for (auto &v : graph) {
    std::cout << "Distace from " << src << " to " << v.get_id() << " = "
              << v.dist << ::std::endl;
  }
}

int main(int argc, char **argv) {
  /**
    Code goes here
  */
  const rlim_t kStackSize = 2000 * 1024 * 1024; // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0) {
    if (rl.rlim_cur < kStackSize) {
      rl.rlim_cur = kStackSize;
      result = setrlimit(RLIMIT_STACK, &rl);
      if (result != 0) {
        fprintf(stderr, "setrlimit returned result = %d\n", result);
      }
    }
  }

  if (argc <= 1) {
    ::std::cerr << "No Input file specified" << ::std::endl;
    abort();
  }
  if (argc <= 2) {
    ::std::cerr << "No Output file specified" << ::std::endl;
    abort();
  }

  string input_filename = argv[1];
  string output_filename = argv[2];
  freopen(input_filename.c_str(), "rt", stdin);
  freopen(output_filename.c_str(), "w", stdout);

  int vertices; // vertices
  int edges;    // edges
  std::cin >> vertices >> edges;
  trace2(vertices, edges);
  vector<vertex>* graph = construct_graph(edges, vertices);
  auto start = steady_clock::now();
  int src = 0;
  thread t(bellman_ford, ref(src), ref(vertices), graph);
  t.join();
  auto end = steady_clock::now();
  ::std::cerr << "Time elapsed: " << duration_cast<milliseconds>(end - start).count() << "ms\n";

  return 0;
}
