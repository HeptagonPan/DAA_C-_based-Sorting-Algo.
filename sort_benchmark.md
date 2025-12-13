# Sorting & Benchmarking Module (C++17)

## 概览
- 包含冒泡、插入、归并、快速排序四种算法，比较次数仅在元素间比较时递增。
- `benchmarkSort` 使用 `std::chrono::high_resolution_clock` 计时，毫秒返回，并校验结果有序性。
- 数据集规模大于 1000 时自动跳过冒泡/插入，仍运行归并与快速。
- 小数组（n <= 20）会打印原始数组；所有算法输出比较次数与耗时，并标注实际最佳（耗时最少，平局看比较次数）。

## 文件
- `main.cpp`：示例驱动，生成多种数据集并打印结果。
- `sorts.cpp`：排序实现、比较计数、计时与有序性校验。

## 编译与运行
```bash
g++ -std=c++17 main.cpp sorts.cpp -o sort_benchmark
./sort_benchmark   # Windows 使用 sort_benchmark.exe
```

## 关键接口（位于 sorts.cpp 顶部）
```cpp
struct SortResult { long long comparisons; double time_ms; };
using SortFunc = std::function<void(std::vector<int>&, long long&)>;
SortResult benchmarkSort(const std::vector<int>& input, SortFunc sorter);
void bubbleSort(std::vector<int>& a, long long& cmp);
void insertionSort(std::vector<int>& a, long long& cmp);
void mergeSort(std::vector<int>& a, long long& cmp);
void quickSort(std::vector<int>& a, long long& cmp);
```

## 行为要点
- 公平性：`benchmarkSort` 会复制输入后排序，避免原地修改影响其他算法。
- 计数规则：仅在 `a[i] <= a[j]`、`a[j] > key` 等元素比较时递增；赋值/交换不计。
- 校验：排序后若未升序，会在标准错误输出提示。
- 输出格式：表格列示算法名、比较次数、耗时（ms），末尾报告“Actual best”。

## 数据集示例（main.cpp）
- 随机（小）
- 近乎有序
- 逆序
- 少唯一值
- 大随机（触发跳过冒泡/插入）

## 下一步可选
1) 扩展更多数据集或自定义规模；2) 增加 AI 预测模块与实际最佳对比；3) 添加单元测试校验计数与有序性。***
