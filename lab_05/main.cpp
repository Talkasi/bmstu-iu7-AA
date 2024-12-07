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
#include <chrono>

#define MAX_KEYS 256

typedef struct {
    char key[64];
    int value;
} KeyValue;

typedef struct {
    std::vector<KeyValue> kv_array;
    bool done;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
} DataChunk;

std::queue<DataChunk> inputQueue;
std::queue<DataChunk> outputQueue;
sem_t inputSemaphore;
sem_t outputSemaphore;
pthread_mutex_t inputMutex;
pthread_mutex_t outputMutex;
int n_groups;

void* read_data(void* arg);
void* distribute_keys(void* arg);
void* write_groups(void* arg);

int main() {
    const char* input_file = "data/in.txt";
    n_groups = sysconf(_SC_NPROCESSORS_ONLN);

    sem_init(&inputSemaphore, 0, 0);
    sem_init(&outputSemaphore, 0, 0);
    pthread_mutex_init(&inputMutex, NULL);
    pthread_mutex_init(&outputMutex, NULL);

    pthread_t readerThread, distributorThread, writerThread;

    auto start_time = std::chrono::high_resolution_clock::now();

    pthread_create(&readerThread, NULL, read_data, (void*)input_file);
    pthread_create(&distributorThread, NULL, distribute_keys, NULL);
    pthread_create(&writerThread, NULL, write_groups, NULL);

    pthread_join(readerThread, NULL);
    pthread_join(distributorThread, NULL);
    pthread_join(writerThread, NULL);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    std::cout << "Total elapsed time: " << elapsed_time.count() << " seconds." << std::endl;

    sem_destroy(&inputSemaphore);
    sem_destroy(&outputSemaphore);
    pthread_mutex_destroy(&inputMutex);
    pthread_mutex_destroy(&outputMutex);
    return 0;
}

void* read_data(void* arg) {
    const char* filename = (const char*)arg;
    std::ifstream file(filename);
    DataChunk chunk;
    chunk.done = false;

    if (!file.is_open()) {
        perror("Failed to open input file");
        return NULL;
    }

    KeyValue tmp_kv;
    while (file.getline(tmp_kv.key, sizeof(tmp_kv.key))) {
        char* colon = strchr(tmp_kv.key, ':');
        if (colon) {
            *colon = '\0';
            tmp_kv.value = atoi(colon + 1);
            chunk.kv_array.push_back(tmp_kv);
        }

        chunk.start_time = std::chrono::high_resolution_clock::now();

        pthread_mutex_lock(&inputMutex);
        inputQueue.push(chunk);
        pthread_mutex_unlock(&inputMutex);
        sem_post(&inputSemaphore);

        chunk.kv_array.clear();
    }

    chunk.done = true;
    pthread_mutex_lock(&inputMutex);
    inputQueue.push(chunk);
    pthread_mutex_unlock(&inputMutex);
    sem_post(&inputSemaphore);

    return NULL;
}

void* distribute_keys(void* arg) {
    std::vector<KeyValue> groups[n_groups];
    std::vector<int> groupSums(n_groups, 0);

    while (true) {
        sem_wait(&inputSemaphore);
        pthread_mutex_lock(&inputMutex);

        if (inputQueue.empty()) {
            pthread_mutex_unlock(&inputMutex);
            continue;
        }

        DataChunk chunk = inputQueue.front();
        inputQueue.pop();
        pthread_mutex_unlock(&inputMutex);

        if (chunk.done) {
            break;
        }

        for (const auto& kv : chunk.kv_array) {
            int min_group_index = 0;
            for (int i = 1; i < n_groups; ++i) {
                if (groupSums[i] < groupSums[min_group_index]) {
                    min_group_index = i;
                }
            }

            auto& group = groups[min_group_index];
            auto it = std::lower_bound(group.begin(), group.end(), kv, [](const KeyValue& a, const KeyValue& b) {
                return strcmp(a.key, b.key) < 0;
            });
            group.insert(it, kv);
            groupSums[min_group_index] += kv.value;
        }
    }

    for (int i = 0; i < n_groups; ++i) {
        DataChunk outputChunk;
        outputChunk.kv_array = groups[i];
        outputChunk.done = false;

        pthread_mutex_lock(&outputMutex);
        outputQueue.push(outputChunk);
        pthread_mutex_unlock(&outputMutex);
        sem_post(&outputSemaphore);
    }

    DataChunk doneChunk;
    doneChunk.done = true;
    pthread_mutex_lock(&outputMutex);
    outputQueue.push(doneChunk);
    pthread_mutex_unlock(&outputMutex);
    sem_post(&outputSemaphore);

    return NULL;
}

void* write_groups(void* arg) {
    int groupIndex = 0;
    double total_write_time = 0.0;
    int count = 0;

    while (true) {
        sem_wait(&outputSemaphore);
        pthread_mutex_lock(&outputMutex);

        if (outputQueue.empty()) {
            pthread_mutex_unlock(&outputMutex);
            continue;
        }

        DataChunk chunk = outputQueue.front();
        outputQueue.pop();
        pthread_mutex_unlock(&outputMutex);

        if (chunk.done) {
            break;
        }

        char output_filename[256];
        snprintf(output_filename, sizeof(output_filename), "group_%d.txt", groupIndex++);
        std::ofstream out_file(output_filename);
        if (!out_file.is_open()) {
            perror("Failed to open output file");
            continue;
        }

        int sum = 0;
        for (const auto& kv : chunk.kv_array) {
            sum += kv.value;
        }

        std::cout << "Sum of group " << groupIndex - 1 << ": " << sum << std::endl;

        auto write_start_time = std::chrono::high_resolution_clock::now();

        out_file << sum << std::endl;
        for (const auto& kv : chunk.kv_array) {
            out_file << kv.key << ": " << kv.value << std::endl;
        }

        out_file.close();

        auto write_end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> write_time = write_end_time - write_start_time;
        total_write_time += write_time.count();
        count++;
    }

    if (count > 0) {
        std::cout << "Average write time per group: " << (total_write_time / count) << " seconds." << std::endl;
    }

    return NULL;
}
