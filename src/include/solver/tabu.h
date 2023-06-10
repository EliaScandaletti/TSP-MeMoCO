#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <ostream>
#include <vector>

#include "solution/path.h"

using TSP::solution::Path;

namespace TSP {

namespace solver {

/**
 * Class that solves a TSP problem by neighborhood search and 2-opt moves
 */
class Tabu {
public:
  Tabu() = default;

  int solve(const Instance &tsp, Path &bestSol, std::size_t tabu_size,
            int max_non_dec_iter, int max_iter, std::ostream *debug = nullptr);
};

} // namespace solver

} // namespace TSP

#endif /* TSPSOLVER_H */
