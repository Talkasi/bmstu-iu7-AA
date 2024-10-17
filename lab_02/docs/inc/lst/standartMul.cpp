matrix_t standartMatrixMul(const matrix_t &a, const matrix_t &b)
{
    matrix_t res;
    if (!a.size() || !a[0].size() || !b.size() || !b[0].size())
        return res;

    res = createMatrix(a.size(), b[0].size());

    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < a[0].size(); ++j) {
            for (int k = 0; k < a.size(); ++k) {
                res[i][j] = res[i][j] + a[i][k] * b[k][j];
            }
        }
    }

    return res;
}