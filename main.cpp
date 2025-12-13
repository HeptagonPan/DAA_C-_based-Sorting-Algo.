#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
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

    std::vector<std::pair<std::string, SortFunc>> algorithms;
    if (!skipSimple) {
        algorithms.push_back({"Bubble Sort", bubbleSort});
        algorithms.push_back({"Insertion Sort", insertionSort});
    }
    algorithms.push_back({"Merge Sort", mergeSort});
    algorithms.push_back({"Quick Sort", quickSort});

    std::cout << std::left << std::setw(15) << "Algorithm" << std::setw(15)
              << "Comparisons"
              << "Time (ms)\n";
    std::cout << std::string(15 + 15 + 12, '-') << "\n";

    struct NamedResult {
        std::string name;
        SortResult result;
    };
    std::vector<NamedResult> results;
    results.reserve(algorithms.size());

    for (const auto& algo : algorithms) {
        SortResult res = benchmarkSort(data, algo.second);
        results.push_back({algo.first, res});
        std::cout << std::left << std::setw(15) << algo.first << std::setw(15)
                  << res.comparisons << std::fixed << std::setprecision(3)
                  << res.time_ms << "\n";
    }

    if (!results.empty()) {
        NamedResult best = results.front();
        for (const auto& r : results) {
            const bool faster = r.result.time_ms < best.result.time_ms;
            const bool tie =
                r.result.time_ms == best.result.time_ms &&
                r.result.comparisons < best.result.comparisons;
            if (faster || tie) {
                best = r;
            }
        }
        std::cout << "Actual best: " << best.name << " (time="
                  << std::fixed << std::setprecision(3) << best.result.time_ms
                  << " ms, comparisons=" << best.result.comparisons << ")\n";
    }
}

int main() {
    std::mt19937 rng(42);  // Fixed seed for reproducibility across runs.

    std::vector<Dataset> datasets = {
        {"Random", generateRandomData(15, -50, 50, rng)},
        {"Nearly Sorted", generateNearlySortedData(20, rng)},
        {"Reversed", generateReversedData(25)},
        {"Few Unique", generateFewUniqueData(200, rng)},
        {"Large Random", generateRandomData(5000, 0, 100000, rng)},
    };

    for (const auto& ds : datasets) {
        runBenchmarks(ds);
    }

    return 0;
}
