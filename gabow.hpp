/**
 * @file gabow.hpp
 *
 * @brief Header for Encryption module
 *
 * @author Abhishek Rawat (abhishek18620@gmail.com)
 */

#ifndef PARALLEL_SSSP_GABOW_HPP_
#define PARALLEL_SSSP_GABOW_HPP_

/*
 * sys includes
 * */
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <queue>

/*
 * our includes
 * */

/**
 * @brief Grapg DS : graph[u] is vectored list of
 * pairs containing v and distace
 * ~ adjMatrix[u][v] = distance
 *
 */

struct Graph {
  ::std::vector<::std::vector<::std::pair<int, int> > > adjList;

  Graph(int n) {
    adjList.resize(n+1);
  }
};

class Gabow {
 public:
   void ExposeBit(int &x, int k) { x = ((1 << k) | x); }

   void SSSP();

 private:
   ::std::priority_queue<int> pq;
};

