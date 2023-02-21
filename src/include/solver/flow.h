#include <ilcplex/cplex.h>
#include <vector>

#include "instance.h"
#include "solution/path.h"

using TSP::solution::Path;

namespace TSP {

namespace solver {

class Flow {
  int status;
  CPXENVptr env;
  CPXLPptr prob;

  unsigned int _n;
  std::vector<std::vector<int>> x_idx;
  std::vector<std::vector<int>> y_idx;
  std::vector<std::vector<double>> cost;

public:
  Flow(const Instance &);
  ~Flow();
  Path solve();

  double evaluate(const Path &) const;
};

} // namespace solver

} // namespace TSP
