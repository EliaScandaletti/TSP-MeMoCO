#include <ctime>
#include <stdexcept>
#include <sys/time.h>

#include "instance/matrix.h"
#include "solution/path.h"
#include "solver/flow.h"

using namespace TSP::instance;
using namespace TSP::solution;
using namespace TSP::solver;

// error status and message buffer
int status;
char errmsg[255];

int main(int argc, char const *argv[]) {
  try {
    if (argc < 3)
      throw std::runtime_error("usage: ./main filename.dat timeout");

    /// create the instance (reading data)
    Matrix instance(argv[1]);
    int timeout = std::stoi(argv[2]);

    ///  CPU time (t2 - t1)
    clock_t t1, t2;
    t1 = clock();

    /// create solver class
    Flow solver(instance);

    /// run the neighborhood search
    Path solution = solver.solve(timeout);

    /// final clocks
    t2 = clock();

    std::cout << argv[1] << "\t" << instance.n() << "\t" << solver.lower_bound()
              << "\t" << solver.evaluate() << "\t"
              << (double)(t2 - t1) / CLOCKS_PER_SEC << std::endl;

  } catch (std::exception &e) {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  }
  return 0;
}
