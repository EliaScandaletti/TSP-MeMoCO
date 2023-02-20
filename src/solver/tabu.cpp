#include "solver/tabu.h"

#include <iostream>

using namespace TSP::solver;
using TSP::Instance;

double Tabu::evaluate(const Instance &tsp, const Path &sol) const {
  double total = 0.0;
  for (uint i = 0; i < sol.size() - 1; ++i) {
    int from = sol.get_nth(i);
    int to = sol.get_nth(i + 1);
    total += tsp.cost(from, to);
  }
  return total;
}

double evaluate_move(const Instance &tsp, const Path &currSol,
                     const Path::opt2 &m) {
  size_t i = m.from, j = m.to;
  int ip = i > 0 ? i - 1 : currSol.size() - 1;
  int jn = j < currSol.size() - 2 ? j + 1 : 0;
  return -tsp.cost(currSol.get_nth(ip), currSol.get_nth(i))  //
         - tsp.cost(currSol.get_nth(j), currSol.get_nth(jn)) //
         + tsp.cost(currSol.get_nth(ip), currSol.get_nth(j)) //
         + tsp.cost(currSol.get_nth(i), currSol.get_nth(jn));
}

Path::opt2 next_opt2(const Instance &tsp, const Path &currSol) {
  Path::opt2 best_m;
  double best_inc = tsp.infinite;
  for (int i = 0; i < tsp.n(); i++) {
    for (int j = i + 1; j < tsp.n(); j++) {
      Path::opt2 m = {i, j};
      double inc = evaluate_move(tsp, currSol, m);
      if (inc < best_inc) {
        best_inc = inc;
        best_m = m;
      }
    }
  }
  return best_m;
}

bool Tabu::solve(const Instance &tsp, Path &sol) {
  try {
    bool stop = false;
    // int iter = 0;

    double bestValue, currValue;
    bestValue = currValue = evaluate(tsp, sol);

    while (!stop) {
      stop = true;
      Path::opt2 m = next_opt2(tsp, sol);
      double inc = evaluate_move(tsp, sol, m);
      while (inc < -1e-6) {
        stop = false;
        bestValue += inc;
        sol.apply_move(m);
        m = next_opt2(tsp, sol);
        inc = evaluate_move(tsp, sol, m);
      }
    }

  } catch (std::exception &e) {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    return false;
  }
  return true;
}