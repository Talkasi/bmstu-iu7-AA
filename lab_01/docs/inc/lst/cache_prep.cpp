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