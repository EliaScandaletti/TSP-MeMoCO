#ifndef TSP_INSTANCE_H
#define TSP_INSTANCE_H

#include <limits>

namespace TSP {

// TSP instance
class Instance {
public:
  // infinite value
  // (upper bound on the value of any feasible solution)
  const double infinite = std::numeric_limits<double>::infinity();

  // number of nodes
  virtual int n() const = 0;

  virtual double cost(int i, int j) const = 0;
};

} // namespace TSP

#endif // TSP_INSTANCE_H
