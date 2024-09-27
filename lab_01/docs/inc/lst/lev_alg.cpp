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