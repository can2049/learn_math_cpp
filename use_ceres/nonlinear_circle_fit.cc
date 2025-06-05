#include <ceres/ceres.h>

#include <iostream>

class circle_cost_function {
 private:
  // 在这里我们去定义需要加载的数据
  // 注意这个数据不是要估计的参数！
  double _x, _y;

 public:
  circle_cost_function(double x, double y) : _x(x), _y(y) {}
  // ceres-solver的套路，用这个operator来定义残差计算公式
  /**
   * @brief
   *
   * @param abc       这个就是你要估计的参数
   *                  abc这里是一个三维数组
   *                  abc[0] = x0, abc[1] = y0, abc[2] = radius
   * @param residual  这个就是残差结果
   */
  template <typename T>
  bool operator()(const T *const abc, T *residual) const {
    // 公式：R^2 - (x-x0)^2 - (y-y0)^2
    residual[0] = abc[2] - pow((T(_x) - abc[0]), 2) - pow((T(_y) - abc[1]), 2);
    return true;
  }
};

int main(int argc, char **argv) {
  double op_x_gd = 3.2;
  double op_y_gd = 1.5;
  double radius = 4.0;
  double abc[3] = {0, 0, 0};
  std::vector<double> x_data, y_data;
  for (int i = 0; i < 100; i++) {
    // sample from 0deg to 100deg
    // 注意：cmath的cos采用弧度制
    double x = op_x_gd + radius * cos(i * M_PI / 180);
    double y = op_y_gd + radius * sin(i * M_PI / 180);
    x_data.push_back(x);
    y_data.push_back(y);
  }

  ceres::Problem problem;
  for (int i = 0; i < 100; i++) {
    problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<circle_cost_function, 1, 3>(
            new circle_cost_function(x_data[i], y_data[i])),
        nullptr, abc);
  }

  ceres::Solver::Options options;
  options.linear_solver_type = ceres::DENSE_NORMAL_CHOLESKY;
  options.minimizer_progress_to_stdout = true;

  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem, &summary);
  // 输出结果
  std::cout << summary.BriefReport() << std::endl;
  std::cout << "x0 =  " << abc[0] << " y0 = " << abc[1]
            << " Radius = " << sqrt(abc[2]) << std::endl;
  return 0;
}
