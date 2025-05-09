#include <cstdio>   // for popen, FILE*
#include <cstdlib>  // for exit()
#include <iostream>
#include <vector>

int main() {
  // 模拟一些散点数据
  std::vector<double> x = {1, 2, 3, 4, 5};
  std::vector<double> y = {1.1, 1.9, 3.2, 4.1, 5.0};

  // 打开与 gnuplot 的管道
  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
  if (!gnuplotPipe) {
    std::cerr << "cannot open gnuplot" << std::endl;
    exit(1);
  }

  // 设置绘图命令
  fprintf(gnuplotPipe, "set title 'scatter plot demo'\n");
  fprintf(gnuplotPipe, "set xlabel 'X-axis'\n");
  fprintf(gnuplotPipe, "set ylabel 'Y-axis'\n");
  fprintf(gnuplotPipe, "plot '-' with points pointtype 7 title 'Data'\n");

  // 写入数据
  for (size_t i = 0; i < x.size(); ++i) {
    fprintf(gnuplotPipe, "%f %f\n", x[i], y[i]);
  }

  // 结束数据块
  fprintf(gnuplotPipe, "e\n");

  // 保持窗口直到用户关闭（仅在某些版本的 gnuplot GUI 有效）
  fflush(gnuplotPipe);
  pclose(gnuplotPipe);

  return 0;
}
