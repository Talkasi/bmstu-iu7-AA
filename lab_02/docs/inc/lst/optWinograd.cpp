matrix_t optimizedWinogradMatrixMul(const matrix_t &a, const matrix_t &b)
{
    matrix_t res;
    if (!a.size() || !a[0].size() || !b.size() || !b[0].size())
        return res;

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
