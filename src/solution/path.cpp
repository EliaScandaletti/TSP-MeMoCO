#include "solution/path.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "instance.h"

using namespace TSP::solution;
using TSP::Instance;

Path::Path(const Instance &tsp) {
  sequence.reserve(tsp.n() + 1);
  for (int i = 0; i < tsp.n(); ++i) {
    sequence.push_back(i);
  }
  sequence.push_back(0);
}

std::size_t TSP::solution::Path::size() const { return sequence.size(); }

void TSP::solution::Path::print() const {
  std::cout << "[" << sequence[0];
  for (uint i = 1; i < size(); i++) {
    std::cout << " " << sequence[i];
  }
  std::cout << "]";
}

int TSP::solution::Path::get_nth(int i) const { return sequence[i]; }

bool TSP::solution::Path::randomize(Path &sol) {
  srand(time(NULL));
  for (uint i = 1; i < sol.size(); ++i) {
    // intial and final position are fixed (initial/final node remains 0)
    int idx1 = rand() % (sol.sequence.size() - 2) + 1;
    int idx2 = rand() % (sol.sequence.size() - 2) + 1;
    int tmp = sol.sequence[idx1];
    sol.sequence[idx1] = sol.sequence[idx2];
    sol.sequence[idx2] = tmp;
  }
  // std::cout << "### ";
  // sol.print();
  // std::cout << " ###" << std::endl;
  return true;
}
