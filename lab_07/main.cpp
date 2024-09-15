#include "search.hpp"
#include "tests.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <gtest/gtest.h>

enum alg_commands {
    DUMMY_SEARCH,
    BIN_SEARCH,
    BOTH_ALGS
};

enum menu_commands {
    EXIT,
    SELF_CONTROLED_MODE,
    MESUREMENTS,
    TESTS,
    N_COMMANDS
};

#define LIGHT_GREEN_C   "\033[1;32m"
#define RED_C           "\033[0;31m"
#define GREEN_C         "\033[0;32m"
#define NC              "\033[0m"

#define MENU "Program menu:\n" \
             "0. Exit\n" \
             "1. Self controled mode\n" \
             "2. Get mesurements\n" \
             "3. Test all\n" 

#define ALG_MENU_PROMPT "Algorithms menu:\n" \
                        "0. Exhaustive search\n" \
                        "1. Binary search\n" \
                        "2. Both algorithms\n"

#define COMMAND_PROMPT          LIGHT_GREEN_C ">>> Enter command to execute: " NC
#define INPUT_ARRAY_N_PROMPT    LIGHT_GREEN_C ">>> Enter number of elements in the array: " NC
#define INPUT_ARRAY_N_ERROR     RED_C "[!] Error. N must be bigger than 0. Try again\n" NC
#define CHOOSE_X_PROMPT         LIGHT_GREEN_C ">>> Enter element to search for: " NC
#define ARRAY_OUTPUT_MSG        GREEN_C "[+] Array you are working with: \n" NC
#define UNKNOWN_COMMAND_MSG     "[!] Unknown command, try again.\n"
#define NUMBER_OF_COMPARES_MSG  "[+] Number of compares done: " 
#define TEST_COMPLETE_MSG       GREEN_C "[+] Tests completed succesfully" NC

#define X_REMINDER_MSG          "[+] You were searching for X = " 
#define X_FOUND_MSG             "[+] It was found at the position with the index "
#define X_NOT_FOUND_MSG         "[-] It was not found in the given array.\n"

void self_controled_mode();

// TODO:
// - Timings

int main() {
    int command = 1;
    while (command) {
        std::cout << MENU << COMMAND_PROMPT;
        std::cin >> command;

        switch (command) {
            case EXIT:
                break;
            case SELF_CONTROLED_MODE:
                self_controled_mode();
                break;
            case MESUREMENTS:
                break;
            case TESTS: {
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

int my_rand() {
    return std::rand() % 1000;
}

template<typename T> 
void ScanWithPrompt(T &val, 
    const std::string &prompt, 
    const std::string &err,
    bool (*verify)(T &val))
{
    std::cout << prompt;
    std::cin >> val;
    while (!verify(val)) {
        std::cout << err << std::endl
                  << prompt;
        std::cin >> val;
    }
}

template<typename T>
void PrintArray(T &a) {
    std::cout << "[";
    for (typename T::size_type i = 0; i < a.size() - 1; ++i) {
        std::cout << a[i] << ", ";
    }
    std::cout << a[a.size() - 1] << "]\n";
}

void PrintResults(int x, int res_i, int cmps = 0) {
    std::cout << X_REMINDER_MSG << x << std::endl;
    if (res_i < 0) {
        std::cout << X_NOT_FOUND_MSG;
    } else {
        std::cout << X_FOUND_MSG << res_i << "." << std::endl;
    }

    std::cout << NUMBER_OF_COMPARES_MSG << cmps << std::endl;
}

void GeneratePairsVector(std::vector<std::pair<int,int>> &res, std::vector<int> a) {
    for (typename std::vector<int>::size_type i = 0; i < a.size(); ++i) {
        res.emplace_back(a[i], i);
    }
}

void self_controled_mode() {
    int n = 0, x = 0;
    ScanWithPrompt(n, INPUT_ARRAY_N_PROMPT, INPUT_ARRAY_N_ERROR, {[](int &val){ return val > 0; }});

    std::srand(unsigned(std::time(nullptr)));
    std::vector<int> a(n);
    std::generate(a.begin(), a.end(), my_rand);

    std::cout << ARRAY_OUTPUT_MSG;
    PrintArray(a);

    std::cout << CHOOSE_X_PROMPT;
    std::cin >> x;

    int alg;
    ScanWithPrompt(alg, ALG_MENU_PROMPT COMMAND_PROMPT, UNKNOWN_COMMAND_MSG, {[](int &alg){
        return alg == DUMMY_SEARCH || alg == BIN_SEARCH || alg == BOTH_ALGS;
    }});

    std::cout << ARRAY_OUTPUT_MSG;
    PrintArray(a);

    std::vector<std::pair<int,int>> analysis_data;
    if (alg == DUMMY_SEARCH || alg == BOTH_ALGS) {
        int res_i = dummySearchForAnalysis(a, x, analysis_data);
        PrintResults(x, res_i, analysis_data[res_i].second);
    }

    if (alg == BIN_SEARCH || alg == BOTH_ALGS) {
        std::vector<std::pair<int,int>> prepared_a;
        GeneratePairsVector(prepared_a, a);

        std::sort(prepared_a.begin(), prepared_a.end(), [](std::pair<int,int> &a, std::pair<int,int> &b){return a.first < b.first;});
        int res_i = binSearchForAnalysis(prepared_a, x, analysis_data);
        PrintResults(x, prepared_a[res_i].second, analysis_data[analysis_data.size() - 1].second);
    }
}
