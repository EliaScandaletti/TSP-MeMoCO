#include <ctime>
#include <stdexcept>
#include <string>
#include <sys/time.h>

#include "instance/matrix.h"
#include "solution/path.h"
#include "solver/farthest.h"
#include "solver/tabu.h"

using namespace TSP::instance;
using namespace TSP::solution;
using namespace TSP::solver;

// error status and message buffer
int status;
char errmsg[255];

int main(int argc, char const *argv[]) {
  try {
    if (argc < 4)
      throw std::runtime_error(
          "usage: ./main filename.dat tabu_length max_iter");

    /// create the instance (reading data)
    Matrix instance(argv[1]);
    int tabu_length = std::stoi(argv[2]);
    int max_iter = std::stoi(argv[3]);

    // CPU time (t2 - t1)
    clock_t t1, t2;

    /// create solver class
    Tabu tabu_solver;
    FarthestInsertion far_ins_solver;

    double cum_sol = 0;
    double cum_iter = 0;
    double cum_time = 0;
    for (int j = 0; j < 10; j++) {
      /// initial solution
      Path solution = far_ins_solver.solve(instance);
      t1 = clock();
      /// run the neighborhood search
      int iter = tabu_solver.solve(instance, solution, tabu_length, max_iter);
      t2 = clock();
      cum_sol += solution.evaluate(instance);
      cum_iter += iter;
      cum_time += (double)(t2 - t1) / CLOCKS_PER_SEC;
    }
    std::cout << argv[1] << "\t" << instance.n() << "\t" << tabu_length << "\t"
              << cum_sol / 10 << "\t" << cum_iter / 10 << "\t" << cum_time / 10
              << std::endl;

  } catch (std::exception &e) {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  }
  return 0;
}
