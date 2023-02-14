#include <ilcplex/cplex.h>

#include "instance.h"
#include "solution.h"
#include "solver.h"

namespace TSP {

namespace solver {

class Flow : public Solver<Instance, Solution> {
  int status;
  CPXENVptr env;
  CPXLPptr prob;

  int _n;

public:
  Flow(const Instance &);
  ~Flow();
  virtual bool solve(const Instance &, Solution &) override;

  virtual double evaluate(const Instance &, const Solution &) const override;
};

} // namespace solver

} // namespace TSP
