#include "solver/tabu.h"

#include <algorithm>
#include <deque>

using namespace TSP::solver;
using TSP::Instance;

bool next_opt2(const Instance &tsp, const Path &currSol, Path::opt2 &m,
               bool &improving, double &best_inc,
               const std::deque<Path::opt2_5> &tabu, double acc_inc) {
  best_inc = -1e-6;
  double worst_inc = 0;
  Path::opt2 worst;
  bool found = false;
  improving = false;

  for (int i = 1; i < tsp.n() - 1; i++) {
    for (int j = i + 1; j < tsp.n(); j++) {
      Path::opt2 mov = {i, j};
      Path::opt2_5 _mov = Path::opt2_5::from(mov);

      double inc = currSol.evaluate_opt2(tsp, mov);
      bool is_tabu = !(inc < acc_inc) &&
                     std::find(tabu.begin(), tabu.end(), _mov) != tabu.end();

      if (!is_tabu) {
        found = true;
        if (inc < best_inc) {
          best_inc = inc;
          m = mov;
          improving = true;
        } else if (!improving && inc > worst_inc) {
          worst_inc = inc;
          worst = mov;
        }
      }
    }
  }

  if (!improving) {
    m = worst;
  }

  return found;
}

bool next_reposition(const Instance &tsp, const Path &currSol,
                     Path::reposition &m, bool &improving, double &best_inc,
                     const std::deque<Path::opt2_5> &tabu, double acc_inc) {
  best_inc = -1e-6;
  double worst_inc = 0;
  Path::reposition worst;
  bool found = false;
  improving = false;

  for (int i = 0; i < tsp.n(); i++) {
    for (int j = 0; j < tsp.n(); j++) {
      if (i == j || i == j + 1)
        continue;
      Path::reposition mov = {i, j};
      Path::opt2_5 _mov = Path::opt2_5::from(mov);

      double inc = currSol.evaluate_reposition(tsp, mov);
      bool is_tabu = !(inc < acc_inc) &&
                     std::find(tabu.begin(), tabu.end(), _mov) != tabu.end();

      if (!is_tabu) {
        found = true;
        if (inc < best_inc) {
          best_inc = inc;
          m = mov;
          improving = true;
        } else if (!improving && inc > worst_inc) {
          worst_inc = inc;
          worst = mov;
        }
      }
    }
  }

  if (!improving) {
    m = worst;
  }

  return found;
}

bool next_opt2_5(const Instance &tsp, const Path &currSol, Path::opt2_5 &m,
                 bool &improving, const std::deque<Path::opt2_5> &tabu,
                 double acc_inc) {
  Path::opt2 m_2;
  double inc_2;
  bool improving_2;
  bool found_2 =
      next_opt2(tsp, currSol, m_2, improving_2, inc_2, tabu, acc_inc);

  Path::reposition m_rep;
  double inc_rep;
  bool improving_rep;
  bool found_rep = next_reposition(tsp, currSol, m_rep, improving_rep, inc_rep,
                                   tabu, acc_inc);

  bool opt2_best;
  if (found_2) {
    if (found_rep) {
      // found_2 && found_rep
      opt2_best = inc_2 < inc_rep;

      // if both moves are not improving, the worse move should be picked
      if (!improving_2 && !improving_rep)
        opt2_best = !opt2_best;

    } else {
      // found && !found_rep
      opt2_best = true;
    }
  } else {
    // !found_2
    opt2_best = false;
  }

  if (opt2_best) {
    m = Path::opt2_5::from(m_2);
    improving = improving_2;
  } else {
    m = Path::opt2_5::from(m_rep);
    improving = improving_rep;
  }

  return found_2 || found_rep;
}

int Tabu::solve(const Instance &tsp, Path &sol, size_t tabu_size,
                int max_non_imp_iter, int max_iter, std::ostream *debug) {
  int iter = 0;
  int non_imp_iter = 0;

  Path curr_sol = sol;
  double best_val, curr_val;
  best_val = curr_val = curr_sol.evaluate(tsp);

  if (debug) {
    *debug << iter << ", " << best_val << ", " << curr_val << ", "
           << non_imp_iter << std::endl;
  }

  std::deque<Path::opt2_5> tabu_list;

  while (non_imp_iter < max_non_imp_iter && iter < max_iter) {

    Path::opt2_5 move;
    bool improving;
    bool can_move = next_opt2_5(tsp, curr_sol, move, improving, tabu_list,
                                best_val - curr_val);
    while (can_move && non_imp_iter < max_non_imp_iter && iter < max_iter) {
      ++iter;
      ++non_imp_iter;

      // apply the move
      curr_val += curr_sol.evaluate_opt2_5(tsp, move);
      curr_sol.apply_opt2_5(move);

      // update tabu list
      tabu_list.push_back(move);
      if (tabu_list.size() > tabu_size)
        tabu_list.pop_front();

      // check if curr_sol is the best so far
      if (curr_val < best_val - 1e-6) {
        best_val = curr_val;
        sol = curr_sol;
        non_imp_iter = 0;
      }

      if (debug) {
        *debug << iter << ", " << best_val << ", " << curr_val << ", "
               << non_imp_iter << std::endl;
      }

      // find next move
      can_move = next_opt2_5(tsp, curr_sol, move, improving, tabu_list,
                             best_val - curr_val);
    }
  }
  return iter;
}