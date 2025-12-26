#ifndef AI_H
#define AI_H

#include <string>
#include <vector>

// Feature 1: sortedness in [0, 1]
double calculateSortedness(const std::vector<int>& data);

// Feature 2: unique ratio in [0, 1]
double calculateUniqueRatio(const std::vector<int>& data);

// Return exactly one of:
// "Bubble Sort", "Insertion Sort", "Merge Sort", "Quick Sort"
std::string predictBestAlgorithm(const std::vector<int>& data);

#endif
