#include "ai.h"
#include <vector>
#include <string>
#include <algorithm> // for max
#include <cmath>     // for abs

// ==========================================
// AI MODULE IMPLEMENTATION
// Strategy: Rule-based Decision Tree
// ==========================================

// Helper: Calculate "Sortedness" Feature
// Checks what percentage of the array is already sorted.
// Returns: 0.0 (random) to 1.0 (perfectly sorted)
double calculateSortedness(const std::vector<int>& data) {
    if (data.empty()) return 0.0;
    
    int n = data.size();
    int correct_order_count = 0;

    // Loop through the array and count sorted pairs
    for (int i = 0; i < n - 1; ++i) {
        if (data[i] <= data[i + 1]) {
            correct_order_count++;
        }
    }

    // Return the ratio (count / total pairs)
    return (double)correct_order_count / (n - 1);
}

// Main AI Prediction Function
// Uses features (Size, Sortedness) to choose the best algorithm.
std::string predictBestAlgorithm(const std::vector<int>& data) {
    int n = data.size();
    
    // Step 1: Extract features
    double sortedness = calculateSortedness(data);

    // Step 2: Decision Tree Logic (Rules)

    // Rule 1: Very small dataset?
    // Insertion Sort is fastest for small arrays because of low overhead.
    if (n <= 50) {
        return "Insertion Sort";
    }

    // Rule 2: Is the data nearly sorted?
    // If sortedness is high (e.g., > 85%), Insertion Sort is O(N).
    // It is faster than Quick Sort (O(N log N)) in this case.
    if (sortedness > 0.85) {
        return "Insertion Sort";
    }

    // Rule 3: Default choice for large, random data
    // Quick Sort is generally the best for random, large arrays.
    // (Note: Your Quick Sort implementation is robust).
    return "Quick Sort";
}