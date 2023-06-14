#include <fstream>
#include <iostream>

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
  std::ofstream *debug = nullptr;
  try {
    if (argc < 6)
      throw std::runtime_error("usage: ./main filename.dat tabu_length "
                               "max_non_imp_tabu max_non_imp_global max_iter");

    /// create the instance (reading data)
    Matrix instance(argv[1]);
    int tabu_length = std::stoi(argv[2]);
    int max_non_imp_tabu = std::stoi(argv[3]);
    int max_non_imp_global = std::stoi(argv[4]);
    int max_iter = std::stoi(argv[5]);
    if (argc > 6) {
      debug = new std::ofstream(argv[6]);
    }

    // CPU time (t2 - t1)
    clock_t t1, t2;

    /// create solver class
    Tabu tabu_solver;
    FarthestInsertion far_ins_solver;

    double cum_sol = 0;
    double cum_iter = 0;
    double cum_time = 0;
    for (int j = 0; j < 1; j++) {
      /// initial solution
      Path solution = far_ins_solver.solve(instance);
      t1 = clock();
      /// run the neighborhood search
      int iter =
          tabu_solver.solve(instance, solution, tabu_length, max_non_imp_tabu,
                            max_non_imp_global, max_iter, debug);
      t2 = clock();
      cum_sol += solution.evaluate(instance);
      cum_iter += iter;
      cum_time += (double)(t2 - t1) / CLOCKS_PER_SEC;
    }
    std::cout << argv[1] << "\t" << instance.n() << "\t" << tabu_length << "\t"
              << cum_sol / 1 << "\t" << cum_iter / 1 << "\t" << cum_time / 1
              << std::endl;

  } catch (std::exception &e) {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
  }
  if (debug) {
    debug->close();
    delete debug;
  }
  return 0;
}
