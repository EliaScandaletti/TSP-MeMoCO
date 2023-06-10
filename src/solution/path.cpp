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

double TSP::solution::Path::evaluate(const Instance &tsp) const {
  double total = 0.0;
  uint i;
  for (i = 0; i < length() - 1; ++i) {
    int from = get_nth(i);
    int to = get_nth(i + 1);
    total += tsp.cost(from, to);
  }
  return total + tsp.cost(get_nth(i), get_nth(0));
}

int TSP::solution::Path::get_nth(int i) const {
  i += sequence.size();
  i %= sequence.size();
  return sequence[i];
}

bool TSP::solution::Path::opt2::operator==(const opt2 &other) const {
  return from == other.from && to == other.to;
}

double TSP::solution::Path::evaluate_opt2(const Instance &tsp,
                                          const opt2 &m) const {
  int i = m.from, j = m.to;
  return -tsp.cost(get_nth(i - 1), get_nth(i))   // - c_{i-1, i}
         - tsp.cost(get_nth(j), get_nth(j + 1))  // - c_{j, j+1}
         + tsp.cost(get_nth(i - 1), get_nth(j))  // + c_{i-1, j}
         + tsp.cost(get_nth(i), get_nth(j + 1)); // + c_{i, j+1}
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

bool TSP::solution::Path::reposition::operator==(
    const reposition &other) const {
  return node == other.node && after == other.after;
}

double TSP::solution::Path::evaluate_reposition(const Instance &tsp,
                                                const reposition &m) const {
  int i = m.node, j = m.after;
  return -tsp.cost(get_nth(i - 1), get_nth(i))  // - c_{i-1, i}
         - tsp.cost(get_nth(i), get_nth(i + 1)) // - c_{i, i+1}
         + tsp.cost(get_nth(i - 1),
                    get_nth(i + 1))              // + c_{i-1, i+1}
         - tsp.cost(get_nth(j), get_nth(j + 1))  // - c_{j, j+1}
         + tsp.cost(get_nth(j), get_nth(i))      // + c_{j, i}
         + tsp.cost(get_nth(i), get_nth(j + 1)); // + c_{i, j+1}
}

void TSP::solution::Path::apply_reposition(const reposition &m) {
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

TSP::solution::Path::opt2_5 TSP::solution::Path::opt2_5::from(const opt2 &m) {
  opt2_5 mov;
  mov.tag = OPT2;
  mov._opt2 = m;
  return mov;
}

TSP::solution::Path::opt2_5
TSP::solution::Path::opt2_5::from(const reposition &m) {
  opt2_5 mov;
  mov.tag = REP;
  mov._rep = m;
  return mov;
}

bool TSP::solution::Path::opt2_5::operator==(const opt2_5 &other) const {
  if (tag != other.tag)
    return false;
  switch (tag) {
  case opt2_5::OPT2:
    return _opt2 == other._opt2;
  case opt2_5::REP:
    return _rep == other._rep;
  default:
    return false;
  }
}

double TSP::solution::Path::evaluate_opt2_5(const Instance &tsp,
                                            const opt2_5 &m) const {
  switch (m.tag) {
  case opt2_5::OPT2:
    return evaluate_opt2(tsp, m._opt2);
  case opt2_5::REP:
    return evaluate_reposition(tsp, m._rep);
  default:
    return false;
  }
}

void TSP::solution::Path::apply_opt2_5(const opt2_5 &m) {
  switch (m.tag) {
  case opt2_5::OPT2:
    apply_opt2(m._opt2);
    break;
  case opt2_5::REP:
    apply_reposition(m._rep);
    break;
  }
}

bool TSP::solution::Path::randomize(Path &sol) {
  srand(time(NULL));
  for (uint i = 1; i < sol.length(); ++i) {
    // initial and final position are fixed (initial/final node remains 0)
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
