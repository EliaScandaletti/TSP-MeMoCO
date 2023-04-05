#include "solver/flow.h"

#include <iostream>
#define STRINGIZE_HELPER(something) #something

using TSP::solution::Path;
using TSP::solver::Flow;

bool error(int status) {
  if (status) {
    char errmsg[CPXMESSAGEBUFSIZE];
    CPXgeterrorstring(NULL, status, errmsg);
    std::cerr << std::string(__FILE__) << ":" << __LINE__ << " : " << errmsg;
    return true;
  }
  return false;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
Flow::Flow(const Instance &tsp)
    : _n(tsp.n()), x_idx(_n, std::vector<int>(_n, -1)), y_idx(x_idx),
      cost(_n, std::vector<double>(_n, 0)) {
  env = CPXopenCPLEX(&status);
  if (error(status))
    return;
  prob = CPXcreateprob(env, &status, "TSP");
  if (error(status))
    return;

  int idx = 0;

  { // insert y_{i,j} vars + create obj function
    double lb = 0;
    double ub = 1;
    char type = 'B';
    char name[50];
    char *names[] = {name};
    for (size_t i = 0; i < _n; i++) {
      for (size_t j = 0; j < _n; j++) {
        cost[i][j] = tsp.cost(i, j);
        if (i == j)
          continue;
        sprintf(name, "y_%ld_%ld", i, j);
        // o.f. = \sum c_{i,j}y_{i,j}
        double coeff[] = {cost[i][j]};
        status = CPXnewcols(env, prob, 1, coeff, &lb, &ub, &type, names);
        y_idx[i][j] = idx++;
        if (error(status))
          return;
      }
    }
  }
  { // insert x_{i,j} vars
    double lb = 0;
    double ub = +CPX_INFBOUND;
    char type = 'C';
    char name[50];
    char *names[] = {name};
    for (size_t i = 0; i < _n; i++) {
      for (size_t j = 1; j < _n; j++) {
        if (i == j)
          continue;
        sprintf(name, "x_%ld_%ld", i, j);
        status = CPXnewcols(env, prob, 1, nullptr, &lb, &ub, &type, names);
        x_idx[i][j] = idx++;
        if (error(status))
          return;
      }
    }
  }

  { // \sum x_{0,j} = |N| - 1
    int rmatbeg[] = {0};
    std::vector<double> coeff(_n - 1, 1);
    double rhs[] = {_n - 1};

    status = CPXaddrows(env, prob, 0, 1, _n - 1, rhs, "E", rmatbeg,
                        &x_idx[0][1], coeff.data(), nullptr, nullptr);
    if (error(status))
      return;
  }

  { // \forall k \in N \setminus {0}
    // \sum x_{i,k} + \sum_{j \neq 0} x_{k,j} = 1
    int rmatbeg[] = {0};
    double rhs[] = {1};
    for (size_t k = 1; k < _n; k++) {
      std::vector<double> coeff;
      std::vector<int> rmatind;
      coeff.reserve((2 * _n - 1) * (_n - 1));
      rmatind.reserve((2 * _n - 1) * (_n - 1));
      for (size_t i = 0; i < _n; i++) {
        if (i == k)
          continue;
        coeff.push_back(1);
        rmatind.push_back(x_idx[i][k]);
      }
      for (size_t j = 1; j < _n; j++) {
        if (k == j)
          continue;
        coeff.push_back(-1);
        rmatind.push_back(x_idx[k][j]);
      }

      status = CPXaddrows(env, prob, 0, 1, coeff.size(), rhs, "E", rmatbeg,
                          rmatind.data(), coeff.data(), nullptr, nullptr);
      if (error(status))
        return;
    }
  }

  { // \forall i \in N
    // \sum y_{i,j} = 1
    int rmatbeg[] = {0};
    std::vector<double> coeff(_n - 1, 1);
    double rhs[] = {1};
    for (size_t i = 0; i < _n; i++) {
      std::vector<int> rmatind;
      rmatind.reserve(_n - 1);
      for (size_t j = 0; j < _n; j++) {
        if (i == j)
          continue;
        rmatind.push_back(y_idx[i][j]);
      }

      status = CPXaddrows(env, prob, 0, 1, coeff.size(), rhs, "E", rmatbeg,
                          rmatind.data(), coeff.data(), nullptr, nullptr);
      if (error(status))
        return;
    }
  }

  { // \forall j \in N
    // \sum y_{i,j} = 1
    int rmatbeg[] = {0};
    std::vector<double> coeff(_n - 1, 1);
    double rhs[] = {1};
    for (size_t j = 0; j < _n; j++) {
      std::vector<int> rmatind;
      rmatind.reserve(_n - 1);
      for (size_t i = 0; i < _n; i++) {
        if (i == j)
          continue;
        rmatind.push_back(y_idx[i][j]);
      }

      status = CPXaddrows(env, prob, 0, 1, coeff.size(), rhs, "E", rmatbeg,
                          rmatind.data(), coeff.data(), nullptr, nullptr);
      if (error(status))
        return;
    }
  }

  { // \forall i \in N
    // \forall j \in N, j \neq 0
    // x_{i,j} - (|N| - 1)*y_{i,j} \leq 0
    int rmatbeg[] = {0};
    double coeff[] = {1, -((double)_n - 1)};
    double rhs[] = {0};
    for (size_t i = 0; i < _n; i++) {
      for (size_t j = 1; j < _n; j++) {
        if (i == j)
          continue;
        int rmatind[] = {x_idx[i][j], y_idx[i][j]};

        status = CPXaddrows(env, prob, 0, 1, 2, rhs, "L", rmatbeg, rmatind,
                            coeff, nullptr, nullptr);
        if (error(status))
          return;
      }
    }
  }
}
#pragma GCC diagnostic pop

Flow::~Flow() {
  CPXfreeprob(env, &prob);
  CPXcloseCPLEX(&env);
}

TSP::solution::Path Flow::solve() {
  status = CPXmipopt(env, prob);
  if (error(status))
    return solution::Path(std::vector<int>());

  double sol[_n * (_n - 1)];
  status = CPXgetx(env, prob, sol, 0, _n * (_n - 1) - 1);
  if (error(status))
    return solution::Path(std::vector<int>());

  std::vector<int> seq;
  seq.reserve(_n);

  int n = 0;
  while (n != 0 || seq.empty()) {
    seq.push_back(n);
    int m = 0;
    while (n == m || sol[y_idx[n][m]] < 0.5) {
      m++;
    }
    n = m;
  }

  return solution::Path(seq);
}

double Flow::evaluate(const Path &sol) const {
  auto traveller = sol.traveller();
  int n = traveller.next(); // = 0
  int m = traveller.next();
  double eval = 0;
  while (m != 0) {
    eval += cost[n][m];
    n = m;
    m = traveller.next();
  }
  return eval + cost[n][0];
}

double Flow::evaluate() const {
  double eval;
  int status = CPXgetobjval(env, prob, &eval);
  if (error(status))
    return 0.0;
  return eval;
}