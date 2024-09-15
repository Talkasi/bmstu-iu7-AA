#ifndef SEARCH_HPP
#define SEARCH_HPP
#include <vector>

template<typename T>
int dummySearch(const std::vector<T> &a, const T &x) noexcept;

template<typename T>
int binSearch(const std::vector<T> &a, const T &x) noexcept;

template<typename T>
int dummySearchForAnalysis(const std::vector<T> &a, const T &x, std::vector<std::pair<int,int>> &analysis_data) noexcept;

template<typename T>
int binSearchForAnalysis(const std::vector<std::pair<T,int>> &a, const T &x, std::vector<std::pair<int,int>> &analysis_data) noexcept;


template<typename T>
int dummySearch(const std::vector<T> &a, const T &x) noexcept {
    for (int i = 0; i < (int)a.size(); ++i) {
        if (a[i] == x)
            return i;
    }

    return -1;
}

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

template<typename T>
int dummySearchForAnalysis(const std::vector<T> &a, const T &x, std::vector<std::pair<int,int>> &analysis_data) noexcept {
    int cmps = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
		analysis_data.emplace_back(i, ++cmps);
        if (a[i] == x)
            return i;
    }

    return -1;
}

template<typename T>
int binSearchForAnalysis(const std::vector<std::pair<T,int>> &a, const T &x, std::vector<std::pair<int,int>> &analysis_data) noexcept {
    int cmps = 0;
    int l = 0;
    int r = a.size() - 1;
    int m = a.size() / 2;
    analysis_data.emplace_back(m, ++cmps);
    while (a[m].first != x && l <= r) {
        if (x > a[m].first)
            l = m + 1;
        else
            r = m - 1;

        m = (r + l) / 2;
        analysis_data.emplace_back(m, ++cmps);
    }

    return l > r ? -1 : m;
}

#endif