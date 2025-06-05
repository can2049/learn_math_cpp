#include <ceres/ceres.h>

#include <chrono>
#include <cmath>
#include <cstdlib>  // for std::system
#include <fstream>
#include <iomanip>  // for std::setprecision, std::scientific
#include <iostream>
#include <limits>  // for std::numeric_limits
#include <random>
#include <typeindex>
#include <vector>

struct CostFunctor {
  template <typename T>
  bool operator()(const T* const x, T* residual) const {
    residual[0] = 10.0 - x[0];
    return true;
  }

  ~CostFunctor() {
    // Destructor implementation
    std::cout << __func__ << " called" << std::endl;
  }
};
int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  // The variable to solve for with its initial value.
  constexpr double initial_x = 50.0;
  double x = initial_x;

  // Build the problem.
  ceres::Problem problem;

  // Set up the only cost function (also known as residual). This uses
  // auto-differentiation to obtain the derivative (jacobian).
  ceres::CostFunction* cost_function =
      new ceres::AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor());
  problem.AddResidualBlock(cost_function, nullptr, &x);

  // Run the solver!
  ceres::Solver::Options options;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = true;
  ceres::Solver::Summary summary;
  Solve(options, &problem, &summary);

  std::cout << summary.BriefReport() << "\n";
  std::cout << "x : " << initial_x << " -> " << x << "\n";
  return 0;
}
