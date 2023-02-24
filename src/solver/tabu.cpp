#include "solver/tabu.h"

#include <algorithm>
#include <deque>
#include <iostream>

using namespace TSP::solver;
using TSP::Instance;

double evaluate_opt2(const Instance &tsp, const Path &currSol,
                     const Path::opt2 &m) {
  int i = m.from, j = m.to;
  return -tsp.cost(currSol.get_nth(i - 1), currSol.get_nth(i))   // - c_{i-1, i}
         - tsp.cost(currSol.get_nth(j), currSol.get_nth(j + 1))  // - c_{j, j+1}
         + tsp.cost(currSol.get_nth(i - 1), currSol.get_nth(j))  // + c_{i-1, j}
         + tsp.cost(currSol.get_nth(i), currSol.get_nth(j + 1)); // + c_{i, j+1}
}

bool next_opt2(const Instance &tsp, const Path &currSol, Path::opt2 &m,
               const std::deque<Path::opt2> &tabu, double acc_inc) {
  double best_inc = -1e-6;
  bool found = false;
  for (int i = 1; i < tsp.n() - 1; i++) {
    for (int j = i + 1; j < tsp.n(); j++) {
      Path::opt2 mov = {i, j};
      double inc = evaluate_opt2(tsp, currSol, mov);

      if (inc >= acc_inc) {
        bool is_tabu = std::find(tabu.begin(), tabu.end(), mov) != tabu.end();
        if (is_tabu) {
          continue;
        }
      }

      if (inc < best_inc) {
        best_inc = inc;
        m = mov;
        found = true;
      }
    }
  }
  return found;
}

double evaluate_opt2_5(const Instance &tsp, const Path &currSol,
                       const Path::opt2_5 &m) {
  int i = m.node, j = m.after;
  return -tsp.cost(currSol.get_nth(i - 1), currSol.get_nth(i))  // - c_{i-1, i}
         - tsp.cost(currSol.get_nth(i), currSol.get_nth(i + 1)) // - c_{i, i+1}
         + tsp.cost(currSol.get_nth(i - 1),
                    currSol.get_nth(i + 1)) // + c_{i-1, i+1}
         - tsp.cost(currSol.get_nth(j), currSol.get_nth(j + 1))  // - c_{j, j+1}
         + tsp.cost(currSol.get_nth(j), currSol.get_nth(i))      // + c_{j, i}
         + tsp.cost(currSol.get_nth(i), currSol.get_nth(j + 1)); // + c_{i, j+1}
}

bool next_opt2_5(const Instance &tsp, const Path &currSol, Path::opt2_5 &m) {
  double best_inc = -1e-6;
  bool found = false;
  for (int i = 0; i < tsp.n(); i++) {
    for (int j = 0; j < tsp.n(); j++) {
      if (i == j || i == j + 1)
        continue;

      Path::opt2_5 mov = {i, j};
      double inc = evaluate_opt2_5(tsp, currSol, mov);

      if (inc < best_inc) {
        best_inc = inc;
        m = mov;
        found = true;
      }
    }
  }
  return found;
};

int Tabu::solve(const Instance &tsp, Path &sol, size_t tabu_size,
                int max_iter) {
  bool stop = false;
  int iter = 0;

  Path curr_sol = sol;
  double best_val, curr_val;
  best_val = curr_val = curr_sol.evaluate(tsp);

  std::deque<Path::opt2> tabu_list;

  while (!stop && iter < max_iter) {
    stop = true;

    Path::opt2 m2;
    bool does_move =
        next_opt2(tsp, curr_sol, m2, tabu_list, best_val - curr_val);
    while (does_move && iter < max_iter) {
      stop = false;
      ++iter;

      // apply the move
      curr_val += evaluate_opt2(tsp, curr_sol, m2);
      curr_sol.apply_opt2(m2);
      // update tabu list
      tabu_list.push_back(m2);
      if (tabu_list.size() > tabu_size)
        tabu_list.pop_front();
      // check if curr_sol is the best so far
      if (curr_val < best_val) {
        best_val = curr_val;
        sol = curr_sol;
      }
      // find next move
      does_move = next_opt2(tsp, curr_sol, m2, tabu_list, best_val - curr_val);
    }

    // either there is a local minimum or max_iter reached
    if (iter < max_iter) {
      // uses 2.5-opt to move past the local minimum
      Path::opt2_5 m2_5;
      does_move = next_opt2_5(tsp, curr_sol, m2_5);

      if (does_move) {
        stop = false;
        ++iter;
        // apply the move
        curr_val += evaluate_opt2_5(tsp, curr_sol, m2_5);
        curr_sol.apply_opt2_5(m2_5);
        // check if curr_sol is the best so far
        if (curr_val < best_val) {
          best_val = curr_val;
          sol = curr_sol;
        }
      }
    }
  }
  return iter;
}