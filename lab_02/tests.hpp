#pragma once
#include "matrix.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(mtx_mul, square_matrix)
{
    matrix_t a = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    matrix_t b = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    matrix_t res1 = standartMatrixMul(a, b);
    matrix_t res2 = standartWinogradMatrixMul(a, b);
    matrix_t res3 = optimizedWinogradMatrixMul(a, b);

    EXPECT_EQ(res1.size(), a.size());
    EXPECT_EQ(res2.size(), a.size());
    EXPECT_EQ(res3.size(), a.size());

    EXPECT_EQ(res1[0].size(), b[0].size());
    EXPECT_EQ(res2[0].size(), b[0].size());
    EXPECT_EQ(res3[0].size(), b[0].size());

    for (size_t i = 0; i < res1.size(); ++i)
    {
        for (size_t j = 0; j < res1[0].size(); ++j)
        {
            EXPECT_EQ(res1[i][j], res2[i][j]);
            EXPECT_EQ(res2[i][j], res3[i][j]);
        }
    }
}

TEST(mtx_mul, mul_on_e)
{
    matrix_t a = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    matrix_t b = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

    matrix_t res1 = standartMatrixMul(a, b);
    matrix_t res2 = standartWinogradMatrixMul(a, b);
    matrix_t res3 = optimizedWinogradMatrixMul(a, b);

    EXPECT_EQ(res1.size(), a.size());
    EXPECT_EQ(res2.size(), a.size());
    EXPECT_EQ(res3.size(), a.size());

    EXPECT_EQ(res1[0].size(), b[0].size());
    EXPECT_EQ(res2[0].size(), b[0].size());
    EXPECT_EQ(res3[0].size(), b[0].size());

    for (size_t i = 0; i < res1.size(); ++i)
    {
        for (size_t j = 0; j < res1[0].size(); ++j)
        {
            EXPECT_EQ(res1[i][j], res2[i][j]);
            EXPECT_EQ(res2[i][j], res3[i][j]);
        }
    }
}

TEST(mtx_mul, e_on_mul)
{
    matrix_t a = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    matrix_t b = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    matrix_t res1 = standartMatrixMul(a, b);
    matrix_t res2 = standartWinogradMatrixMul(a, b);
    matrix_t res3 = optimizedWinogradMatrixMul(a, b);

    EXPECT_EQ(res1.size(), a.size());
    EXPECT_EQ(res2.size(), a.size());
    EXPECT_EQ(res3.size(), a.size());

    EXPECT_EQ(res1[0].size(), b[0].size());
    EXPECT_EQ(res2[0].size(), b[0].size());
    EXPECT_EQ(res3[0].size(), b[0].size());

    for (size_t i = 0; i < res1.size(); ++i)
    {
        for (size_t j = 0; j < res1[0].size(); ++j)
        {
            EXPECT_EQ(res1[i][j], res2[i][j]);
            EXPECT_EQ(res2[i][j], res3[i][j]);
        }
    }
}

TEST(mtx_mul, one_x_one_matrix)
{
    matrix_t a = {{1}};
    matrix_t b = {{1}};

    matrix_t res1 = standartMatrixMul(a, b);
    matrix_t res2 = standartWinogradMatrixMul(a, b);
    matrix_t res3 = optimizedWinogradMatrixMul(a, b);

    EXPECT_EQ(res1.size(), a.size());
    EXPECT_EQ(res2.size(), a.size());
    EXPECT_EQ(res3.size(), a.size());

    EXPECT_EQ(res1[0].size(), b[0].size());
    EXPECT_EQ(res2[0].size(), b[0].size());
    EXPECT_EQ(res3[0].size(), b[0].size());

    for (size_t i = 0; i < res1.size(); ++i)
    {
        for (size_t j = 0; j < res1[0].size(); ++j)
        {
            EXPECT_EQ(res1[i][j], res2[i][j]);
            EXPECT_EQ(res2[i][j], res3[i][j]);
        }
    }
}

TEST(mtx_mul, rect_matrix)
{
    matrix_t a = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    matrix_t b = {{1, 2}, {4, 5}, {7, 8}};

    matrix_t res1 = standartMatrixMul(a, b);
    matrix_t res2 = standartWinogradMatrixMul(a, b);
    matrix_t res3 = optimizedWinogradMatrixMul(a, b);

    EXPECT_EQ(res1.size(), a.size());
    EXPECT_EQ(res2.size(), a.size());
    EXPECT_EQ(res3.size(), a.size());

    EXPECT_EQ(res1[0].size(), b[0].size());
    EXPECT_EQ(res2[0].size(), b[0].size());
    EXPECT_EQ(res3[0].size(), b[0].size());

    for (size_t i = 0; i < res1.size(); ++i)
    {
        for (size_t j = 0; j < res1[0].size(); ++j)
        {
            EXPECT_EQ(res1[i][j], res2[i][j]);
            EXPECT_EQ(res2[i][j], res3[i][j]);
        }
    }
}
