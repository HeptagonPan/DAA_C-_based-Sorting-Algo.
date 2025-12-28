
# AI-Driven Sorting Algorithm Optimizer

> **CST207 Design and Analysis of Algorithms - Group Project (2025/09)**

This is an intelligent sorting system built with C++17. It solves the problem that "no single sorting algorithm fits all situations." The system uses an **AI Module** to analyze dataset features (such as size, sortedness, and unique ratio) before sorting. It automatically predicts and selects the best algorithm to optimize performance.

## Project Introduction

In modern software, choosing the right algorithm for different datasets is critical. This library simulates a real-world scenario:

1. **Algorithms** : Includes Bubble Sort, Insertion Sort, Merge Sort, and Quick Sort.
2. **Datasets** : Supports Random, Nearly Sorted, Reversed, and Few Unique value datasets.
3. **Smart Decision** : Integrates  **Decision Tree** ,  **k-NN** , and  **Custom Rules** . The prediction is lightweight ( **$O(N)$** ).
4. **Benchmarking** : Accurately measures execution time (ms) and comparison counts, and verifies if the AI prediction was correct.

## Core Features

* **Linear Efficiency** : Feature extraction (sortedness, unique ratio) is strictly  **$O(N)$** . It uses hash sets to ensure the prediction process has "zero overhead."
* **Robustness & Safety** : Includes defensive checks to prevent crashes on empty or single-element arrays. It detects high duplicates (Unique Ratio) and switches to Merge Sort to avoid Quick Sort's worst-case scenarios.
* **Hardware Awareness** : For tiny datasets ( **$N \le 30$** ), it intelligently switches to simple sorts (Insertion/Bubble) to utilize CPU cache locality and avoid recursion overhead.
* **Full Benchmarking** : Generates a performance table (Time & Comparisons). It automatically skips **$O(N^2)$** algorithms for large datasets ( **$N > 1000$** ) to save time.

## Tech Stack

* **Language** : C++17
* **Dependencies** : Standard Library (STL) - `<vector>`, `<algorithm>`, `<chrono>`, `<unordered_set>`, `<random>`
* **Build Tool** : g++ / clang++ (supporting C++17)

## Quick Start

### 1. Compile the Project

Ensure your compiler supports C++17. Run the following command in your terminal:

**Bash**

```
g++ -std=c++17 main.cpp sorts.cpp ai.cpp -o sort_benchmark
```

*(Windows users please use `sort_benchmark.exe` as the output filename)*

### 2. Run the Program

**Bash**

```
./sort_benchmark
```

### 3. Usage Guide

The program provides an interactive menu:

1. **Select Mode** :

* `Demo datasets`: Automatically runs a set of preset test cases (covering various types and sizes).
* `Custom dataset`: Manually select dataset type and size.

1. **Select AI Mode** :

* `Decision Tree`: Rule-based decision tree (Recommended).
* `k-NN`: Prototype-based k-Nearest Neighbors.
* `Custom Rules`: Tuned hybrid rules.

1. **View Results** :

* The program outputs the AI prediction.
* It runs all algorithms and displays a performance table.
* It verifies if the AI prediction was **Correct** or  **Wrong** .

## AI Module Logic (Decision Tree)

The AI analyzes three features:  **Size (**$N$**)** ,  **Sortedness (**$S$**)** , and  **Unique Ratio (**$U$**)** .

| **Scenario**        | **Condition**      | **Algorithm**          | **Design Reason**                                                                                                                         |
| ------------------------- | ------------------------ | ---------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| **Tiny Data**       | **$N \le 30$**   | **Bubble / Insertion** | **Hardware Awareness** : Avoids recursion overhead and uses cache locality. Uses Bubble for educational baselines or Insertion otherwise. |
| **Small Data**      | **$N \le 50$**   | **Insertion Sort**     | **Low Overhead** : Simple algorithms are usually faster than**$O(N \log N)$**at this scale.                                             |
| **Nearly Sorted**   | **$S \ge 0.90$** | **Insertion Sort**     | **Algorithm Property** : Insertion Sort is nearly**$O(N)$**for sorted data.                                                             |
| **Many Duplicates** | **$U \le 0.20$** | **Merge Sort**         | **Risk Aversion** : Prevents Quick Sort performance degradation caused by unbalanced partitioning.                                        |
| **Highly Reversed** | **$S \le 0.10$** | **Merge Sort**         | **Stability** : Guarantees consistent performance even in worst-case ordering.                                                            |
| **Default**         | Others                   | **Quick Sort**         | **Average Best** : Quick Sort is generally the fastest for large, random datasets.                                                        |

## Project Structure

* `main.cpp`: Entry point. Handles user input, dataset generation, benchmarking flow, and output.
* `ai.cpp` / `ai.h`:  **AI Core** . Contains feature extraction functions (`calculateSortedness`, `calculateUniqueRatio`) and the three prediction modes.
* `sorts.cpp`:  **Sorting Implementation** . Contains Bubble, Insertion, Merge, and Quick Sort with comparison counting and timing.
