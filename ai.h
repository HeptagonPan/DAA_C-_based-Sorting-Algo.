#ifndef AI_H
#define AI_H

#include <vector>
#include <string>

// ==========================================
// AI MODULE HEADER
// ==========================================

// Calculate how many elements are already in order (0.0 to 1.0)
double calculateSortedness(const std::vector<int>& data);

// Main function: Predict the best sorting algorithm
// Input: The dataset array
// Output: The name of the algorithm (e.g., "Quick Sort")
std::string predictBestAlgorithm(const std::vector<int>& data);

#endif