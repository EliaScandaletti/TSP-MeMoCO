#ifndef FARTHEST_H
#define FARTHEST_H

#include <vector>

#include "solution/path.h"

using TSP::solution::Path;

namespace TSP {

namespace solver {

/**
 * Class that solves a TSP problem using farthest insertion algorithm
 */
class FarthestInsertion {
public:
  FarthestInsertion() = default;

  Path solve(const Instance &tsp);
};

} // namespace solver

} // namespace TSP

#endif /* FARTHEST_H */
