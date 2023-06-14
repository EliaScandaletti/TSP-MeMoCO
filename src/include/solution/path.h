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
  double evaluate(const Instance &tsp) const override;

  int get_nth(int) const;

  /**
   * Class representing substring reversal move
   */
  struct opt2 {
    int from;
    int to;
    bool operator==(const opt2 &other) const;
  };
  double evaluate_move(const Instance &tsp, const opt2 &m) const;
  void apply_move(const opt2 &m);

  struct reposition {
    int node;
    int after;
    bool operator==(const reposition &other) const;
  };
  double evaluate_move(const Instance &tsp, const reposition &m) const;
  void apply_move(const reposition &m);

  /**
   * Struct representing a 2.5-opt move
   * https://www.cs.ubc.ca/~hutter/previous-earg/EmpAlgReadingGroup/TSP-JohMcg97.pdf
   * Move the node `node` after the node `after`
   */
  class opt2_5 {
    friend Path;
    enum { OPT2, REP } tag;
    union {
      opt2 _opt2;
      reposition _rep;
    };

  public:
    static opt2_5 from(const opt2 &m);
    static opt2_5 from(const reposition &m);
    bool operator==(const opt2_5 &other) const;
  };
  double evaluate_move(const Instance &tsp, const opt2_5 &m) const;
  void apply_move(const opt2_5 &m);

  /**
   * set a solution as a random sequence by random swaps
   * @param sol path to be initialized
   * @return true if everything OK, false otherwise
   */
  static bool randomize(Path &sol);
  void print() const;
};

} // namespace solution

} // namespace TSP

#endif // TSP_SOLUTION_PATH_H
