#include <iostream>
#include <vector>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <map>
#include <fstream>

#define MAX_KEYS 256
#define MAX_LINE_LENGTH 256
#define QUEUE_SIZE 10

typedef struct {
    char key[64];
    int value;
} KeyValue;

typedef struct {
    KeyValue kv[MAX_KEYS];
    int n;
    int sum;
} Group;

void process_without_threads(FILE *f, long n);
void read_data_from_file(std::vector<KeyValue> &kv_array, FILE *f);
void init_groups(Group *g, int n_groups, std::vector<KeyValue> kv_array);
void write_groups_to_files(Group *g, int n_groups, const char *filename);
void distribute_keys_to_groups(Group *g, int n_groups, std::vector<KeyValue> kv_array);

int main() {
    char input_file[100] = "in.txt";
    long n = sysconf(_SC_NPROCESSORS_ONLN);

    FILE *f = fopen(input_file, "r");
    if (!f) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    clock_t start_time = clock();

    process_without_threads(f, n);

    clock_t end_time = clock();
    double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    std::cout << "Elapsed time: " << elapsed_time << " seconds." << std::endl;

    fclose(f);
    return EXIT_SUCCESS;
}

void process_without_threads(FILE *f, long n_groups) {
    std::vector<KeyValue> kv_array;
    read_data_from_file(kv_array, f);

    Group *g = (Group *)malloc(n_groups * sizeof(Group));
    init_groups(g, n_groups, kv_array);

    distribute_keys_to_groups(g, n_groups, kv_array);
    write_groups_to_files(g, n_groups, "in.txt");

    free(g);
}

void read_data_from_file(std::vector<KeyValue> &kv_array, FILE *f) {
    KeyValue tmp_kv;
    while (fgets(tmp_kv.key, sizeof(tmp_kv.key), f)) {
        char *colon = strchr(tmp_kv.key, ':');
        if (colon) {
            *colon = '\0';
            tmp_kv.value = atoi(colon + 1);
            kv_array.emplace_back(tmp_kv);
        }
    }
}

void init_groups(Group *g, int n_groups, std::vector<KeyValue> kv_array) {
    for (int i = 0; i < n_groups; ++i) {
        g[i].n = 0;
        g[i].sum = 0;
    }
}

void distribute_keys_to_groups(Group *g, int n_groups, std::vector<KeyValue> kv_array) {
    std::sort(kv_array.begin(), kv_array.end(), [](const KeyValue &a, const KeyValue &b) {
        return a.value < b.value;
    });

    for (const auto &kv : kv_array) {
        int min_group_index = 0;
        for (int i = 1; i < n_groups; ++i) {
            if (g[i].sum < g[min_group_index].sum) {
                min_group_index = i;
            }
        }

        g[min_group_index].kv[g[min_group_index].n++] = kv;
        g[min_group_index].sum += kv.value;
    }
}

void write_groups_to_files(Group *g, int n_groups, const char *filename) {
    char output_filename[256];
    for (int i = 0; i < n_groups; ++i) {
        snprintf(output_filename, sizeof(output_filename), "group_%d.%s.txt", i, filename);
        FILE *out_f = fopen(output_filename, "w");
        if (!out_f) {
            perror("Failed to open output file");
            continue;
        }

        std::sort(g[i].kv, g[i].kv + g[i].n, [](const KeyValue &a, const KeyValue &b) {
            return strcmp(a.key, b.key) < 0;
        });

        fprintf(out_f, "%d\n", g[i].sum);
        for (int j = 0; j < g[i].n; ++j) {
            fprintf(out_f, "%s: %d\n", g[i].kv[j].key, g[i].kv[j].value);
        }

        fclose(out_f);
    }
}