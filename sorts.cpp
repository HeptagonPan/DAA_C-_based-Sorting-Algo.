#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <vector>

// Shared structs and aliases (moved from header to keep only .cpp files).
struct SortResult {
    long long comparisons{0};
    double time_ms{0.0};
};

using SortFunc = std::function<void(std::vector<int>&, long long&)>;

bool isSorted(const std::vector<int>& data);
void bubbleSort(std::vector<int>& a, long long& comparisons);
void insertionSort(std::vector<int>& a, long long& comparisons);
void mergeSort(std::vector<int>& a, long long& comparisons);
void quickSort(std::vector<int>& a, long long& comparisons);
SortResult benchmarkSort(const std::vector<int>& input, SortFunc sorter);

namespace {
using Clock = std::chrono::high_resolution_clock;

// Merge helper used by mergeSort.
void mergeSections(std::vector<int>& a,
                   std::vector<int>& buffer,
                   std::size_t left,
                   std::size_t mid,
                   std::size_t right,
                   long long& comparisons) {
    std::size_t i = left;
    std::size_t j = mid + 1;
    std::size_t k = left;

    while (i <= mid && j <= right) {
        ++comparisons;  // Comparing two array values (a[i] vs a[j]).
        if (a[i] <= a[j]) {
            buffer[k++] = a[i++];
        } else {
            buffer[k++] = a[j++];
        }
    }

    while (i <= mid) {
        buffer[k++] = a[i++];
    }
    while (j <= right) {
        buffer[k++] = a[j++];
    }

    for (std::size_t idx = left; idx <= right; ++idx) {
        a[idx] = buffer[idx];
    }
}

void mergeSortImpl(std::vector<int>& a,
                   std::vector<int>& buffer,
                   std::size_t left,
                   std::size_t right,
                   long long& comparisons) {
    if (left >= right) {
        return;
    }
    std::size_t mid = left + (right - left) / 2;
    mergeSortImpl(a, buffer, left, mid, comparisons);
    mergeSortImpl(a, buffer, mid + 1, right, comparisons);
    mergeSections(a, buffer, left, mid, right, comparisons);
}

std::size_t partition(std::vector<int>& a,
                      std::size_t low,
                      std::size_t high,
                      long long& comparisons) {
    int pivot = a[high];
    std::ptrdiff_t i = static_cast<std::ptrdiff_t>(low) - 1;

    for (std::size_t j = low; j < high; ++j) {
        ++comparisons;  // Comparing array value with pivot.
        if (a[j] <= pivot) {
            ++i;
            std::swap(a[static_cast<std::size_t>(i)], a[j]);
        }
    }

    std::swap(a[static_cast<std::size_t>(i + 1)], a[high]);
    return static_cast<std::size_t>(i + 1);
}

void quickSortImpl(std::vector<int>& a,
                   std::size_t low,
                   std::size_t high,
                   long long& comparisons) {
    if (low >= high) {
        return;
    }

    std::size_t pivotIndex = partition(a, low, high, comparisons);
    if (pivotIndex > 0) {
        quickSortImpl(a, low, pivotIndex - 1, comparisons);
    }
    if (pivotIndex + 1 <= high) {
        quickSortImpl(a, pivotIndex + 1, high, comparisons);
    }
}
}  // namespace

SortResult benchmarkSort(const std::vector<int>& input, SortFunc sorter) {
    std::vector<int> data = input;  // Work on a copy for fairness.
    long long comparisons = 0;

    auto start = Clock::now();
    sorter(data, comparisons);
    auto end = Clock::now();

    double time_ms =
        std::chrono::duration<double, std::milli>(end - start).count();

    if (!isSorted(data)) {
        std::cerr << "[Error] Sorting failed: output is not in ascending order.\n";
    }

    return SortResult{comparisons, time_ms};
}

void bubbleSort(std::vector<int>& a, long long& comparisons) {
    const std::size_t n = a.size();
    if (n < 2) {
        return;
    }

    for (std::size_t i = 0; i + 1 < n; ++i) {
        bool swapped = false;
        for (std::size_t j = 0; j + 1 < n - i; ++j) {
            ++comparisons;  // Comparing adjacent values.
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;  // Array already sorted.
        }
    }
}

void insertionSort(std::vector<int>& a, long long& comparisons) {
    const std::size_t n = a.size();
    for (std::size_t i = 1; i < n; ++i) {
        int key = a[i];
        std::ptrdiff_t j = static_cast<std::ptrdiff_t>(i) - 1;
        while (j >= 0) {
            ++comparisons;  // Comparing a[j] with key.
            if (a[static_cast<std::size_t>(j)] > key) {
                a[static_cast<std::size_t>(j) + 1] = a[static_cast<std::size_t>(j)];
                --j;
            } else {
                break;
            }
        }
        a[static_cast<std::size_t>(j) + 1] = key;
    }
}

void mergeSort(std::vector<int>& a, long long& comparisons) {
    if (a.size() < 2) {
        return;
    }
    std::vector<int> buffer(a.size());
    mergeSortImpl(a, buffer, 0, a.size() - 1, comparisons);
}

void quickSort(std::vector<int>& a, long long& comparisons) {
    if (a.size() < 2) {
        return;
    }
    quickSortImpl(a, 0, a.size() - 1, comparisons);
}

bool isSorted(const std::vector<int>& data) {
    for (std::size_t i = 1; i < data.size(); ++i) {
        if (data[i - 1] > data[i]) {
            return false;
        }
    }
    return true;
}
