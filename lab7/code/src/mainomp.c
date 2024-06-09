#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <coz.h>

int find_sequence_in_chunk(FILE *file, const char *sequence, long start, long end) {
    size_t sequence_length = strlen(sequence);
    size_t match_count = 0;
    int ch;

    fseek(file, start, SEEK_SET);
    while (ftell(file) < end && (ch = fgetc(file)) != EOF) {
        COZ_PROGRESS;
        if (ch == sequence[match_count]) {
            match_count++;
            if (match_count == sequence_length) {
                return 1;
            }
        } else {
            match_count = (ch == sequence[0]) ? 1 : 0;
        }
    }

    return 0;
}

int find_sequence_in_file(const char *filename, const char *sequence) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    long chunk_size = filesize / omp_get_max_threads();

    int found = 0;

    #pragma omp parallel shared(found)
    {
        int tid = omp_get_thread_num();
        long start = tid * chunk_size;
        long end = (tid == omp_get_max_threads() - 1) ? filesize : start + chunk_size;

        if (found == 0) {
            FILE *file_chunk = fopen(filename, "r");
            if (file_chunk != NULL) {
                int result = find_sequence_in_chunk(file_chunk, sequence, start, end);
                fclose(file_chunk);
                if (result) {
                    #pragma omp critical
                    {
                        found = 1;
                    }
                }
            }
        }
        COZ_PROGRESS;
    }

    fclose(file);
    return found;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <sequence>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    const char *sequence = argv[2];

    int found = find_sequence_in_file(filename, sequence);
    if (found) {
        printf("Sequence found\n");
    } else {
        printf("Sequence not found\n");
    }

    return EXIT_SUCCESS;
}

