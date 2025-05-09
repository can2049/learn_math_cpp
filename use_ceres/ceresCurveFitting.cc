#include <ceres/ceres.h>

#include <chrono>
#include <cmath>
#include <cstdlib>  // for std::system
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

// 曲线拟合的代价函数
struct CURVE_FITTING_COST {
  double x, y;
  CURVE_FITTING_COST(double x, double y) : x(x), y(y) {}

  template <typename T>
  bool operator()(const T* const abc, T* residual) const {
    residual[0] =
        T(y) - ceres::exp(abc[0] * T(x) * T(x) + abc[1] * T(x) + abc[2]);
    return true;
  }
};

int main(int argc, char** argv) {
  double ar = 1.0, br = 2.0, cr = 1.0;   // 真实参数
  double ae = 2.0, be = -1.0, ce = 5.0;  // 初始估计值
  int N = 100;
  double w_sigma = 1.0;

  std::mt19937 gen(std::random_device{}());
  std::normal_distribution<> dist{0, w_sigma};

  std::vector<double> x_data, y_data;
  for (int i = 0; i < N; ++i) {
    double x = i / 100.0;
    x_data.push_back(x);
    y_data.push_back(exp(ar * x * x + br * x + cr) + dist(gen));
  }

  double abc[3] = {ae, be, ce};

  // 构建最小二乘问题
  ceres::Problem problem;
  for (int i = 0; i < N; ++i) {
    problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 3>(
            new CURVE_FITTING_COST(x_data[i], y_data[i])),
        nullptr, abc);
  }

  // 配置求解器
  ceres::Solver::Options options;
  options.linear_solver_type = ceres::DENSE_NORMAL_CHOLESKY;
  options.minimizer_progress_to_stdout = false;

  ceres::Solver::Summary summary;
  auto t1 = std::chrono::steady_clock::now();
  ceres::Solve(options, &problem, &summary);
  auto t2 = std::chrono::steady_clock::now();

  std::chrono::duration<double> time_used = t2 - t1;
  std::cout << "solve time cost = " << time_used.count() << " seconds.\n";
  std::cout << summary.BriefReport() << std::endl;
  std::cout << "estimated a,b,c = " << abc[0] << " " << abc[1] << " " << abc[2]
            << "\n";

  // 将数据写入临时文件
  const std::string data_file = "/tmp/ceres_curve_data_delete_me.txt";
  std::ofstream fout(data_file);
  for (int i = 0; i < N; ++i) {
    double y_true = exp(ar * x_data[i] * x_data[i] + br * x_data[i] + cr);
    double y_fit =
        exp(abc[0] * x_data[i] * x_data[i] + abc[1] * x_data[i] + abc[2]);
    fout << x_data[i] << " " << y_data[i] << " " << y_true << " " << y_fit
         << "\n";
  }
  fout.close();

  // 创建 gnuplot 脚本文件
  const std::string plot_script = "/tmp/gnuplot_plot_curve_delete_me.gp";
  std::ofstream gp(plot_script);
  gp << "set xlabel 'x'\n";
  gp << "set ylabel 'y'\n";
  gp << "set title 'Curve Fitting Result'\n";
  gp << "plot '" << data_file
     << "' using 1:2 with points pt 7 ps 0.5 title 'Noisy Data', \\\n"
     << "     '" << data_file
     << "' using 1:3 with lines lw 2 title 'True Curve', \\\n"
     << "     '" << data_file
     << "' using 1:4 with lines lw 2 title 'Fitted Curve'\n";
  gp << "pause -1\n";  // 按任意键关闭
  gp.close();

  // 调用 gnuplot 执行绘图
  std::string cmd = "gnuplot " + plot_script;
  std::system(cmd.c_str());

  // 可选：删除临时文件
  // std::remove(data_file.c_str());
  // std::remove(plot_script.c_str());

  return 0;
}
