#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits.h>

template<typename T>
void InitLDRCache(std::vector<std::vector<T>> &cache, int l1, int l2) {
    cache.resize(l1);
    for (int i = 0; i < l1; ++i) {
        cache[i].resize(l2);
        for (int j = 0; j < l2; ++j) {
            if (i == 0 || j == 0)
                cache[i][j] = j + i;
            else
                cache[i][j] = INT_MAX;
        }
    }
}

template<typename T>
void PrintMatrix(T matrix) {
    using TSizeType = typename T::size_type;
    TSizeType n_rows = matrix.size();
    if (!n_rows)
        return;

    TSizeType n_cols = matrix[0].size();
    if (!n_cols)
        return;
    
    for (TSizeType i = 0; i < n_rows; ++i) {
        for (TSizeType j = 0; j < n_cols; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
typename T::size_type LevenshteinDistanceSimple(const T &source, const T &target, bool verify) noexcept {
    using TSizeType = typename T::size_type;
    auto n_rows = source.size();
    auto n_cols = target.size();

    std::vector<std::vector<TSizeType>> lev_dist_mtx(n_rows + 1);
    for (TSizeType i = 0; i <= n_rows; ++i) {
        lev_dist_mtx[i].resize(n_cols + 1);
        lev_dist_mtx[i][0] = i;
    }

    for (TSizeType i = 0; i <= n_cols; ++i) {
        lev_dist_mtx[0][i] = i;
    }

    for (TSizeType i = 1; i <= n_rows; ++i) {
        for (TSizeType j = 1; j <= n_cols; ++j) {
            if (source[i - 1] == target[j - 1]) {
                lev_dist_mtx[i][j] = lev_dist_mtx[i - 1][j - 1];
            } else {
                lev_dist_mtx[i][j] = std::min(std::min(lev_dist_mtx[i - 1][j], lev_dist_mtx[i][j - 1]), lev_dist_mtx[i - 1][j - 1]) + 1;
            }
        }
    }

    if (verify) {
        PrintMatrix(lev_dist_mtx);
    }

    return lev_dist_mtx[n_rows][n_cols];
}

template<typename T>
typename T::size_type LevenshteinDistance(const T &source, const T &target) noexcept {
    using TSizeType = typename T::size_type;
    if (source.size() > target.size()) {
        return LevenshteinDistance(target, source);
    }

    auto min_size = source.size();
    auto max_size = target.size();

    std::vector<TSizeType> lev_dist(min_size + 1);

    for (TSizeType i = 0; i <= min_size; ++i) {
        lev_dist[i] = i;
    }

    for (TSizeType j = 1; j <= max_size; ++j) {
        TSizeType previous_diagonal = lev_dist[0];
        TSizeType previous_diagonal_save;

        ++lev_dist[0];

        for (TSizeType i = 1; i <= min_size; ++i) {
            previous_diagonal_save = lev_dist[i];
            if (source[i - 1] == target[j - 1]) {
                lev_dist[i] = previous_diagonal;
            } else {
                lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
            }
            previous_diagonal = previous_diagonal_save;
        }
    }

    return lev_dist[min_size];
}

template<typename T>
typename T::size_type LevenshteinDistanceRecursive(const T &source, const T &target, 
                                                   int l1, int l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    int flag = (source[l1 - 1] != target[l2 - 1]);
    return std::min(std::min(LevenshteinDistanceRecursive(source, target, l1, l2 - 1) + 1,
                             LevenshteinDistanceRecursive(source, target, l1 - 1, l2) + 1),
                    LevenshteinDistanceRecursive(source, target, l1 - 1, l2 - 1) + flag);
}

template<typename T>
typename T::size_type LevenshteinDistanceRecursiveCache(const T &source, const T &target, 
                                                        int l1, int l2,
                                                        std::vector<std::vector<int>>& cache) noexcept {
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

template<typename T>
typename T::size_type DamerauLevenshteinDistanceSimple(const T &source, const T &target) noexcept {
    using TSizeType = typename T::size_type;
    auto n_rows = source.size();
    auto n_cols = target.size();

    std::vector<std::vector<TSizeType>> lev_dist_mtx(n_rows + 1);
    for (TSizeType i = 0; i <= n_rows; ++i) {
        lev_dist_mtx[i].resize(n_cols + 1);
        lev_dist_mtx[i][0] = i;
    }

    for (TSizeType i = 0; i <= n_cols; ++i) {
        lev_dist_mtx[0][i] = i;
    }

    for (TSizeType i = 1; i <= n_rows; ++i) {
        for (TSizeType j = 1; j <= n_cols; ++j) {
            if (source[i - 1] == target[j - 1]) {
                lev_dist_mtx[i][j] = lev_dist_mtx[i - 1][j - 1];
            } else {
                lev_dist_mtx[i][j] = std::min(std::min(lev_dist_mtx[i - 1][j], lev_dist_mtx[i][j - 1]), lev_dist_mtx[i - 1][j - 1]) + 1;
            }

            if (i > 1 && j > 1 && (source[i - 1] == target[j - 2] && source[i - 2] == target[j - 1])) {
                lev_dist_mtx[i][j] = std::min(lev_dist_mtx[i - 2][j - 2] + 1, lev_dist_mtx[i][j]);
            }
        }
    }

    return lev_dist_mtx[n_rows][n_cols];
}


template<typename T>
long MemLevenshteinDistanceSimple(const T &source, const T &target, bool verify) noexcept {
    using TSizeType = typename T::size_type;
    long bytes = 2 * sizeof(T&);
    auto n_rows = source.size();
    auto n_cols = target.size();

    bytes += sizeof(n_rows) + sizeof(n_cols);

    std::vector<std::vector<TSizeType>> lev_dist_mtx(n_rows + 1);
    for (TSizeType i = 0; i <= n_rows; ++i) {
        lev_dist_mtx[i].resize(n_cols + 1);
        lev_dist_mtx[i][0] = i;
    }

    for (TSizeType i = 0; i <= n_cols; ++i) {
        lev_dist_mtx[0][i] = i;
    }

    bytes += sizeof(TSizeType) * (n_rows + 1) * (n_cols + 1);

    for (TSizeType i = 1; i <= n_rows; ++i) {
        for (TSizeType j = 1; j <= n_cols; ++j) {
            if (source[i - 1] == target[j - 1]) {
                lev_dist_mtx[i][j] = lev_dist_mtx[i - 1][j - 1];
            } else {
                lev_dist_mtx[i][j] = std::min(std::min(lev_dist_mtx[i - 1][j], lev_dist_mtx[i][j - 1]), lev_dist_mtx[i - 1][j - 1]) + 1;
            }
        }
    }

    bytes += 4 * sizeof(TSizeType);

    if (verify) {
        PrintMatrix(lev_dist_mtx);
    }

    return bytes;
}

template<typename T>
long MemDamerauLevenshteinDistanceSimple(const T &source, const T &target) noexcept {
    using TSizeType = typename T::size_type;
    long bytes = 2 * sizeof(T&);
    auto n_rows = source.size();
    auto n_cols = target.size();

    bytes += sizeof(n_rows) + sizeof(n_cols);

    std::vector<std::vector<TSizeType>> lev_dist_mtx(n_rows + 1);
    for (TSizeType i = 0; i <= n_rows; ++i) {
        lev_dist_mtx[i].resize(n_cols + 1);
        lev_dist_mtx[i][0] = i;
    }

    for (TSizeType i = 0; i <= n_cols; ++i) {
        lev_dist_mtx[0][i] = i;
    }

    bytes += sizeof(TSizeType) * (n_rows + 1) * (n_cols + 1);

    for (TSizeType i = 1; i <= n_rows; ++i) {
        for (TSizeType j = 1; j <= n_cols; ++j) {
            if (source[i - 1] == target[j - 1]) {
                lev_dist_mtx[i][j] = lev_dist_mtx[i - 1][j - 1];
            } else {
                lev_dist_mtx[i][j] = std::min(std::min(lev_dist_mtx[i - 1][j], lev_dist_mtx[i][j - 1]), lev_dist_mtx[i - 1][j - 1]) + 1;
            }

            if (i > 1 && j > 1 && (source[i - 1] == target[j - 2] && source[i - 2] == target[j - 1])) {
                lev_dist_mtx[i][j] = std::min(lev_dist_mtx[i - 2][j - 2] + 1, lev_dist_mtx[i][j]);
            }
        }
    }

    bytes += 4 * sizeof(TSizeType);

    return bytes;
}

