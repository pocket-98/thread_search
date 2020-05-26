/*
 * test using threads to parallelize searching documents
 * @file main.cpp
 * @author Pavan Dayal
 */

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "sync_reader.hpp"

#define LINE_LENGTH 1200

#ifndef NUM_THREADS
#define NUM_THREADS 2
#endif

// print help usage and example
void print_help(const char* prgm) {
    fprintf(stderr, "usage: %s <file> <search_phrase>\n", prgm);
    fprintf(stderr, "example: %s wordy.txt app\n", prgm);
}

// count needles in haystack (with 5 delay)
unsigned int count_occurences(const char* line, const char* word);

// holds file pointer and array of threads
struct thread_data {
    unsigned int* counts;
    char (*lines)[LINE_LENGTH]; // ptr to char[1200]
    const char* phrase;
    sync::reader* reader;
};

// get line search to be run on each thread
void thread_search(int id, thread_data& data);

// main
int main(int argc, char* argv[]) {
    // err messages
    int j;
    if (argc < 2) {
        fprintf(stderr, "error: supply file to parse\n");
        print_help(argv[0]);
        return EXIT_FAILURE;
    } else if (argc < 3) {
        fprintf(stderr, "error: supply search phrase\n");
        print_help(argv[0]);
        return EXIT_FAILURE;
    }

    // arguments
    const char* filestr = argv[1];
    const char* phrase = argv[2];


    // count lines
    printf("searching %s for phrase %s\n", filestr, phrase);
    FILE* file = fopen(filestr, "r");
    unsigned int num_lines = 0;
    size_t n = LINE_LENGTH;
    char line[LINE_LENGTH];
    char* l = line;
    while (-1 != getline(&l, &n, file)) {
        ++num_lines;
    }
    printf("num lines: %u\n", num_lines);
    fseek(file, 0, SEEK_SET);

    // create threads and thread data
    std::thread* threads[NUM_THREADS];
    unsigned int counts[NUM_THREADS];
    char linetmp[NUM_THREADS][LINE_LENGTH];
    sync::reader reader(file);

    thread_data data = {counts, linetmp, phrase, &reader};
    for (j=0; j<NUM_THREADS; ++j) {
        counts[j] = 0;
        threads[j] = new std::thread(thread_search, j, std::ref(data));
    }

    // wait for all threads to finish
    for (j=0; j<NUM_THREADS; ++j) {
        threads[j]->join();
    }

    // print counts per thread as well as total count
    unsigned int c = 0;
    for (j=0; j<NUM_THREADS; ++j) {
        printf("%d> %u\n", j, counts[j]);
        c += counts[j];
    }
    printf("num occurences: %u\n", c);

    fclose(file);
    return EXIT_SUCCESS;
}

unsigned int count_occurences(const char* line, const char* word) {
    unsigned int count = 0;
    char j, k;
    int index = 0;
    bool matches = false;
    while ((j = *(line++))) {
        k = word[index++]; // needle current char
        if (!k && matches) { // end of needle and all chars match
            ++count;
            k = word[0];
            index = 1;
        }
        if (j != k) { // needle isnt matching haystack rn
            matches = false;
            index = 0;
        } else {
            matches = true;
        }
    }
    if (!word[index] && matches) { //if matched up to last char
        ++count;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    return count;
}

// keep getting lines and adding up occurrences independently for each thread
void thread_search(int id, thread_data& data) {
    // read a line from the file to threads private data
    bool got_line = data.reader->readline(data.lines[id], LINE_LENGTH);
    while (got_line) {
        // save count in threads own data
        data.counts[id] += count_occurences(data.lines[id], data.phrase);
        // get another line
        got_line = data.reader->readline(data.lines[id], LINE_LENGTH);
    }
}
