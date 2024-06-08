// ChatGPT generated program

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_random_digits(int x, const char *filename) {
    // Initialize the random number generator with the current time as the seed
    srand(time(NULL));

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < x; i++) {
        // Generate a random digit between 0 and 9
        int digit = rand() % 10;
        // Write the digit to the file
        fprintf(file, "%d", digit);
    }

    // Close the file
    fclose(file);
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_digits> <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse the number of digits
    int x = atoi(argv[1]);
    if (x <= 0) {
        fprintf(stderr, "The number of digits must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    // Get the filename
    const char *filename = argv[2];

    // Generate and write the random digits to the file
    generate_random_digits(x, filename);

    return EXIT_SUCCESS;
}
