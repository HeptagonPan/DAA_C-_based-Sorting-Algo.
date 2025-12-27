#ifndef AI_H
#define AI_H

#include <vector>
#include <string>

// ==========================================
// AI MODULE HEADER
// ==========================================

// ------------------------------
// Feature extraction
// ------------------------------
// Sortedness: percentage of adjacent pairs already in non-decreasing order.
// Range: 0.0 (very unsorted) to 1.0 (perfectly sorted)
double calculateSortedness(const std::vector<int>& data);

// Unique ratio: unique_count / n
// Range: 0.0 to 1.0
double calculateUniqueRatio(const std::vector<int>& data);

// ------------------------------
// AI selection modes
// ------------------------------
enum class AIMode {
    DecisionTree = 1,
    KNN = 2,
    CustomRules = 3,
};

// Human-readable name for printing
std::string aiModeName(AIMode mode);

// Predict the best sorting algorithm under a chosen AI mode.
// Output must match the names printed in the benchmark table.
std::string predictBestAlgorithm(const std::vector<int>& data, AIMode mode);

// Backward compatible default (Decision Tree)
std::string predictBestAlgorithm(const std::vector<int>& data);

#endif
