#pragma once
#include <iostream>
#include <random>

using matrix_t = std::vector<std::vector<int>>;

matrix_t generateMatrix(int n_rows, int n_cols);
matrix_t createMatrix(int n_rows, int n_cols);
void printMatrix(const matrix_t &mtx);

matrix_t standartMatrixMul(const matrix_t &a, const matrix_t &b);
matrix_t standartWinogradMatrixMul(const matrix_t &a, const matrix_t &b);
matrix_t optimizedWinogradMatrixMul(const matrix_t &a, const matrix_t &b);
