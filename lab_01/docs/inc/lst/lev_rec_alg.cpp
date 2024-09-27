template<typename T>
typename T::size_type LevenshteinDistanceRecursive(const T &source, const T &target, int l1, int l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    int flag = (source[l1 - 1] != target[l2 - 1]);
    return std::min(std::min(LevenshteinDistanceRecursive(source, target, l1, l2 - 1) + 1, LevenshteinDistanceRecursive(source, target, l1 - 1, l2) + 1), LevenshteinDistanceRecursive(source, target, l1 - 1, l2 - 1) + flag);
}
