#include "ai.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <set>
#include <tuple>

// ==========================================
// AI MODULE IMPLEMENTATION
// Provides three selectable modes:
//  1) Decision Tree (interpretable rules)
//  2) k-NN (small prototype-based classifier)
//  3) Custom Rules (tuned hybrid rules)
// ==========================================

// ------------------------------
// Feature extraction
// ------------------------------

double calculateSortedness(const std::vector<int>& data) {
    if (data.size() < 2) return 1.0;

    const int n = static_cast<int>(data.size());
    int in_order = 0;
    for (int i = 0; i < n - 1; ++i) {
        if (data[i] <= data[i + 1]) {
            ++in_order;
        }
    }
    return static_cast<double>(in_order) / static_cast<double>(n - 1);
}

double calculateUniqueRatio(const std::vector<int>& data) {
    if (data.empty()) return 0.0;
    std::set<int> uniq(data.begin(), data.end());
    return static_cast<double>(uniq.size()) / static_cast<double>(data.size());
}

std::string aiModeName(AIMode mode) {
    switch (mode) {
        case AIMode::DecisionTree:
            return "Decision Tree";
        case AIMode::KNN:
            return "k-NN";
        case AIMode::CustomRules:
            return "Custom Rules";
        default:
            return "Decision Tree";
    }
}

// ------------------------------
// Mode 1: Decision Tree (interpretable)
// Uses: n, sortedness, unique_ratio
// ------------------------------
static std::string predictDecisionTree(const std::vector<int>& data) {
    const int n = static_cast<int>(data.size());
    const double sortedness = calculateSortedness(data);
    const double uniq = calculateUniqueRatio(data);

    // Rule A0: Tiny and not-nearly-sorted -> Bubble as educational baseline
    // (We only allow Bubble on very small inputs to satisfy 4-choice output and keep accuracy stable.)
    if (n <= 30) {
        if (sortedness < 0.80) return "Bubble Sort";
        return "Insertion Sort";
    }

    // Rule A: Very small arrays -> Insertion (low overhead)
    if (n <= 50) return "Insertion Sort";

    // Rule B: Nearly sorted -> Insertion
    if (sortedness >= 0.90) return "Insertion Sort";

    // Rule C: Many duplicates -> Merge is stable and consistent
    // (partition-based quick sort can be sensitive if many equal keys)
    if (uniq <= 0.20 && n >= 1000) return "Merge Sort";

    // Rule D: Highly unsorted / reversed-like -> prefer stable O(n log n)
    if (sortedness <= 0.10) return "Merge Sort";

    // Default: Quick sort for general large random-like data
    return "Quick Sort";
}

// ------------------------------
// Mode 2: k-NN (prototype-based)
// A small set of hand-crafted "training" prototypes.
// Features are normalized to similar ranges.
// ------------------------------

struct Prototype {
    double f_size;       // normalized log size
    double f_sortedness; // 0..1
    double f_unique;     // 0..1
    const char* label;   // algorithm name
};

static double normLogSize(int n) {
    // Map n roughly into 0..1 using log10.
    // For n in [1, 100000], log10(n) in [0, 5].
    if (n <= 1) return 0.0;
    double v = std::log10(static_cast<double>(n));
    return std::min(1.0, std::max(0.0, v / 5.0));
}

static std::string predictKNN(const std::vector<int>& data) {
    const int n = static_cast<int>(data.size());
    const double s = calculateSortedness(data);
    const double u = calculateUniqueRatio(data);
    const double x = normLogSize(n);

    // Small prototype set (enough for a coursework demo).
    // (size, sortedness, unique) -> label
    static const std::vector<Prototype> protos = {
        // small
        // tiny unsorted baseline
        {normLogSize(20), 0.30, 0.90, "Bubble Sort"},
        {normLogSize(30), 0.20, 0.90, "Bubble Sort"},
        {normLogSize(20), 0.50, 0.90, "Insertion Sort"},
        {normLogSize(50), 0.95, 0.90, "Insertion Sort"},
        // nearly sorted
        {normLogSize(500), 0.95, 0.80, "Insertion Sort"},
        {normLogSize(5000), 0.92, 0.80, "Insertion Sort"},
        // duplicates
        {normLogSize(2000), 0.50, 0.10, "Merge Sort"},
        {normLogSize(20000), 0.50, 0.10, "Merge Sort"},
        // random large
        {normLogSize(5000), 0.55, 0.95, "Quick Sort"},
        {normLogSize(50000), 0.55, 0.95, "Quick Sort"},
        // reversed-like
        {normLogSize(5000), 0.05, 0.90, "Merge Sort"},
    };

    // Compute distances and take k=3 nearest neighbors.
    const int k = 3;
    std::vector<std::pair<double, const char*>> dist;
    dist.reserve(protos.size());
    for (const auto& p : protos) {
        const double dx = x - p.f_size;
        const double ds = s - p.f_sortedness;
        const double du = u - p.f_unique;
        const double d2 = dx * dx + ds * ds + du * du;
        dist.push_back({d2, p.label});
    }
    std::nth_element(dist.begin(), dist.begin() + k, dist.end(),
                     [](auto& a, auto& b) { return a.first < b.first; });

    // Majority vote among k nearest.
    int countBub = 0, countIns = 0, countMer = 0, countQui = 0;
    for (int i = 0; i < k; ++i) {
        const std::string label = dist[i].second;
        if (label == "Bubble Sort") ++countBub;
        else if (label == "Insertion Sort") ++countIns;
        else if (label == "Merge Sort") ++countMer;
        else if (label == "Quick Sort") ++countQui;
    }
    if (countIns >= countBub && countIns >= countMer && countIns >= countQui) return "Insertion Sort";
    if (countMer >= countBub && countMer >= countIns && countMer >= countQui) return "Merge Sort";
    if (countQui >= countBub && countQui >= countIns && countQui >= countMer) return "Quick Sort";
    return "Bubble Sort";
}

// ------------------------------
// Mode 3: Custom Rules (tuned)
// This is the place to reflect your "final" rules,
// e.g., include unique_ratio, a Merge rule, and a tiny-input Bubble baseline.
// ------------------------------
static std::string predictCustomRules(const std::vector<int>& data) {
    const int n = static_cast<int>(data.size());
    const double sortedness = calculateSortedness(data);
    const double uniq = calculateUniqueRatio(data);

    // Tiny and not-nearly-sorted -> Bubble as educational baseline
    if (n <= 30 && sortedness < 0.80) return "Bubble Sort";

    // Tuned thresholds (you can mention these in the report)
    const int smallN = 60;
    const double sortedThresh = 0.88;
    const double dupThresh = 0.15;

    // 1) Small inputs
    if (n <= smallN) return "Insertion Sort";

    // 2) Highly sorted
    if (sortedness >= sortedThresh) return "Insertion Sort";

    // 3) Many duplicates -> Merge
    if (uniq <= dupThresh) return "Merge Sort";

    // 4) Otherwise choose Quick
    return "Quick Sort";
}

// ------------------------------
// Public API
// ------------------------------

std::string predictBestAlgorithm(const std::vector<int>& data, AIMode mode) {
    switch (mode) {
        case AIMode::DecisionTree:
            return predictDecisionTree(data);
        case AIMode::KNN:
            return predictKNN(data);
        case AIMode::CustomRules:
            return predictCustomRules(data);
        default:
            return predictDecisionTree(data);
    }
}

std::string predictBestAlgorithm(const std::vector<int>& data) {
    return predictBestAlgorithm(data, AIMode::DecisionTree);
}

