#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <array>
#include <functional>
#include <string_view>

enum {
    DUMMY_SEARCH,
    BIN_SEARCH,
    BOTH_ALGS
} alg_commands;

enum {
    EXIT,
    SELF_CONTROLED_MODE,
    MESUREMENTS,
    N_COMMANDS
} menu_commands;

#define MENU "Program menu:\n" \
             "0. Exit\n" \
             "1. Self controled mode\n" \
             "2. Get mesurements\n"

#define ALG_MENU_PROMPT "Algorithms menu:\n" \
                        "0. Exhaustive search\n" \
                        "1. Binary search\n" \
                        "2. Both algorithms\n"

#define COMMAND_PROMPT          ">>> Enter command to execute: "
#define INPUT_ARRAY_N_PROMPT    ">>> Enter number of elements in the array: "
#define INPUT_ARRAY_N_ERROR     "[!] Error. N must be bigger than 0. Try again\n"
#define CHOOSE_X_PROMPT         ">>> Enter element to search for: "
#define GENERATED_ARRAY_MSG     "[+] Array was generated successfully: \n"
#define UNKNOWN_COMMAND_MSG     "[!] Unknown command, try again.\n"
#define NUMBER_OF_COMPARES_MSG  "[+] Number of compares done: \n"

#define X_REMINDER_MSG          "[+] You were searching for X = "
#define X_FOUND_MSG             "[+] It was found at the position with the index "
#define X_NOT_FOUND_MSG         "[-] It was not found in the given array.\n"

template<typename T> int dummySearch(const std::vector<T> &a, const T &x) noexcept;
template<typename T> int binSearch(const std::vector<T> &a, const T &x) noexcept;

void self_controled_mode();

// TODO:
// - Count number of compares
// - Write tests
// - Change code to find the correct index
// - Remind user the array
// - Add colors

int main() {
    int command = 1;
    while (command) {
        // TODO: Fix dumb input check
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

void self_controled_mode() {
    int n = 0, x = 0;
    ScanWithPrompt(n, INPUT_ARRAY_N_PROMPT, INPUT_ARRAY_N_ERROR, {[](int &val){ return val > 0; }});

    std::srand(unsigned(std::time(nullptr)));
    std::vector<int> a(n);
    std::generate(a.begin(), a.end(), my_rand);

    std::cout << GENERATED_ARRAY_MSG;
    PrintArray(a);

    std::cout << CHOOSE_X_PROMPT;
    std::cin >> x;

    int alg;
    ScanWithPrompt(alg, ALG_MENU_PROMPT COMMAND_PROMPT, UNKNOWN_COMMAND_MSG, {[](int &alg){
        return alg == DUMMY_SEARCH || alg == BIN_SEARCH || alg == BOTH_ALGS;
    }});

    if (alg == DUMMY_SEARCH || alg == BOTH_ALGS) {
        int res_i = dummySearch(a, x);
        PrintResults(x, res_i);
    }

    if (alg == BIN_SEARCH || alg == BOTH_ALGS) {
        std::sort(a.begin(), a.end());
        int res_i = binSearch(a, x);
        PrintResults(x, res_i);
    }
}

template<typename T>
int dummySearch(const std::vector<T> &a, const T &x) noexcept {
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] == x)
            return i;
    }

    return -1;
}

template<typename T>
int binSearch(const std::vector<T> &a, const T &x) noexcept {
    int l = 0;
    int r = a.size() - 1;
    int m = a.size() / 2;
    while (a[m] != x && l <= r) {
        if (x > a[m])
            l = m + 1;
        else
            r = m - 1;

        m = (r + l) / 2;
    }

    return l > r ? -1 : m;
}
