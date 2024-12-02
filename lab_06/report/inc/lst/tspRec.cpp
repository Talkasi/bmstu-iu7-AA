void tspRec(int currentPos, int count, int cost, int path[], int n, int visited[], int *minCost, int bestPath[], int distanceMatrix[][SIZE]) 
{
    if (count == n && distanceMatrix[currentPos][0]) {
        if (cost + distanceMatrix[currentPos][0] < *minCost) {
            *minCost = cost + distanceMatrix[currentPos][0];
            for (int i = 0; i < n; ++i) {
                bestPath[i] = path[i];
            }
            bestPath[n] = 0;
        }
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && distanceMatrix[currentPos][i]) {
            visited[i] = 1;
            path[count] = i;
            tspRec(i, count + 1, cost + distanceMatrix[currentPos][i], path, n, visited, minCost, bestPath, distanceMatrix);
            visited[i] = 0;
        }
    }
}