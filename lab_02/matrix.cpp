#include "matrix.hpp"

static int get_rand_int();

matrix_t generateMatrix(int n_rows, int n_cols)
{
    matrix_t mtx(n_rows);
    for (int i = 0; i < n_rows; ++i)
    {
        mtx[i].resize(n_cols);
        for (int j = 0; j < n_cols; ++j)
        {
            mtx[i][j] = get_rand_int();
        }
    }

    return mtx;
}

matrix_t createMatrix(int n_rows, int n_cols)
{
    matrix_t mtx(n_rows);
    for (int i = 0; i < n_rows; ++i)
    {
        mtx[i].resize(n_cols);
        for (int j = 0; j < n_cols; ++j)
        {
            mtx[i][j] = 0;
        }
    }
    return mtx;
}

void printMatrix(const matrix_t &mtx)
{
    for (int i = 0; i < (int)mtx.size(); ++i)
    {
        for (int j = 0; j < (int)mtx[0].size(); ++j)
        {
            std::cout << mtx[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

matrix_t standartMatrixMul(const matrix_t &a, const matrix_t &b)
{
    matrix_t res;
    if (!a.size() || !a[0].size() || !b.size() || !b[0].size())
        return res;

    res = createMatrix(a.size(), b[0].size());

    for (int i = 0; i < (int)a.size(); ++i)
    {
        for (int j = 0; j < (int)a[0].size(); ++j)
        {
            for (int k = 0; k < (int)a.size(); ++k)
            {
                res[i][j] = res[i][j] + a[i][k] * b[k][j];
            }
        }
    }

    return res;
}

matrix_t standartWinogradMatrixMul(const matrix_t &a, const matrix_t &b)
{
    matrix_t res;
    if (!a.size() || !a[0].size() || !b.size() || !b[0].size())
    {
        return res;
    }

    int n = a.size();
    int m = b.size();
    int t = b[0].size();

    res = createMatrix(n, t);

    std::vector<int> rowFactor(n);
    std::vector<int> columnFactor(t);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m / 2; ++j)
            rowFactor[i] = rowFactor[i] + a[i][2 * j + 1] * a[i][2 * j];
    }

    for (int i = 0; i < t; ++i)
    {
        for (int j = 0; j < m / 2; ++j)
            columnFactor[i] = columnFactor[i] + b[2 * j + 1][i] * b[2 * j][i];
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < t; ++j)
        {
            res[i][j] = -rowFactor[i] - columnFactor[j];
            for (int k = 0; k < m / 2; ++k)
            {
                res[i][j] = res[i][j] + (a[i][2 * k + 1] + b[2 * k][j]) * (a[i][2 * k] + b[2 * k + 1][j]);
            }
        }

    if (m % 2)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < t; ++j)
            {
                res[i][j] = res[i][j] + a[i][m - 1] * b[m - 1][j];
            }
        }
    }

    return res;
}

// Вариант:
// - двоичный сдвиг вместо умножения на 2;
// - объединение III и IV частей алгоритма Винограда;
// - введение декремента при вычислении вспомогательных массивов;
matrix_t optimizedWinogradMatrixMul(const matrix_t &a, const matrix_t &b)
{
    matrix_t res;
    if (!a.size() || !a[0].size() || !b.size() || !b[0].size())
    {
        return res;
    }

    int n = a.size();
    int m = b.size();
    int t = b[0].size();

    int half_m = m / 2;

    res = createMatrix(n, t);

    std::vector<int> rowFactor(n);
    std::vector<int> columnFactor(t);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < half_m; ++j)
            rowFactor[i] -= a[i][j << 1] * a[i][(j << 1) + 1];
    }

    for (int i = 0; i < t; ++i)
    {
        for (int j = 0; j < half_m; ++j)
            columnFactor[i] -= b[j << 1][i] * b[(j << 1) + 1][i];
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < t; ++j)
        {
            res[i][j] = rowFactor[i] + columnFactor[j];
            for (int k = 0; k < half_m; ++k)
            {
                res[i][j] += (a[i][(k << 1) + 1] + b[k << 1][j]) * (a[i][k << 1] + b[(k << 1) + 1][j]);
            }

            if (m % 2)
            {
                res[i][j] += a[i][m - 1] * b[m - 1][j];
            }
        }
    }

    return res;
}

static int get_rand_int()
{
    return std::rand() % 100 - 50;
}