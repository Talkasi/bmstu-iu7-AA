template<typename T>
int binSearch(const std::vector<T> &a, const T &x) noexcept {
    int l = 0;
    int r = a.size() - 1;
    int m = a.size() / 2;
    while (a[m] != x && l <= r) {
        if (x > a[m])
            l = m + 1;
        else
            r = m - 1;

        m = (r + l) / 2;
    }

    return l > r ? -1 : m;
}