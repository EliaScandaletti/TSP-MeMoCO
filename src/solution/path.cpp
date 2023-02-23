#include "solution/path.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "instance.h"

using TSP::Instance;

TSP::solution::Path::Path(const Instance &tsp) {
  sequence.reserve(tsp.n());
  for (int i = 0; i < tsp.n(); ++i) {
    sequence.push_back(i);
  }
}

TSP::solution::Path::Path(const std::vector<int> &seq) : sequence(seq) {}

std::size_t TSP::solution::Path::length() const { return sequence.size(); }

int TSP::solution::Path::get_nth(int i) const {
  i += sequence.size();
  i %= sequence.size();
  return sequence[i];
}

bool TSP::solution::Path::opt2::operator==(const opt2 &other) const {
  return from == other.from && to == other.to;
}

void TSP::solution::Path::apply_opt2(const opt2 &m) {
  int i = m.from, j = m.to;
  while (i < j) {
    int t = sequence[i];
    sequence[i] = sequence[j];
    sequence[j] = t;
    ++i;
    --j;
  }
}

void TSP::solution::Path::apply_opt2_5(const opt2_5 &m) {
  int i = m.node, j = m.after;
  if (i < j) {
    int temp = sequence[i];
    while (i < j) {
      sequence[i] = sequence[i + 1];
      ++i;
    }
    sequence[i] = temp;
  } else {
    int temp = sequence[i];
    while (i > j + 1) {
      sequence[i] = sequence[i - 1];
      --i;
    }
    sequence[i] = temp;
  }
}

bool TSP::solution::Path::randomize(Path &sol) {
  srand(time(NULL));
  for (uint i = 1; i < sol.length(); ++i) {
    // intial and final position are fixed (initial/final node remains 0)
    int idx1 = rand() % (sol.sequence.size() - 1) + 1;
    int idx2 = rand() % (sol.sequence.size() - 1) + 1;
    int tmp = sol.sequence[idx1];
    sol.sequence[idx1] = sol.sequence[idx2];
    sol.sequence[idx2] = tmp;
  }
  return true;
}

void TSP::solution::Path::print() const {
  std::cout << "[" << sequence[0];
  for (uint i = 1; i < length(); i++) {
    std::cout << " " << sequence[i];
  }
  std::cout << " " << sequence[0] << "]";
}

TSP::solution::Path::Traveller::Traveller(const std::vector<int> &seq) : seq(seq), idx(0) {}

int TSP::solution::Path::Traveller::next() { return seq[idx++ % seq.size()]; }

TSP::solution::Path::Traveller TSP::solution::Path::traveller() const {
  return Traveller(sequence);
}
