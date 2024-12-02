#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 11
#define N_ITERATIONS 100

#define NUM_ANTS 1
#define INIT_PHEROMONE_VAL 0.05

#define CSV 0
#define LATEX 1

void generateSymmetricMatrix(int matrix[SIZE][SIZE], int max_val);
void printMatrix(int matrix[SIZE][SIZE]);

struct params {
    double alpha;
    double beta;
    double evaporation_rate;
    int q;
    int n_days;
};

void tspRec(int currentPos, int count, int cost, int path[], int n, int visited[], int *minCost, int bestPath[], int distanceMatrix[][SIZE]);
void tspAnts(params *p, int distanceMatrix[SIZE][SIZE], int bestPath[SIZE], int* bestCost);
void parametrization(params *p, int type);

double measureRecTime(int matrix[SIZE][SIZE]);
double measureAntTime(params &p, int matrix[SIZE][SIZE]);

int calcQ(int distanceMatrix[SIZE][SIZE])
{
    int sum_q = 0;
    int n_q = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (i != j) {
                ++n_q;
                sum_q += distanceMatrix[i][j];
            }
        }
    }   

    return sum_q / n_q;        
}

int main()
{
    srand(time(NULL));
    // int distanceMatrix[SIZE][SIZE] = {
    //     {0,      800,   1500,  2000,  1500,  1000,  300,   200,   400,   1200},
    //     {800,    0,     1000,  1300,  1200,  1500,  1600,  1200,  900,   200},
    //     {1500,   1000,  0,     1000,  800,   1200,  700,   1300,  1000,  1100},
    //     {2000,   1300,  1000,  0,     600,   1500,  1200,  1600,  1300,  1400},
    //     {1500,   1200,  800,   600,   0,     1000,  900,   1100,  800,   900},
    //     {1000,   1500,  1200,  1500,  1000,  0,     800,   600,   1300,  1200},
    //     {300,    1600,  700,   1200,  900,   800,   0,     400,   500,   600},
    //     {200,    1200,  1300,  1600,  1100,  600,   400,   0,     300,   800},
    //     {400,    900,   1000,  1300,  800,   1300,  500,   300,   0,     700},
    //     {1200,   200,   1100,  1400,  900,   1200,  600,   800,   700,   0}
    // };

    // int distanceMatrix[SIZE][SIZE] = {
    //     {0, 1, 2, 3},
    //     {1, 0, 4, 5},
    //     {2, 4, 0, 6},
    //     {3, 5, 6, 0}
    // };

    // int distanceMatrix[SIZE][SIZE] = {
    //     {0, 27, 12, 21},
    //     {27, 0, 16, 24},
    //     {12, 16, 0, 11},
    //     {21, 24, 11, 0}
    // };

    // int distanceMatrix[SIZE][SIZE] = {
    //     {0, 1, 2, 3, 4},
    //     {1, 0, 5, 6, 7},
    //     {2, 5, 0, 8, 9},
    //     {3, 6, 8, 0, 10},
    //     {4, 7, 9, 10, 0}
    // };

    int matrix[SIZE][SIZE];
    generateSymmetricMatrix(matrix, 10); 

    params p;
    p.alpha = 0;
    p.beta = 1;
    p.evaporation_rate = 0.1;
    p.q = calcQ(matrix);
    p.n_days = 100;

    int bestPath[SIZE];
    int bestCostAnts = INT_MAX;

    printf("Проводится параметризация муравьиного алгоритма.\n");
    // parametrization(&p, LATEX);
    printf("Подобранные коэффициенты: a = %lf, b = %lf, e = %lf, days = %d\n", p.alpha, p.beta, p.evaporation_rate, p.n_days);

    int n = 100;
    double a = 0;
    double r = 0;

    for (int i = 0; i < n; ++i) {
        r += measureRecTime(matrix);
        a += measureAntTime(p, matrix);
    }

    printf(" %d & %lf & %lf \\\\\\hline\n", SIZE, r / n, a / n);

    return 0;
}

// currentPos       Текущий город, в котором находится "коммивояжер"
// count            Количество посещенных городов
// cost             Текущая стоимость пути (сумма расстояний)
// path             Массив, который хранит последовательность посещенных городов
// n                Общее количество городов
// visited          Массив, который отслеживает, какие города уже были посещены (1 - посещен, 0 - не посещен)
// *minCost         Указатель на переменную, хранящую минимальную стоимость найденного пути
// bestPath         Массив, который будет хранить лучший найденный путь
// distanceMatrix   Матрица расстояний между городами
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

void initPheromone(double pheromone[][SIZE], size_t n, size_t m, double init_value)
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

void generateSymmetricMatrix(int matrix[SIZE][SIZE], int max_val) 
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            } else if (i < j) {
                matrix[i][j] = rand() % max_val + 1;
                matrix[j][i] = matrix[i][j];
            }
        }
    }
}

void printMatrix(int matrix[SIZE][SIZE]) 
{
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

double measureRecTime(int matrix[SIZE][SIZE]) {
    clock_t start, end;
    double cpu_time_used;

    int visited[SIZE]= {1};
    int path[SIZE] = {};
    int bestPathRec[SIZE];
    int optimal;

    start = clock();
    tspRec(0, 1, 0, path, SIZE, visited, &optimal, bestPathRec, matrix);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}

double measureAntTime(params &p, int matrix[SIZE][SIZE]) {
    clock_t start, end;
    double cpu_time_used;

    int newPramCostAnts = INT_MAX;
    int newParamPath[SIZE] = {};

    start = clock();
    tspAnts(&p, matrix, newParamPath, &newPramCostAnts);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}

void parametrization(params *p, int type) {
    int max_val = 30;

    int matrix1[SIZE][SIZE];
    int matrix2[SIZE][SIZE];
    int matrix3[SIZE][SIZE];

    generateSymmetricMatrix(matrix1, max_val);
    generateSymmetricMatrix(matrix2, max_val);
    generateSymmetricMatrix(matrix3, max_val);

    printMatrix(matrix1);
    printMatrix(matrix2);
    printMatrix(matrix3);

    int optimal1 = INT_MAX;
    int optimal2 = INT_MAX;
    int optimal3 = INT_MAX;

    int visited1[SIZE]= {1};
    int path1[SIZE] = {};
    int bestPathRec[SIZE];

    tspRec(0, 1, 0, path1, SIZE, visited1, &optimal1, bestPathRec, matrix1);
    int visited2[SIZE]= {1};
    int path2[SIZE] = {};

    tspRec(0, 1, 0, path2, SIZE, visited2, &optimal2, bestPathRec, matrix2);

    int visited3[SIZE]= {1};
    int path3[SIZE] = {};
    tspRec(0, 1, 0, path3, SIZE, visited3, &optimal3, bestPathRec, matrix3);

    FILE *file1 = fopen("parametrization_class1.txt", "w");
    FILE *file2 = fopen("parametrization_class2.txt", "w");
    FILE *file3 = fopen("parametrization_class3.txt", "w");

    if (!file1 || !file2 || !file3)
        printf("Error\n");

    int paramBestCostAnts1 = INT_MAX;
    int paramBestCostAnts2 = INT_MAX;
    int paramBestCostAnts3 = INT_MAX;

    double a = 0, b = 0, e = 0;
    for (; a < 1; a += 0.1) {
        b = 1 - a;
        for (e = 0; e < 1; e += 0.1) {
            for (int days = 50; days < 51; days += 50) {
                int n_tests = 50;
                int sumCost1 = 0;
                int sumCost2 = 0;
                int sumCost3 = 0;

                for (int i = 0; i < n_tests; ++i) {
                    params new_p = {a, b, e, p->q, days};
                    int newPramCostAnts1 = INT_MAX;
                    int newPramCostAnts2 = INT_MAX;
                    int newPramCostAnts3 = INT_MAX;
                    int newParamPath1[SIZE] = {};
                    int newParamPath2[SIZE] = {};
                    int newParamPath3[SIZE] = {};
                    tspAnts(&new_p, matrix1, newParamPath1, &newPramCostAnts1);
                    tspAnts(&new_p, matrix2, newParamPath2, &newPramCostAnts2);
                    tspAnts(&new_p, matrix3, newParamPath3, &newPramCostAnts3);
                    sumCost1 += newPramCostAnts1;
                    sumCost2 += newPramCostAnts2;
                    sumCost3 += newPramCostAnts3;
                }

                double res1 = (double)sumCost1 / n_tests;
                double res2 = (double)sumCost2 / n_tests;
                double res3 = (double)sumCost3 / n_tests;
                if (res1 + res2 + res3 < paramBestCostAnts1 + paramBestCostAnts2 + paramBestCostAnts3) {
                    paramBestCostAnts1 = res1;
                    paramBestCostAnts2 = res2;
                    paramBestCostAnts3 = res3;
                    p->alpha = a;
                    p->beta = b;
                    p->evaporation_rate = e;
                    p->n_days = days;
                }

                const char *sep;
                const char *ender;

                if (type == LATEX) {
                    sep = " & ";
                    ender = " \\\\";
                } else if (type == CSV) {
                    sep = ", ";
                    ender = "";
                } else {
                    sep = " | ";
                    ender = "";
                }

                fprintf(file1, "%4.1f%s%4.1f%s%4.1f%s%d%s%5d%s\n", a, sep, b, sep, e, sep, days, sep, (int)(res1 - optimal1), ender);
                fprintf(file2, "%4.1f%s%4.1f%s%4.1f%s%d%s%5d%s\n", a, sep, b, sep, e, sep, days, sep, (int)(res2 - optimal2), ender);
                fprintf(file3, "%4.1f%s%4.1f%s%4.1f%s%d%s%5d%s\n", a, sep, b, sep, e, sep, days, sep, (int)(res3 - optimal3), ender);
            }
        }
    }

    fclose(file1);
    fclose(file2);
    fclose(file3);
}
