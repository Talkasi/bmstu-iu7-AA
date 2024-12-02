void initPheromone(double pheromone[SIZE][SIZE], size_t n, size_t m, double init_value)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            pheromone[i][j] = init_value;
}


void tspAnts(params *p, int distanceMatrix[SIZE][SIZE], int bestPath[SIZE], int* bestCost) 
{
    double pheromone[SIZE][SIZE];
    int path[SIZE];
    int visited[SIZE];
    initPheromone(pheromone, SIZE, SIZE, INIT_PHEROMONE_VAL);

    for (int day = 0; day < p->n_days; ++day) {
        for (int ant_k = 0; ant_k < SIZE; ++ant_k) {
            for (int i = 0; i < SIZE; ++i) {
                visited[i] = 0;
                path[i] = -1;
            }

            int currentCity = ant_k;
            path[0] = currentCity;
            visited[currentCity] = 1;
            int cost = 0;

            for (int step = 1; step < SIZE; ++step) {
                double total = 0.0;
                double probabilities[SIZE] = {0};

                for (int nextCity = 0; nextCity < SIZE; ++nextCity) {
                    if (!visited[nextCity]) {
                        probabilities[nextCity] = pow(pheromone[currentCity][nextCity], p->alpha) * 
                            pow(1.0 / distanceMatrix[currentCity][nextCity], p->beta);
                        total += probabilities[nextCity];
                    }
                }

                for (int nextCity = 0; nextCity < SIZE; ++nextCity) {
                    if (total > 0 && !visited[nextCity]) {
                        probabilities[nextCity] /= total;
                    } else {
                        probabilities[nextCity] = 0;
                    }
                }

                double randValue = (double)rand() / RAND_MAX;
                double cumulativeProbability = 0.0;
                for (int nextCity = 0; nextCity < SIZE; ++nextCity) {
                    cumulativeProbability += probabilities[nextCity];
                    if (cumulativeProbability >= randValue) {
                        currentCity = nextCity;
                        break;
                    }
                }

                path[step] = currentCity;
                visited[currentCity] = 1;
            }

            for (int step = 0; step < SIZE; ++step) {
                int nextCity = (step + 1) % SIZE;
                cost += distanceMatrix[path[step]][path[nextCity]];
            }

            if (cost < *bestCost) {
                *bestCost = cost;
                for (int i = 0; i < SIZE; ++i)
                    bestPath[i] = path[i];
            }

            for (int step = 0; step < SIZE; ++step) {
                int nextCity = (step + 1) % SIZE;
                pheromone[path[step]][path[nextCity]] += p->q / cost;
            }
        }

        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                pheromone[i][j] *= (1 - p->evaporation_rate);
                if (pheromone[i][j] < INIT_PHEROMONE_VAL)
                    pheromone[i][j] = INIT_PHEROMONE_VAL;
            }
        }
    }
}