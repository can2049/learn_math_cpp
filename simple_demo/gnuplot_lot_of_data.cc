#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>   // 示例用 sin()

int main() {
    // 生成大量数据（例如 100000 个点）
    const int N = 100000;
    std::vector<double> x(N), y(N);
    for (int i = 0; i < N; ++i) {
        x[i] = i * 0.001;
        y[i] = std::sin(x[i]);
    }

    // 启动 Gnuplot
    FILE* gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe) {
        std::cerr << "无法启动 Gnuplot。" << std::endl;
        return 1;
    }

    // 设置图像样式
    fprintf(gnuplotPipe, "set title 'lot of data'\n");
    fprintf(gnuplotPipe, "set xlabel 'X'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y'\n");
    fprintf(gnuplotPipe, "plot '-' with lines title 'sin(x)'\n");

    // 发送数据（逐行）
    for (int i = 0; i < N; ++i) {
        fprintf(gnuplotPipe, "%f %f\n", x[i], y[i]);
    }
    fprintf(gnuplotPipe, "e\n");

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    return 0;
}
