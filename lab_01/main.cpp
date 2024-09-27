#include <iostream>
#include <gtest/gtest.h>
#include <random>
#include <iostream>
#include <wchar.h>
#include <fstream>
#include "tests.hpp"
#include "timings.hpp"
#include "distance.hpp"

enum menu_commands {
    EXIT,
    LEVENSHTEIN_DISTANCE_SIMPLE,
    LEVENSHTEIN_DISTANCE,
    LEVENSHTEIN_DISTANCE_RECURSIVE,
    LEVENSHTEIN_DISTANCE_RECURSIVE_CACHE,
    DAMERAU_LEVENSHTEIN_DISTANCE_SIMPLE,
    MEASURE_TIME_ALL,
    MEASURE_MEM_ALL,
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
             "1. Levenshtein distance simple (matrix-based)\n" \
             "2. Levenshtein distance (saving diagonal)\n" \
             "3. Levenshtein distance recursive\n" \
             "4. Levenshtein distance recursive with cache\n" \
             "5. Damerau-Levenshtein distance (matrix-based)\n" \
             "6. Mesure time all\n" \
             "7. Mesure mem all\n" \
             "8. Test all\n"

#define COMMAND_PROMPT          LIGHT_GREEN_C ">>> Enter command to execute: " NC
#define UNKNOWN_COMMAND_MSG     "[!] Unknown command, try again.\n"
#define SCAN_FIRST_STR_PROMPT   GREEN_C ">>> Enter first string: " NC
#define SCAN_SECOND_STR_PROMPT  GREEN_C ">>> Enter second string: " NC
#define ANSWER_MSG              "[+] The distance is: "
#define TEST_COMPLETE_MSG       "[+] Testing complete\n"

int GetStrAndRun(int alg);
void MeasureTimeAll();
void MeasureMemAll();

int main() {
    int command = 1;
    while (command) {
        std::cout << MENU << COMMAND_PROMPT;
        std::cin >> command;

        switch (command) {
            case EXIT:
                break;
            case LEVENSHTEIN_DISTANCE:
            case LEVENSHTEIN_DISTANCE_SIMPLE:
            case LEVENSHTEIN_DISTANCE_RECURSIVE:
            case LEVENSHTEIN_DISTANCE_RECURSIVE_CACHE:
            case DAMERAU_LEVENSHTEIN_DISTANCE_SIMPLE:
            {
                int res = GetStrAndRun(command);
                std::cout << ANSWER_MSG << res << std::endl;
            } break;
            case MEASURE_TIME_ALL:
            {
                MeasureTimeAll();
            } break;
            case MEASURE_MEM_ALL:
            {
                MeasureMemAll();
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

int GetStrAndRun(int alg) {
    std::string s1;
    std::string s2;

    std::cout << SCAN_FIRST_STR_PROMPT;
    std::cin >> s1;

    std::cout << SCAN_SECOND_STR_PROMPT;
    std::cin >> s2;

    int res;
    switch (alg) {
        case LEVENSHTEIN_DISTANCE:
            res = LevenshteinDistance(s1, s2);
            break;
        case LEVENSHTEIN_DISTANCE_SIMPLE:
            res = LevenshteinDistanceSimple(s1, s2, true);
            break;
        case LEVENSHTEIN_DISTANCE_RECURSIVE:
            res = LevenshteinDistanceRecursive(s1, s2, s1.size(), s2.size());
            break;
        case LEVENSHTEIN_DISTANCE_RECURSIVE_CACHE: 
        {
            std::vector<std::vector<int>> cache;
            InitLDRCache(cache, s1.size() + 1, s2.size() + 1);
            res = LevenshteinDistanceRecursiveCache(s1, s2, s1.size(), s2.size(), cache);
        } break;
        case DAMERAU_LEVENSHTEIN_DISTANCE_SIMPLE:
            res = DamerauLevenshteinDistanceSimple(s1, s2);
            break;
        default:
            std::cout << UNKNOWN_COMMAND_MSG;
            break;
    }

    return res;
}

std::string generateString(const size_t len)
{
	srand(time(NULL));
	std::string src = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string res;
	res.resize(len);
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0,src.size() - 1);
	for (size_t i = 0; i < len; ++i)
		res[i] = src[dist(rng)];

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
    std::vector words_len = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 30, 35, 40, 45, 50, 100, 150, 200};
    std::ofstream all_mesurements(OUT_DIR "all_mesurements.txt");

    double res_time_all_mesurements[4];

    for (int len : words_len) {
        std::ofstream lev_dist(OUT_DIR + std::to_string(len) + "lev_dist.txt");
        std::ofstream lev_dist_rec(OUT_DIR + std::to_string(len) + "lev_dist_rec.txt");
        std::ofstream lev_dist_rec_cache(OUT_DIR + std::to_string(len) + "lev_dist_rec_cache.txt");
        std::ofstream dam_lev_dist(OUT_DIR + std::to_string(len) + "dam_lev_dist.txt");

        if (!lev_dist.is_open() || !lev_dist_rec.is_open() || !lev_dist_rec_cache.is_open() || !dam_lev_dist.is_open() || !all_mesurements.is_open())
            std::cout << "File error!" << std::endl;

        long startTime, endTime;

        double res_time_lev_dist[N];
        double res_time_lev_dist_rec[N];
        double res_time_lev_dist_rec_cache[N];
        double res_time_dam_lev_dist[N];

        std::string s1 = generateString(len);
        std::string s2 = generateString(len);
        for (int i = 0; i < N; ++i) {
            startTime = getCpuTime();
            LevenshteinDistanceSimple(s1, s2, false);
            endTime = getCpuTime();
            res_time_lev_dist[i] = (endTime - startTime) / TIMEDIVISION_MS;
            lev_dist << res_time_lev_dist[i] << std::endl;

            if (len <= 10) {
                startTime = getCpuTime();
                LevenshteinDistanceRecursive(s1, s2, len, len);
                endTime = getCpuTime();
                res_time_lev_dist_rec[i] = (endTime - startTime) / TIMEDIVISION_MS;
                lev_dist_rec << res_time_lev_dist_rec[i] << std::endl;
            }

            startTime = getCpuTime();
            std::vector<std::vector<int>> cache;
            InitLDRCache(cache, len + 1, len + 1);
            LevenshteinDistanceRecursiveCache(s1, s2, len, len, cache);
            endTime = getCpuTime();
            res_time_lev_dist_rec_cache[i] = (endTime - startTime) / TIMEDIVISION_MS;
            lev_dist_rec_cache << res_time_lev_dist_rec_cache[i] << std::endl;

            startTime = getCpuTime();
            DamerauLevenshteinDistanceSimple(s1, s2);
            endTime = getCpuTime();
            res_time_dam_lev_dist[i] = (endTime - startTime) / TIMEDIVISION_MS;
            dam_lev_dist << res_time_dam_lev_dist[i] << std::endl;
        }

        for (int i = 0; i < N; ++i) {
            res_time_all_mesurements[0] += res_time_lev_dist[i];
            res_time_all_mesurements[1] += res_time_lev_dist_rec[i];
            res_time_all_mesurements[2] += res_time_lev_dist_rec_cache[i];
            res_time_all_mesurements[3] += res_time_dam_lev_dist[i];
        }

        all_mesurements << len << "\t";
        all_mesurements << res_time_all_mesurements[0] / N << " ";
        all_mesurements << ((len <= 10) ? res_time_all_mesurements[1] / N : 0) << " ";
        all_mesurements << res_time_all_mesurements[2] / N << " ";
        all_mesurements << res_time_all_mesurements[3] / N << " ";
        all_mesurements << std::endl;

        lev_dist.close();
        lev_dist_rec.close();
        lev_dist_rec_cache.close();
        dam_lev_dist.close();
    }

    all_mesurements.close();
}

void MeasureMemAll() {
    std::vector words_len = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 30, 35, 40, 45, 50, 100, 150, 200};
    std::ofstream all_mesurements(OUT_DIR "all_mesurements_data.txt");

    for (int len : words_len) {
        if (!all_mesurements.is_open())
            std::cout << "File error!" << std::endl;

        std::string s1 = generateString(len);
        std::string s2 = generateString(len);

        std::vector<std::vector<int>> cache;
        InitLDRCache(cache, len + 1, len + 1);

        long bytesRec = (2 * len + 2) * (2 * sizeof(std::string&) + 2 * sizeof(int) + 4 * sizeof(int)) + 2 * sizeof(std::string);
        long bytesRecCache = (2 * len + 2) * (2 * sizeof(std::string&) + 2 * sizeof(int) + sizeof(std::vector<std::vector<int>>&) + 4 * sizeof(int)) + 2 * sizeof(std::string) + \
            (len + 1) * (len + 1) * sizeof(int);

        all_mesurements << len << "\t";
        all_mesurements << MemLevenshteinDistanceSimple(s1, s2, false) << " ";
        all_mesurements << bytesRec << " ";
        all_mesurements << bytesRecCache << " ";
        all_mesurements << MemDamerauLevenshteinDistanceSimple(s1, s2) << " ";
        all_mesurements << std::endl;
    }

    all_mesurements.close();
}