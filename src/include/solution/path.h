#ifndef TSP_SOLUTION_PATH_H
#define TSP_SOLUTION_PATH_H

#include <vector>

#include "solution.h"

namespace TSP {

namespace solution {

class Path : public Solution {
  std::vector<int> sequence;

public:
  Path(const Instance &tsp);
  Path(const Path &tspSol) = default;
  Path(const std::vector<int> &seq);

  class Traveller : public Solution::Traveller {
    friend Path;
    const std::vector<int> &seq;
    size_t idx;
    Traveller(const std::vector<int> &seq);

  public:
    int next() override;
  };

  std::size_t size() const override;
  std::unique_ptr<Solution::Traveller> traveller() const override;
  void print() const override;
  int get_nth(int) const;

  /**
   * set a solution as a random sequence by random swaps
   * @param sol path to be initialized
   * @return true if everything OK, false otherwise
   */
  static bool randomize(Path &sol);
};

} // namespace solution

} // namespace TSP

#endif // TSP_SOLUTION_PATH_H
