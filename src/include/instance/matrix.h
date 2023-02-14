#ifndef TSP_INSTANCE_MATRIX_H
#define TSP_INSTANCE_MATRIX_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "instance.h"

namespace TSP {

namespace instance {

// Matrix representation of a TSP instance
class Matrix : public Instance {
  // number of nodes
  int _n;
  // cost matrix
  std::vector<std::vector<double>> _cost;

public:
  Matrix(const char *filename);
  void read(const char *filename);
  int n() const override;
  double cost(int i, int j) const override;
};

} // namespace instance

} // namespace TSP

#endif // TSP_INSTANCE_MATRIX_H
