#include "ai.h"

#include <unordered_set>
#include <vector>
#include <string>

// ==========================================
// AI MODULE IMPLEMENTATION
// Strategy: Rule-based Decision Tree
// Features: size, sortedness, unique_ratio
// ==========================================

// Sortedness: proportion of adjacent pairs already in non-decreasing order
double calculateSortedness(const std::vector<int>& data) {
    const size_t n = data.size();
    if (n < 2) return 1.0; // empty or 1-element is "fully sorted"

    long long inOrder = 0;
    for (size_t i = 0; i + 1 < n; ++i) {
        if (data[i] <= data[i + 1]) inOrder++;
    }
    return static_cast<double>(inOrder) / static_cast<double>(n - 1);
}

// Unique ratio: (#unique values) / n
double calculateUniqueRatio(const std::vector<int>& data) {
    const size_t n = data.size();
    if (n == 0) return 0.0;

    std::unordered_set<int> s;
    s.reserve(n);
    for (int x : data) s.insert(x);

    return static_cast<double>(s.size()) / static_cast<double>(n);
}

std::string predictBestAlgorithm(const std::vector<int>& data) {
    const size_t n = data.size();
    const double sortedness  = calculateSortedness(data);
    const double uniqueRatio = calculateUniqueRatio(data);

    // --------------------------
    // Decision Tree Rules
    // --------------------------

    // Rule 0: tiny arrays — simple sorts are fine
    if (n <= 30) {
        // if already quite sorted, insertion is usually best
        if (sortedness > 0.80) return "Insertion Sort";
        return "Bubble Sort";
    }

    // Rule 1: nearly sorted — insertion is great (close to O(n))
    if (sortedness > 0.85) {
        return "Insertion Sort";
    }

    // Rule 2: many duplicates — prefer Merge Sort (more stable performance)
    // (few-unique dataset often makes partitioning less ideal for quick sort)
    if (uniqueRatio < 0.25) {
        return "Merge Sort";
    }

    // Rule 3: default — large/random
    return "Quick Sort";
}
