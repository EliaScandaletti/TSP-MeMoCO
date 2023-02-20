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
    // Opt2 solver;
    Flow solver(instance);

    /// initial solution (random)
    // Path bestSolution(instance);
    // Path::randomize(bestSolution);
    // Path aSolution(bestSolution);

    /// run the neighborhood search
    // solver.solve(instance, bestSolution);
    Path solution = solver.solve();

    /// final clocks
    t2 = clock();
    gettimeofday(&tv2, NULL);

    // std::cout << "FROM solution: ";
    // aSolution.print();
    // std::cout << "(value : " << solver.evaluate(instance, aSolution) <<
    // ")\n"; std::cout << "TO   solution: "; bestSolution.print(); std::cout <<
    // "(value : " << solver.evaluate(instance, bestSolution)
    //           << ")\n";
    // std::cout << "Found solution: ";
    // solution.print();
    // std::cout << "(value : " << solver.evaluate(solution) << ")\n";
    // std::cout << "in "
    //           << (double)(tv2.tv_sec + tv2.tv_usec * 1e-6 -
    //                       (tv1.tv_sec + tv1.tv_usec * 1e-6))
    //           << " seconds (user time)\n";
    // std::cout << "in " << (double)(t2 - t1) / CLOCKS_PER_SEC
    //           << " seconds (CPU time)\n";
    std::cout << argv[1] << "\t" << instance.n() << "\t"
              << solver.evaluate(solution) << "\t"
              << (double)(t2 - t1) / CLOCKS_PER_SEC << std::endl;

  } catch (std::exception &e) {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  }
  return 0;
}
