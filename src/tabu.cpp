#include <ctime>
#include <stdexcept>
#include <sys/time.h>

#include "instance/matrix.h"
#include "solution/path.h"
#include "solver/tabu.h"

using namespace TSP::instance;
using namespace TSP::solution;
using namespace TSP::solver;

// error status and message buffer
int status;
char errmsg[255];

int main(int argc, char const *argv[]) {
  try {
    if (argc < 2)
      throw std::runtime_error("usage: ./main filename.dat");

    /// create the instance (reading data)
    Matrix instance(argv[1]);

    /// initialize clocks for running time recording
    ///   two ways:
    ///   1) CPU time (t2 - t1)
    clock_t t1, t2;
    t1 = clock();
    ///   2) wall-clock time (tv2 - tv1)
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);

    /// create solver class
    Tabu solver();

    /// initial solution (random)
    Path solution(instance);
    Path::randomize(solution);

    /// run the neighborhood search
    solver.solve(instance, solution);

    /// final clocks
    t2 = clock();
    gettimeofday(&tv2, NULL);

    std::cout << "(value : " << solver.evaluate(instance, solution) << ")\n";
    std::cout << "Found solution: ";
    std::cout << "(value : " << solver.evaluate(solution) << ")\n";
    std::cout << "\t" << (double)(t2 - t1) / CLOCKS_PER_SEC << "" << std::endl;

  } catch (std::exception &e) {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  }
  return 0;
}
