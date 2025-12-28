
# AI-Driven Sorting Algorithm Optimizer

This project is a C++17 application that benchmarks different sorting algorithms. It includes an AI module that analyzes dataset characteristics (size, sortedness, and uniqueness) to predict which sorting algorithm will perform best before running them.

## Overview

The system simulates a smart library that automatically selects the most efficient sorting method. It compares the AI's prediction against the actual execution time and comparison counts of standard algorithms.

### Supported Algorithms

* **Bubble Sort** : Used as a baseline and for educational comparison.
* **Insertion Sort** : Optimized for small or nearly sorted datasets.
* **Merge Sort** : A stable algorithm used for datasets with many duplicates or worst-case ordering.
* **Quick Sort** : The default choice for general large, random datasets.

### Supported Datasets

* **Random** : Completely random integers.
* **Nearly Sorted** : Sorted data with a few random swaps.
* **Reversed** : Data sorted in descending order.
* **Few Unique** : Data containing many duplicate values.
* **Large Random** : Large datasets (N > 1000) where O(N^2) algorithms are skipped.

## Key Features

* **Linear Efficiency** : The AI analyzes data features (Sortedness and Unique Ratio) in strict **O(N)** time. This ensures the prediction process is extremely fast and adds negligible overhead.
* **Safety and Robustness** : The code includes defensive checks for edge cases, such as empty arrays or single-element arrays, preventing division-by-zero errors and crashes.
* **Hardware Awareness** : For very small datasets (N <= 30), the system intelligently switches to simple algorithms (Insertion/Bubble Sort) to avoid the recursion overhead of Quick Sort and Merge Sort.
* **Benchmarking** : Accurately measures execution time (in milliseconds) and the number of comparisons made by each algorithm.

## Requirements

* A C++ compiler that supports C++17 (e.g., GCC, Clang, MSVC).
* Standard Template Library (STL).

## Compilation

To compile the project, run the following command in your terminal:

**Bash**

```
g++ -std=c++17 main.cpp sorts.cpp ai.cpp -o sort_benchmark
```

*Note: Windows users may prefer to output as `sort_benchmark.exe`.*

## Usage

Run the compiled program:

**Bash**

```
./sort_benchmark
```

The program offers an interactive menu with the following steps:

1. **Select Mode** :

* **Demo datasets** : Runs a predefined set of tests covering various scenarios.
* **Custom dataset** : Allows you to specify the data type and size.

1. **Select AI Mode** :

* **Decision Tree** : Uses rule-based logic (Recommended).
* **k-NN** : Uses a prototype-based nearest neighbor classifier.
* **Custom Rules** : Uses tuned hybrid rules.

1. **View Results** :

* The program prints the AI's prediction.
* It runs the algorithms and displays a table of results.
* It compares the prediction with the actual best result to verify accuracy.

## AI Decision Logic

The default AI mode (Decision Tree) uses the following logic to select algorithms:

| **Condition**           | **Selected Algorithm** | **Reason**                                           |
| ----------------------------- | ---------------------------- | ---------------------------------------------------------- |
| **Size <= 30**          | Bubble / Insertion           | Low overhead for tiny arrays (Hardware Awareness).         |
| **Size <= 50**          | Insertion Sort               | Simple sorts are faster than O(N log N) at this scale.     |
| **Sortedness >= 90%**   | Insertion Sort               | Runs in near O(N) time for sorted data.                    |
| **Unique Ratio <= 20%** | Merge Sort                   | Handles duplicates better than Quick Sort (Risk Aversion). |
| **Sortedness <= 10%**   | Merge Sort                   | Consistent performance for reversed data.                  |
| **Default**             | Quick Sort                   | Fastest for general large, random data.                    |

## Project Structure

* **main.cpp** : Entry point. Handles user input, dataset generation, and the benchmarking loop.
* **ai.cpp / ai.h** : Contains the feature extraction logic and the AI prediction models.
* **sorts.cpp** : Contains the implementation of the four sorting algorithms.
