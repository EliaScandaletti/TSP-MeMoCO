#ifndef TSP_SOLUTION_H
#define TSP_SOLUTION_H

#include "instance.h"
#include <vector>

using TSP::Instance;

namespace TSP {

class Solution {

public:
  virtual std::size_t length() const = 0;

  virtual double evaluate(const Instance &tsp) const = 0;
};

} // namespace TSP

#endif // TSP_SOLUTION_H
