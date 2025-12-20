# DAA_C-_based-Sorting-Algo.
Our group project of 2025/09 sem Design and Analysis of Algorithm.
# 说明

本项目的主要文件如下：

- `main.cpp`：程序入口与交互逻辑，负责选择数据集类型与规模、调用基准测试并输出结果表格。
- `sorts.cpp`：包含四种排序算法（冒泡、插入、归并、快速）、比较次数统计、计时封装与排序正确性校验。
- `sort_benchmark.md`：使用说明与模块概览，包含编译、运行与输出行为说明。

编译示例：
```bash
g++ -std=c++17 main.cpp sorts.cpp -o sort_benchmark
```
