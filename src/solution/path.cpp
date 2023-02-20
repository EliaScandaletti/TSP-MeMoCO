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

Path::Path(const std::vector<int> &seq) : sequence(seq) {}

Path::Traveller::Traveller(const std::vector<int> &seq) : seq(seq), idx(0) {}

int Path::Traveller::next() { return seq[idx++ % seq.size()]; }

std::size_t TSP::solution::Path::size() const { return sequence.size(); }

std::unique_ptr<TSP::Solution::Traveller>
TSP::solution::Path::traveller() const {
  return std::unique_ptr<TSP::Solution::Traveller>(new Traveller(sequence));
}

void TSP::solution::Path::print() const {
  std::cout << "[" << sequence[0];
  for (uint i = 1; i < size(); i++) {
    std::cout << " " << sequence[i];
  }
  std::cout << "]";
}

int TSP::solution::Path::get_nth(int i) const { return sequence[i]; }

void TSP::solution::Path::apply_move(const opt2 &m) {
  int i = m.from, j = m.to;
  while (i < j) {
    int t = sequence[i];
    sequence[i] = sequence[j];
    sequence[j] = t;
    ++i;
    --j;
  }
}

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
