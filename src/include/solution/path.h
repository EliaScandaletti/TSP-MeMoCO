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

  std::size_t length() const override;

  int get_nth(int) const;

  /**
   * Class representing substring reversal move
   */
  struct opt2 {
    int from;
    int to;
    bool operator==(const opt2 &other) const;
  };
  void apply_opt2(const opt2 &m);

  /**
   * Struct representing a 2.5-opt move
   * https://www.cs.ubc.ca/~hutter/previous-earg/EmpAlgReadingGroup/TSP-JohMcg97.pdf
   * Move the node `node` after the node `after`
   */
  struct opt2_5 {
    int node;
    int after;
  };
  void apply_opt2_5(const opt2_5 &m);

  /**
   * set a solution as a random sequence by random swaps
   * @param sol path to be initialized
   * @return true if everything OK, false otherwise
   */
  static bool randomize(Path &sol);
  void print() const;

  class Traveller {
    friend Path;
    const std::vector<int> &seq;
    std::size_t idx;
    Traveller(const std::vector<int> &seq);

  public:
    int next();
  };
  Traveller traveller() const;
};

} // namespace solution

} // namespace TSP

#endif // TSP_SOLUTION_PATH_H
