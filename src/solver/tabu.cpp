#include "solver/tabu.h"

#include <algorithm>
#include <deque>
#include <limits>

using namespace TSP::solver;
using TSP::Instance;

bool next_opt2(const Instance &tsp, const Path &currSol, Path::opt2 &m,
               double &best_inc, const std::deque<Path::opt2_5> &tabu,
               double acc_inc) {
  best_inc = std::numeric_limits<double>::infinity();
  bool found = false;

  for (int i = 1; i < tsp.n() - 1; i++) {
    for (int j = i + 1; j < tsp.n(); j++) {
      Path::opt2 mov = {i, j};
      Path::opt2_5 _mov = Path::opt2_5::from(mov);

      double inc = currSol.evaluate_move(tsp, mov);

      if (inc < best_inc) {
        bool is_tabu = !(inc < acc_inc) &&
                       std::find(tabu.begin(), tabu.end(), _mov) != tabu.end();
        if (!is_tabu) {
          best_inc = inc;
          m = mov;
          found = true;
        }
      }
    }
  }

  return found;
}

bool worst_opt2(const Instance &tsp, const Path &currSol, Path::opt2 &m,
                double &worst_inc) {
  worst_inc = -std::numeric_limits<double>::infinity();
  bool found = false;

  for (int i = 1; i < tsp.n() - 1; i++) {
    for (int j = i + 1; j < tsp.n(); j++) {
      Path::opt2 mov = {i, j};

      double inc = currSol.evaluate_move(tsp, mov);

      if (inc > worst_inc) {
        worst_inc = inc;
        m = mov;
        found = true;
      }
    }
  }

  return found;
}

bool next_reposition(const Instance &tsp, const Path &currSol,
                     Path::reposition &m, double &best_inc,
                     const std::deque<Path::opt2_5> &tabu, double acc_inc) {
  best_inc = std::numeric_limits<double>::infinity();
  bool found = false;

  for (int i = 0; i < tsp.n(); i++) {
    for (int j = 0; j < tsp.n(); j++) {
      if (i == j || i == j + 1)
        continue;
      Path::reposition mov = {i, j};
      Path::opt2_5 _mov = Path::opt2_5::from(mov);

      double inc = currSol.evaluate_move(tsp, mov);

      if (inc < best_inc) {
        bool is_tabu = !(inc < acc_inc) &&
                       std::find(tabu.begin(), tabu.end(), _mov) != tabu.end();
        if (!is_tabu) {
          best_inc = inc;
          m = mov;
          found = true;
        }
      }
    }
  }

  return found;
}

bool worst_reposition(const Instance &tsp, const Path &currSol,
                      Path::reposition &m, double &worst_inc) {
  worst_inc = -std::numeric_limits<double>::infinity();
  bool found = false;

  for (int i = 1; i < tsp.n() - 1; i++) {
    for (int j = i + 1; j < tsp.n(); j++) {
      Path::reposition mov = {i, j};

      double inc = currSol.evaluate_move(tsp, mov);

      if (inc > worst_inc) {
        worst_inc = inc;
        m = mov;
        found = true;
      }
    }
  }

  return found;
}

bool next_opt2_5(const Instance &tsp, const Path &currSol, Path::opt2_5 &m,
                 double &best_inc, const std::deque<Path::opt2_5> &tabu,
                 double acc_inc) {
  Path::opt2 m_2;
  double inc_2;
  bool found_2 = next_opt2(tsp, currSol, m_2, inc_2, tabu, acc_inc);

  Path::reposition m_rep;
  double inc_rep;
  bool found_rep = next_reposition(tsp, currSol, m_rep, inc_rep, tabu, acc_inc);

  bool opt2_best;
  if (found_2) {
    if (found_rep) {
      // found_2 && found_rep
      opt2_best = inc_2 < inc_rep;

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
    best_inc = inc_2;
  } else {
    m = Path::opt2_5::from(m_rep);
    best_inc = inc_rep;
  }

  return found_2 || found_rep;
}

bool worst_opt2_5(const Instance &tsp, const Path &currSol, Path::opt2_5 &m,
                  double &worst_inc) {
  Path::opt2 m_2;
  double inc_2;
  bool found_2 = worst_opt2(tsp, currSol, m_2, inc_2);

  Path::reposition m_rep;
  double inc_rep;
  bool found_rep = worst_reposition(tsp, currSol, m_rep, inc_rep);

  bool opt2_worst;
  if (found_2) {
    if (found_rep) {
      // found_2 && found_rep
      opt2_worst = inc_2 > inc_rep;

    } else {
      // found && !found_rep
      opt2_worst = true;
    }
  } else {
    // !found_2
    opt2_worst = false;
  }

  if (opt2_worst) {
    m = Path::opt2_5::from(m_2);
    worst_inc = inc_2;
  } else {
    m = Path::opt2_5::from(m_rep);
    worst_inc = inc_rep;
  }

  return found_2 || found_rep;
}

int Tabu::solve(const Instance &tsp, Path &best_sol, size_t tabu_size,
                int max_non_imp_tabu, int max_non_imp_global, int max_iter,
                std::ostream *debug) {
  int iter = 0;
  int non_imp_tabu = 0;
  int non_imp_global = 0;

  Path curr_sol = best_sol;
  double best_val, curr_val;
  best_val = curr_val = curr_sol.evaluate(tsp);

  if (debug) {
    *debug << "init: " << iter << ",\t" << best_val << ",\t" << curr_val
           << ",\t" << non_imp_tabu << ",\t" << non_imp_global << std::endl;
  }

  std::deque<Path::opt2_5> tabu_list;
  Path::opt2_5 move;
  double inc;
  bool can_move =
      next_opt2_5(tsp, curr_sol, move, inc, tabu_list, best_val - curr_val);
  bool worst = false;

  while (can_move && non_imp_global < max_non_imp_global && iter < max_iter) {
    ++iter;
    ++non_imp_tabu;
    ++non_imp_global;

    // apply `move' to `curr_sol'
    curr_val += curr_sol.evaluate_move(tsp, move);
    curr_sol.apply_move(move);

    // update the tabu list
    tabu_list.push_back(move);
    if (tabu_list.size() > tabu_size)
      tabu_list.pop_front();

    // check if `curr_sol' is the best so far
    if (curr_val < best_val - 1e-6) {
      best_val = curr_val;
      best_sol = curr_sol;
      non_imp_tabu = 0;
      non_imp_global = 0;
    }

    if (debug) {
      *debug << (worst ? "jump: " : "tabu: ") << iter << ",\t" << best_val
             << ",\t" << curr_val << ",\t" << non_imp_tabu << ",\t"
             << non_imp_global << std::endl;
    }

    // find the next move
    if (non_imp_tabu < max_non_imp_tabu) {
      can_move =
          next_opt2_5(tsp, curr_sol, move, inc, tabu_list, best_val - curr_val);
      worst = false;
    } else {
      // if the tabu search hasn't yield any improvement in the last
      // `max_non_imp_tabu' iterations, then tries to diversificate by picking
      // the worst possible move
      non_imp_tabu = 0;
      can_move = worst_opt2_5(tsp, curr_sol, move, inc);
      worst = true;
    }
  }

  return iter;
}