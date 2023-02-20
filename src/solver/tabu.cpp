#include "solver/tabu.h"

#include <iostream>

using namespace TSP::solver;

double Tabu::evaluate(const Instance &tsp, const Path &sol) const {
  double total = 0.0;
  for (uint i = 0; i < sol.size() - 1; ++i) {
    int from = sol.get_nth(i);
    int to = sol.get_nth(i + 1);
    total += tsp.cost(from, to);
  }
  return total;
}

bool Tabu::solve(const Instance &tsp, Path &bestSol) {
  try {
    bool stop = false;
    // int iter = 0;

    Path currSol(tsp);
    // double bestValue, currValue;
    // bestValue = currValue = evaluate(tsp, currSol);
    // move mv;
    while (!stop) {
      /// TODO: local search iteration...
      if (true)
        stop = true;
    }
    bestSol = currSol;
  } catch (std::exception &e) {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    return false;
  }
  return true;
}