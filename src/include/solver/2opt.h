#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <vector>

#include "solution/path.h"

using TSP::solution::Path;

namespace TSP {

namespace solver {

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class Opt2 {
public:
  /**
   * Class representing substring reversal move
   */
  struct move {
    int from;
    int to;
  };

  Opt2() = default;

  double evaluate(const Instance &tsp, const Path &sol) const;

  bool solve(const Instance &tsp, Path &bestSol);

protected:
  // TODO: declare here any "internal" method
};

} // namespace solver

} // namespace TSP

#endif /* TSPSOLVER_H */
