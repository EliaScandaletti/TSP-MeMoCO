#include "solver/farthest.h"

#include <list>
#include <vector>

Path TSP::solver::FarthestInsertion::solve(const Instance &tsp) {
  std::list<int> path;
  std::list<int> nodes;
  for (int i = 0; i < tsp.n(); i++) {
    nodes.push_back(i);
  }

  {
    int i, j;
    double dist = 0;
    for (auto a = nodes.begin(); a != nodes.end(); ++a) {
      for (auto b = nodes.begin(); b != nodes.end(); ++b) {
        if (a != b && tsp.cost(*a, *b) > dist) {
          dist = tsp.cost(*a, *b);
          i = *a;
          j = *b;
        }
      }
    }
    path.push_back(i);
    path.push_back(j);
  }

  while (nodes.size() > 0) {
    int r;
    {
      auto far_it = nodes.end();
      double dist = 0;
      for (auto v = nodes.begin(); v != nodes.end(); ++v) {
        for (auto j = path.begin(); j != path.end(); ++j) {
          if (tsp.cost(*v, *j) > dist) {
            dist = tsp.cost(*v, *j);
            far_it = v;
          }
        }
      }
      r = *far_it;
      nodes.erase(far_it);
    }

    {
      auto ins_before = path.end(); // equivalent to path.begin()
      double cost = tsp.cost(path.back(), r) + tsp.cost(r, path.front()) -
                    tsp.cost(path.back(), path.front());

      auto i = path.begin(); // i != path.end()
      auto j = i;
      ++j; // j != path.end() since path.size() >= 2
      for (; j != path.end(); ++i, ++j) {
        double c = tsp.cost(*i, r) + tsp.cost(r, *j) - tsp.cost(*i, *j);
        if (c < cost) {
          cost = c;
          ins_before = j;
        }
      }
      path.insert(ins_before, r);
    }
  }

  std::vector<int> vec(path.begin(), path.end());
  return Path(vec);
}