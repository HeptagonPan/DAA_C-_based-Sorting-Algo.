# results.csv schema
Columns:
- run_id (string)  e.g. 2025-12-21T13-00-00
- dataset_type (string) Random / NearlySorted / Reversed / FewUnique / LargeRandom
- n (int)
- algorithm (string) Bubble / Insertion / Merge / Quick
- comparisons (int)
- time_ms (float)

# predictions.csv schema
Columns:
- run_id
- dataset_type
- n
- ai_module (DecisionTree / kNN / Custom)
- predicted_best (string)
- actual_best (string)
- correct (0/1)