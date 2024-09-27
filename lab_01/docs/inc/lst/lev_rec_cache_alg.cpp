template<typename T>
typename T::size_type LevenshteinDistanceRecursiveCache(const T &source, const T &target, int l1, int l2, std::vector<std::vector<int>>& cache) noexcept {
    if (l1 == 0) return l2;
    if (l2 == 0) return l1;

    if (cache[l1][l2] != INT_MAX) return cache[l1][l2];

    int flag = (source[l1 - 1] != target[l2 - 1]);
    
    int op1 = LevenshteinDistanceRecursiveCache(source, target, l1, l2 - 1, cache) + 1;
    int op2 = LevenshteinDistanceRecursiveCache(source, target, l1 - 1, l2, cache) + 1;
    int op3 = LevenshteinDistanceRecursiveCache(source, target, l1 - 1, l2 - 1, cache) + flag;

    cache[l1][l2] = std::min({op1, op2, op3});
    return cache[l1][l2];
}