#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>

// Forward declarations (no header file, per requirements).
struct SortResult {
    long long comparisons{0};
    double time_ms{0.0};
};
using SortFunc = std::function<void(std::vector<int>&, long long&)>;
SortResult benchmarkSort(const std::vector<int>& input, SortFunc sorter);
void bubbleSort(std::vector<int>& a, long long& comparisons);
void insertionSort(std::vector<int>& a, long long& comparisons);
void mergeSort(std::vector<int>& a, long long& comparisons);
void quickSort(std::vector<int>& a, long long& comparisons);

struct Dataset {
    std::string name;
    std::vector<int> values;
};

int readIntInRange(const std::string& prompt, int minValue, int maxValue) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue && value <= maxValue) {
            return value;
        }
        std::cout << "Invalid input. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::size_t readSizeAtLeast(const std::string& prompt, std::size_t minValue) {
    long long value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= static_cast<long long>(minValue)) {
            return static_cast<std::size_t>(value);
        }
        std::cout << "Invalid input. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::vector<int> generateRandomData(std::size_t n,
                                    int minVal,
                                    int maxVal,
                                    std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(minVal, maxVal);
    std::vector<int> data;
    data.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        data.push_back(dist(rng));
    }
    return data;
}

std::vector<int> generateNearlySortedData(std::size_t n, std::mt19937& rng) {
    if (n < 2) {
        return std::vector<int>(n);
    }
    std::vector<int> data(n);
    std::iota(data.begin(), data.end(), 1);

    // Apply a few random swaps to make it "nearly" sorted.
    const std::size_t swaps = std::max<std::size_t>(1, n / 10);
    std::uniform_int_distribution<std::size_t> dist(0, n - 1);
    for (std::size_t i = 0; i < swaps; ++i) {
        std::size_t a = dist(rng);
        std::size_t b = dist(rng);
        std::swap(data[a], data[b]);
    }
    return data;
}

std::vector<int> generateReversedData(std::size_t n) {
    std::vector<int> data(n);
    for (std::size_t i = 0; i < n; ++i) {
        data[i] = static_cast<int>(n - i);
    }
    return data;
}

std::vector<int> generateFewUniqueData(std::size_t n, std::mt19937& rng) {
    const std::vector<int> pool = {-5, -1, 0, 3, 7, 12};
    std::uniform_int_distribution<std::size_t> pick(0, pool.size() - 1);
    std::vector<int> data;
    data.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        data.push_back(pool[pick(rng)]);
    }
    return data;
}

Dataset buildDataset(int choice, std::size_t size, std::mt19937& rng) {
    switch (choice) {
        case 1:
            return {"Random", generateRandomData(size, -50, 50, rng)};
        case 2:
            return {"Nearly Sorted", generateNearlySortedData(size, rng)};
        case 3:
            return {"Reversed", generateReversedData(size)};
        case 4:
            return {"Few Unique", generateFewUniqueData(size, rng)};
        case 5:
            return {"Large Random", generateRandomData(size, 0, 100000, rng)};
        default:
            return {"Random", generateRandomData(size, -50, 50, rng)};
    }
}

std::vector<Dataset> buildDemoDatasets(std::mt19937& rng) {
    return {
        {"Random", generateRandomData(15, -50, 50, rng)},
        {"Nearly Sorted", generateNearlySortedData(20, rng)},
        {"Reversed", generateReversedData(25)},
        {"Few Unique", generateFewUniqueData(200, rng)},
        {"Large Random", generateRandomData(5000, 0, 100000, rng)},
    };
}

void printArray(const std::vector<int>& data) {
    std::cout << "[";
    for (std::size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if (i + 1 < data.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

void runBenchmarks(const Dataset& dataset) {
    const auto& data = dataset.values;
    const std::size_t n = data.size();

    std::cout << "\n=== Dataset: " << dataset.name << " (n=" << n << ") ===\n";
    if (n <= 20) {
        std::cout << "Original: ";
        printArray(data);
    }

    const bool skipSimple = n > 1000;
    if (skipSimple) {
        std::cout << "Skipping Bubble Sort and Insertion Sort because n > 1000.\n";
    }

    struct AlgorithmEntry {
        std::string name;
        SortFunc func;
        bool skipWhenLarge;
    };
    const std::vector<AlgorithmEntry> algorithms = {
        {"Bubble Sort", bubbleSort, true},
        {"Insertion Sort", insertionSort, true},
        {"Merge Sort", mergeSort, false},
        {"Quick Sort", quickSort, false},
    };

    struct NamedResult {
        std::string name;
        bool skipped{false};
        SortResult result{};
    };
    std::vector<NamedResult> results;
    results.reserve(algorithms.size());

    for (const auto& algo : algorithms) {
        if (skipSimple && algo.skipWhenLarge) {
            results.push_back({algo.name, true, {}});
            continue;
        }
        SortResult res = benchmarkSort(data, algo.func);
        results.push_back({algo.name, false, res});
    }

    std::size_t nameWidth = std::string("Algorithm").size();
    for (const auto& r : results) {
        nameWidth = std::max(nameWidth, r.name.size());
    }
    nameWidth += 2;

    const std::size_t compWidth =
        std::max<std::size_t>(12, std::string("Comparisons").size() + 2);
    const std::size_t timeWidth =
        std::max<std::size_t>(12, std::string("Time (ms)").size() + 2);

    std::cout << std::left << std::setw(static_cast<int>(nameWidth))
              << "Algorithm" << std::setw(static_cast<int>(compWidth))
              << "Comparisons" << std::setw(static_cast<int>(timeWidth))
              << "Time (ms)" << "\n";
    std::cout << std::string(nameWidth + compWidth + timeWidth, '-') << "\n";

    for (const auto& r : results) {
        std::cout << std::left << std::setw(static_cast<int>(nameWidth))
                  << r.name;
        if (r.skipped) {
            std::cout << std::setw(static_cast<int>(compWidth)) << "skipped"
                      << std::setw(static_cast<int>(timeWidth)) << "skipped"
                      << "\n";
        } else {
            std::cout << std::setw(static_cast<int>(compWidth))
                      << r.result.comparisons << std::fixed
                      << std::setprecision(3)
                      << std::setw(static_cast<int>(timeWidth))
                      << r.result.time_ms << "\n";
        }
    }

    if (!results.empty()) {
        const double epsilon = 1e-6;
        bool bestSet = false;
        NamedResult best;
        for (const auto& r : results) {
            if (r.skipped) {
                continue;
            }
            if (!bestSet) {
                best = r;
                bestSet = true;
                continue;
            }
            const bool faster =
                r.result.time_ms + epsilon < best.result.time_ms;
            const bool tie =
                std::abs(r.result.time_ms - best.result.time_ms) <= epsilon &&
                r.result.comparisons < best.result.comparisons;
            if (faster || tie) {
                best = r;
            }
        }
        if (bestSet) {
            std::cout << "Actual best: " << best.name << " (time="
                      << std::fixed << std::setprecision(3)
                      << best.result.time_ms
                      << " ms, comparisons=" << best.result.comparisons
                      << ")\n";
        }
    }
}

int main() {
    std::mt19937 rng(42);  // Fixed seed for reproducibility across runs.

    std::cout << "Sorting Benchmark\n";
    std::cout << "1) Demo datasets\n";
    std::cout << "2) Custom dataset\n";
    int mode = readIntInRange("Select mode (1-2): ", 1, 2);

    std::vector<Dataset> datasets;
    if (mode == 1) {
        datasets = buildDemoDatasets(rng);
    } else {
        std::cout << "Dataset types:\n";
        std::cout << "1) Random\n";
        std::cout << "2) Nearly Sorted\n";
        std::cout << "3) Reversed\n";
        std::cout << "4) Few Unique\n";
        std::cout << "5) Large Random (n > 1000)\n";
        int choice = readIntInRange("Select dataset type (1-5): ", 1, 5);
        std::size_t minSize = (choice == 5) ? 1001 : 1;
        std::string prompt =
            (choice == 5) ? "Enter dataset size (> 1000): "
                          : "Enter dataset size: ";
        std::size_t size = readSizeAtLeast(prompt, minSize);
        datasets.push_back(buildDataset(choice, size, rng));
    }

    for (const auto& ds : datasets) {
        runBenchmarks(ds);
    }

    return 0;
}
