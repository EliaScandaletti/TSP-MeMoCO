#include "instance/matrix.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace TSP::instance;

Matrix::Matrix(const char *filename) { read(filename); }

void Matrix::read(const char *filename) {
  std::ifstream in(filename);
  // read size
  in >> _n;
  std::cout << "number of nodes n = " << _n << std::endl;
  // read costs
  _cost.resize(_n);
  for (int i = 0; i < _n; i++) {
    _cost[i].reserve(_n);
    for (int j = 0; j < _n; j++) {
      double c;
      in >> c;
      _cost[i].push_back(c);
    }
  }
  in.close();
}

int Matrix::n() const { return _n; }

double Matrix::cost(int i, int j) const { return _cost[i][j]; }
