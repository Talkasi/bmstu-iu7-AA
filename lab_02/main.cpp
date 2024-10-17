#include <iostream>
#include <gtest/gtest.h>
#include <wchar.h>
#include <fstream>
#include "tests.hpp"
#include "matrix.hpp"

// Вариант: 
// - двоичный сдвиг вместо умножения на 2; 
// - объединение III и IV частей алгоритма Винограда; 
// - введение декремента при вычислении вспомогательных массивов;

enum menu_commands {
    EXIT,
    STANDART_MUL,
    STANDART_WINOGRAD,
    OPTIMIZED_WINOGRAD,
    MEASURE_TIME_ALL,
    TEST_ALL,
    N_COMMANDS
};

#define TIMEDIVISION_MS 1e6 
#define OUT_DIR "data/"

#define LIGHT_GREEN_C   "\033[1;32m"
#define RED_C           "\033[0;31m"
#define GREEN_C         "\033[0;32m"
#define NC              "\033[0m"

#define MENU "Program menu:\n" \
             "0. Exit\n" \
             "1. Standart matrix multiplication\n" \
             "2. Standart Winograd algorithm\n" \
             "3. Optimized Winograd algorithm\n" \
             "4. Mesure time all\n" \
             "5. Test all\n"

#define COMMAND_PROMPT              LIGHT_GREEN_C ">>> Enter command to execute: " NC
#define UNKNOWN_COMMAND_MSG         "[!] Unknown command, try again.\n"
#define SCAN_FIRST_NROWS_PROMPT     GREEN_C ">>> Enter number of rows of the first matrix: " NC
#define SCAN_FIRST_NCOLS_PROMPT     GREEN_C ">>> Enter number of columns of the first matrix: " NC
#define SECOND_N_ROWSS_IS_MSG       "Number of rows of the matrix is: " 
#define SCAN_SECOND_NCOLS_PROMPT    GREEN_C ">>> Enter number of columns of the second matrix: " NC
#define ANSWER_MSG                  "[+] The result matrix is: "
#define TEST_COMPLETE_MSG           "[+] Testing complete\n"

matrix_t GetMtxAndRun(int alg);
void MeasureTimeAll();

int main() {
    srand(time(NULL));

    int command = 1;
    while (command) {
        std::cout << MENU << COMMAND_PROMPT;
        std::cin >> command;

        switch (command) {
            case EXIT:
                break;
            case STANDART_MUL:
            case STANDART_WINOGRAD:
            case OPTIMIZED_WINOGRAD:
            {
                matrix_t res = GetMtxAndRun(command);
                std::cout << ANSWER_MSG << std::endl;
                printMatrix(res);
            } break;
            case MEASURE_TIME_ALL:
            {
                MeasureTimeAll();
            } break;
            case TEST_ALL:
            {
                setlocale(LC_ALL, "Russian");
                testing::InitGoogleTest();
                int res = RUN_ALL_TESTS();
                if (res)
                    std::cout << TEST_COMPLETE_MSG;
            } break;
            default:
                std::cout << UNKNOWN_COMMAND_MSG;
                break;
        }
    }

    return 0;
}

matrix_t GetMtxAndRun(int alg) {
    int first_n_rows, first_n_cols;
    int second_n_rows, second_n_cols;

    std::cout << SCAN_FIRST_NROWS_PROMPT;
    std::cin >> first_n_rows;

    std::cout << SCAN_FIRST_NCOLS_PROMPT;
    std::cin >> first_n_cols;

    second_n_rows = first_n_cols;
    std::cout << SECOND_N_ROWSS_IS_MSG << second_n_rows << std::endl;

    std::cout << SCAN_SECOND_NCOLS_PROMPT;
    std::cin >> second_n_cols;

    matrix_t a = generateMatrix(first_n_rows, first_n_cols);
    matrix_t b = generateMatrix(second_n_rows, second_n_cols);

    std::cout << "First matrix:\n";
    printMatrix(a);
    
    std::cout << "Second matrix:\n";
    printMatrix(b);

    matrix_t res;
    switch (alg) {
        case STANDART_MUL:
            res = standartMatrixMul(a, b);
            break;
        case STANDART_WINOGRAD:
            res = standartWinogradMatrixMul(a, b);
            break;
        case OPTIMIZED_WINOGRAD:
            res = optimizedWinogradMatrixMul(a, b);
            break;
        default:
            std::cout << UNKNOWN_COMMAND_MSG;
            break;
    }

    return res;
}

long getCpuTime()
{
	struct timespec t;

	if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t)) {
		std::cerr << "Impossible to get time\n";
		return -1;
	}

	return t.tv_sec * 1000000000LL + t.tv_nsec;
}

#define N 1000
void MeasureTimeAll() {
    std::vector ns = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 30, 35, 40, 45, 50};

    std::ofstream all_mesurements(OUT_DIR "all_mesurements.txt");
    if (!all_mesurements.is_open())
        std::cout << "File error!" << std::endl;

    matrix_t res;
    for (int n : ns) {
        long startTime, endTime;

        double sum_time_standartMul = 0;
        double sum_time_standartWinograd = 0;
        double sum_time_optWinograd = 0;

        matrix_t a = generateMatrix(n, n);
        matrix_t b = generateMatrix(n, n);

        for (int i = 0; i < N; ++i) {
            startTime = getCpuTime();
            res = standartMatrixMul(a, b);
            endTime = getCpuTime();
            sum_time_standartMul = (endTime - startTime) / TIMEDIVISION_MS;

            startTime = getCpuTime();
            res = standartWinogradMatrixMul(a, b);
            endTime = getCpuTime();
            sum_time_standartWinograd = (endTime - startTime) / TIMEDIVISION_MS;

            startTime = getCpuTime();
            res = optimizedWinogradMatrixMul(a, b);
            endTime = getCpuTime();
            sum_time_optWinograd = (endTime - startTime) / TIMEDIVISION_MS;
        }

        all_mesurements << n << "\t";
        all_mesurements << sum_time_standartMul / N << " ";
        all_mesurements << sum_time_standartWinograd / N << " ";
        all_mesurements << sum_time_optWinograd / N << " ";
        all_mesurements << std::endl;
    }

    all_mesurements.close();
}
