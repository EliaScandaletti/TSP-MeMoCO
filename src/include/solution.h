#ifndef TSP_SOLUTION_H
#define TSP_SOLUTION_H

#include <memory>
#include <vector>

#include "instance.h"

namespace TSP {

class Solution {

public:
  virtual std::size_t length() const = 0;

  virtual void print() const = 0;

  /** assignment method
   * copy a solution into another one
   * @param right TSP solution to get into
   * @return ---
   */
  // Solution &operator=(const Solution &right) {
  //   // Handle self-assignment:
  //   if (this == &right)
  //     return *this;
  //   for (uint i = 0; i < sequence.size(); i++) {
  //     sequence[i] = right.sequence[i];
  //   }
  //   return *this;
  // }
};

} // namespace TSP

#endif // TSP_SOLUTION_H
