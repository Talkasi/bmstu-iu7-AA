matrix_t standartWinogradMatrixMul(const matrix_t &a, const matrix_t &b)
{
    matrix_t res;
    if (!a.size() || !a[0].size() || !b.size() || !b[0].size())
        return res;

	int n = a.size();
	int m = b.size();
	int t = b[0].size();

	res = createMatrix(n, t);

	std::vector<int> rowFactor(n);
	std::vector<int> columnFactor(t);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m / 2; ++j)
			rowFactor[i] = rowFactor[i] + a[i][2 * j + 1] * a[i][2 * j];
	}

	for (int i = 0; i < t; ++i) {
		for (int j = 0; j < m / 2; ++j)
			columnFactor[i] = columnFactor[i] + b[2 * j + 1][i] * b[2 * j][i];
	}

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < t; ++j) {
			res[i][j] = -rowFactor[i] - columnFactor[j];
			for (int k = 0; k < m / 2; ++k)
			{
				res[i][j] = res[i][j] + (a[i][2 * k + 1] + b[2 * k][j])
					* (a[i][2 * k] + b[2 * k + 1][j]);

			}
		}

	if (m % 2) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < t; ++j) {
				res[i][j] = res[i][j] + a[i][m - 1] * b[m - 1][j];
            }
        }
    }

	return res;
}