#pragma once
#include "search.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(search, len1_x_found)
{
    std::vector<int> a = {1};
    int i = 0;

    int dummy_search_res = dummySearch(a, a[i]);
    int bin_search_res = binSearch(a, a[i]);
    EXPECT_EQ(dummy_search_res, i);
    EXPECT_EQ(bin_search_res, i);
}

TEST(search, len1_x_not_found)
{
    std::vector<int> a = {123};

    int dummy_search_res = dummySearch(a, 1);
    int bin_search_res = binSearch(a, 1);
    EXPECT_EQ(dummy_search_res, -1);
    EXPECT_EQ(bin_search_res, -1);
}

TEST(search, len10_x_found_first)
{
    std::vector<int> a = {-1111, -234, -34, 0, 23, 45, 67, 3444, 23444, 99999};
    int i = 0;

    int dummy_search_res = dummySearch(a, a[i]);
    int bin_search_res = binSearch(a, a[i]);
    EXPECT_EQ(dummy_search_res, i);
    EXPECT_EQ(bin_search_res, i);
}

TEST(search, len10_x_found_middle)
{
    std::vector<int> a = {-1111, -234, -34, 0, 23, 45, 67, 3444, 23444, 99999};
    int i = a.size() / 2;

    int dummy_search_res = dummySearch(a, a[i]);
    int bin_search_res = binSearch(a, a[i]);
    EXPECT_EQ(dummy_search_res, i);
    EXPECT_EQ(bin_search_res, i);
}

TEST(search, len10_x_found_last)
{
    std::vector<int> a = {-1111, -234, -34, 0, 23, 45, 67, 3444, 23444, 99999};
    int i = a.size() - 1;

    int dummy_search_res = dummySearch(a, a[i]);
    int bin_search_res = binSearch(a, a[i]);
    EXPECT_EQ(dummy_search_res, i);
    EXPECT_EQ(bin_search_res, i);
}

TEST(search, len10_x_not_found)
{
    std::vector<int> a = {-1111, -234, -34, 0, 23, 45, 67, 3444, 23444, 99999};

    int dummy_search_res = dummySearch(a, 1);
    int bin_search_res = binSearch(a, 1);
    EXPECT_EQ(dummy_search_res, -1);
    EXPECT_EQ(bin_search_res, -1);
}
