#include "solver/tabu.h"

#include <algorithm>
#include <deque>
#include <limits>

using namespace TSP::solver;
using TSP::Instance;

bool next_opt2(const Instance &tsp, const Path &currSol, Path::opt2 &m,
               const std::deque<Path::opt2> &tabu, double acc_inc) {
  double best_inc = std::numeric_limits<double>::infinity();
  bool found = false;

  for (int i = 1; i < tsp.n() - 1; i++) {
    for (int j = i + 1; j < tsp.n(); j++) {
      Path::opt2 mov = {i, j};
      double inc = currSol.evaluate_opt2(tsp, mov);

      bool is_tabu = inc >= acc_inc &&
                     std::find(tabu.begin(), tabu.end(), mov) != tabu.end();

      if (!is_tabu && inc < best_inc) {
        best_inc = inc;
        m = mov;
        found = true;
      }
    }
  }
  return found;
}

bool next_reposition(const Instance &tsp, const Path &currSol,
                     Path::reposition &m) {
  double best_inc = -1e-6;
  bool found = false;
  for (int i = 0; i < tsp.n(); i++) {
    for (int j = 0; j < tsp.n(); j++) {
      if (i == j || i == j + 1)
        continue;

      Path::reposition mov = {i, j};
      double inc = currSol.evaluate_reposition(tsp, mov);

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
                int max_non_imp_iter, int max_iter) {
  bool local_minima = false;
  int iter = 0;
  int non_imp_iter = 0;

  Path curr_sol = sol;
  double best_val, curr_val;
  best_val = curr_val = curr_sol.evaluate(tsp);

  std::deque<Path::opt2> tabu_list;

  while (!local_minima && non_imp_iter < max_non_imp_iter && iter < max_iter) {
    local_minima = true;

    Path::opt2 m2;
    bool does_move =
        next_opt2(tsp, curr_sol, m2, tabu_list, best_val - curr_val);
    while (does_move && non_imp_iter < max_non_imp_iter && iter < max_iter) {
      local_minima = false;
      ++iter;
      ++non_imp_iter;

      // apply the move
      curr_val += curr_sol.evaluate_opt2(tsp, m2);
      curr_sol.apply_opt2(m2);
      // update tabu list
      tabu_list.push_back(m2);
      if (tabu_list.size() > tabu_size)
        tabu_list.pop_front();

      // check if curr_sol is the best so far
      if (curr_val < best_val) {
        best_val = curr_val;
        sol = curr_sol;
        non_imp_iter = 0;
      }
      // find next move
      does_move = next_opt2(tsp, curr_sol, m2, tabu_list, best_val - curr_val);
    }

    // either there is a local minimum or max_iter reached
    if (non_imp_iter < max_non_imp_iter && iter < max_iter) {
      // uses 2.5-opt to move past the local minimum
      Path::reposition m2_5;
      does_move = next_reposition(tsp, curr_sol, m2_5);

      if (does_move) {
        ++iter;
        ++non_imp_iter;
        // apply the move
        curr_val += curr_sol.evaluate_reposition(tsp, m2_5);
        curr_sol.apply_reposition(m2_5);
        // check if curr_sol is the best so far
        if (curr_val < best_val) {
          best_val = curr_val;
          sol = curr_sol;
          non_imp_iter = 0;
        }
      }
    }
  }
  return iter;
}