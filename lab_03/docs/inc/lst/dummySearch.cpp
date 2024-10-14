template<typename T>
int dummySearch(const std::vector<T> &a, const T &x) noexcept {
    for (int i = 0; i < (int)a.size(); ++i) {
        if (a[i] == x)
            return i;
    }

    return -1;
}