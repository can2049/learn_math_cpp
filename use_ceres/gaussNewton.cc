#include <Eigen/Core>
#include <Eigen/Dense>
#include <chrono>
#include <cmath>
#include <cstdlib>  // for std::system
#include <fstream>
#include <iomanip>  // for std::setprecision, std::scientific
#include <iostream>
#include <limits>  // for std::numeric_limits
#include <random>
#include <vector>

int main(int argc, char **argv) {
  double ar = 1.0, br = 2.0, cr = 1.0;   // 真实参数值
  double ae = 5.0, be = -5.0, ce = 5.0;  // 估计参数值
  int N = 200;                           // 数据点
  double w_sigma = 1.5;                  // 噪声Sigma值
  double inv_sigma = 1.0 / w_sigma;

  std::mt19937 gen(std::random_device{}());
  std::normal_distribution<> dist(0, w_sigma);

  std::vector<double> x_data, y_data, y_true_data;  // 数据
  for (int i = 0; i < N; i++) {
    double x = 1.0 * i / N;
    x_data.push_back(x);
    auto y = exp(ar * x * x + br * x + cr);  // 真实值
    y_true_data.push_back(y);
    y_data.push_back(y + dist(gen));  // 添加噪声
  }

  // 开始Gauss-Newton迭代
  int iterations = 100;           // 迭代次数
  double cost = 0, lastCost = 0;  // 本次迭代的cost和上一次迭代的cost

  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
  for (int iter = 0; iter < iterations; iter++) {
    // Hessian = J^T W^{-1} J in Gauss-Newton
    Eigen::Matrix3d H = Eigen::Matrix3d::Zero();
    Eigen::Vector3d b = Eigen::Vector3d::Zero();  // bias
    cost = 0;

    for (int i = 0; i < N; i++) {
      double xi = x_data[i], yi = y_data[i];  // 第i个数据点
      double error = yi - exp(ae * xi * xi + be * xi + ce);
      Eigen::Vector3d J;                                   // 雅可比矩阵
      J[0] = -xi * xi * exp(ae * xi * xi + be * xi + ce);  // de/da
      J[1] = -xi * exp(ae * xi * xi + be * xi + ce);       // de/db
      J[2] = -exp(ae * xi * xi + be * xi + ce);            // de/dc

      H += inv_sigma * inv_sigma * J * J.transpose();
      b += -inv_sigma * inv_sigma * error * J;

      cost += error * error;
    }

    // 求解线性方程 Hx=b
    Eigen::Vector3d dx = H.ldlt().solve(b);
    if (std::isnan(dx[0])) {
      std::cout << "result is nan!" << std::endl;
      break;
    }

    if (iter > 0 && cost >= lastCost) {
      std::cout << "cost: " << cost << ">= last cost: " << lastCost
                << ", break." << std::endl;
      break;
    }

    ae += dx[0];
    be += dx[1];
    ce += dx[2];

    lastCost = cost;

    std::cout << "total cost: " << cost << ", \t\tupdate: " << dx.transpose()
              << "\t\testimated params: " << ae << "," << be << "," << ce
              << std::endl;
  }

  std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_used =
      std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "solve time cost = " << time_used.count() << " seconds. "
            << std::endl;

  std::cout << "estimated abc = " << ae << ", " << be << ", " << ce
            << std::endl;

  // print data to file
  std::string data_file = "/tmp/gauss_newton_data_delete_me.txt";

  std::ofstream fout(data_file);

  for (int i = 0; i < N; i++) {
    // x, y, y_true, y_fit
    double y_fit = exp(ae * x_data[i] * x_data[i] + be * x_data[i] + ce);
    fout << std::setprecision(10) << x_data[i] << " " << y_data[i] << " "
         << y_true_data[i] << " " << y_fit << std::endl;
  }
  fout.close();

  // use gnuplot to plot
  std::string plot_script = "/tmp/gnuplot_plot_curve_delete_me.gp";
  std::ofstream gp(plot_script);
  gp << "set xlabel 'x'\n";
  gp << "set ylabel 'y'\n";
  gp << "set title 'Gauss-Newton Curve Fitting'\n";
  gp << "plot '" << data_file
     << "' using 1:2 title 'data' with points pt 7 ps 0.5, "
        "'"
     << data_file << "' using 1:3 title 'true' with lines lw 2, '" << data_file
     << "' using 1:4 title 'fit' with lines lw 2\n";
  gp << "pause -1\n";  // 按任意键关闭
  gp.close();
  std::string cmd = "gnuplot " + plot_script;
  std::system(cmd.c_str());
  std::cout << "Data file: " << data_file << std::endl;
  std::cout << "Gnuplot script: " << plot_script << std::endl;
  // 可选：删除临时文件
  // std::remove(data_file.c_str());
  // std::remove(plot_script.c_str());

  return 0;
}
