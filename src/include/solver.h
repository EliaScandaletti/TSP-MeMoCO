#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "instance.h"

namespace TSP {

template <typename instance_t, typename solution_t> class Solver {
public:
  /**
   * search for a good tour by neighbourhood search
   * @param TSP TSP data
   * @param initSol initial solution
   * @param bestSol best found solution (output)
   * @return true id everything OK, false otherwise
   */
  virtual bool solve(const instance_t &, solution_t &) = 0;

  /**
   * evaluate a solution
   * @param sol: solution to be evaluated
   * @param TSP: TSP data
   * @return the value of the solution
   */
  virtual double evaluate(const instance_t &, const solution_t &) const = 0;
};

} // namespace TSP

#endif