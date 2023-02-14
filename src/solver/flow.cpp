#include "solver/flow.h"

#include <iostream>
#define STRINGIZE_HELPER(something) #something

bool error(int status) {
  if (status) {
    char errmsg[CPXMESSAGEBUFSIZE];
    CPXgeterrorstring(NULL, status, errmsg);
    std::cerr << std::string(__FILE__) << ":" << __LINE__ << " : " << errmsg;
    return true;
  }
  return false;
}

TSP::solver::Flow::Flow(const Instance &tsp) : _n(tsp.n()) {
  CPXopenCPLEX(&status);
  if (error(status))
    return;
  CPXcreateprob(env, &status, "TSP");
  if (error(status))
    return;
}

TSP::solver::Flow::~Flow() {
  CPXfreeprob(env, &prob);
  CPXcloseCPLEX(&env);
}

bool TSP::solver::Flow::solve(const Instance &tsp, Solution &sol) {
  return false;
}

double TSP::solver::Flow::evaluate(const Instance &tsp,
                                   const Solution &sol) const {
  return 0.0;
}
