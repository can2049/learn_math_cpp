//
// Created by xiang on 18-11-19.
//

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "ceres/ceres.h"

// 代价函数的计算模型
struct CURVE_FITTING_COST {
  CURVE_FITTING_COST(double x, double y) : _x(x), _y(y) {}

  // 残差的计算
  template <typename T>
  bool operator()(const T *const abc,  // 模型参数，有3维
                  T *residual) const {
    residual[0] = T(_y) - ceres::exp(abc[0] * T(_x) * T(_x) + abc[1] * T(_x) +
                                     abc[2]);  // y-exp(ax^2+bx+c)
    return true;
  }

  const double _x, _y;  // x,y数据
};

int main(int argc, char **argv) {
  double ar = 1.0, br = 2.0, cr = 1.0;   // 真实参数值
  double ae = 2.0, be = -1.0, ce = 5.0;  // 估计参数值
  int N = 100;                           // 数据点
  double w_sigma = 1.0;                  // 噪声Sigma值
  double inv_sigma = 1.0 / w_sigma;

  std::random_device rd{};
  std::mt19937 gen(rd());

  std::normal_distribution<> dist{0, w_sigma};

  std::vector<double> x_data, y_data;  // 数据
  for (int i = 0; i < N; i++) {
    double x = i / 100.0;
    x_data.push_back(x);
    y_data.push_back(exp(ar * x * x + br * x + cr) + dist(gen));
  }

  double abc[3] = {ae, be, ce};

  // 构建最小二乘问题
  ceres::Problem problem;
  for (int i = 0; i < N; i++) {
    problem
        .AddResidualBlock(  // 向问题中添加误差项
                            // 使用自动求导，模板参数：误差类型，输出维度，输入维度，维数要与前面struct中一致
            new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 3>(
                new CURVE_FITTING_COST(x_data[i], y_data[i])),
            nullptr,  // 核函数，这里不使用，为空
            abc       // 待估计参数
        );
  }

  // 配置求解器
  ceres::Solver::Options options;  // 这里有很多配置项可以填
  options.linear_solver_type =
      ceres::DENSE_NORMAL_CHOLESKY;             // 增量方程如何求解
  options.minimizer_progress_to_stdout = true;  // 输出到cout

  ceres::Solver::Summary summary;  // 优化信息
  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
  ceres::Solve(options, &problem, &summary);  // 开始优化
  std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_used =
      std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "solve time cost = " << time_used.count() << " seconds. \n";

  // 输出结果
  std::cout << summary.BriefReport() << std::endl;
  std::cout << "estimated a,b,c = ";
  for (auto a : abc) std::cout << a << " ";
  std::cout << std::endl;

  return 0;
}
