#include <stdio.h>
#include <string.h>
#include <wctype.h>

// #FIX this method doesn't count well the amount of words in images 
long countWords(const char *filename) {
    FILE *file = fopen(filename, "r"); // Open the file in read mode
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    long amountWords = 0;
    int thereIsWord = 0;
    wint_t ch; // Variable to store each character read from the file

    // count all words between white spaces
    while((ch = fgetc(file)) != EOF) {
        // check if wide character is whitespace - ' ', \n, \t, \r, etc.
        if (iswspace(ch) && thereIsWord) {
            amountWords++;
            thereIsWord = 0;
        }
        else if (!iswspace(ch))  {   
            thereIsWord = 1;
        }
    }

    // at the end of the file there is a word 
    if (thereIsWord) amountWords++;

    return amountWords;
}

int countLines(const char *filename) {
    FILE *file = fopen(filename, "r"); // Open the file in read mode
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    int amountLines = 0;
    wint_t ch; // Variable to store each character read from the file

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            amountLines++; // Increment the count for each newline character
        }
    }

    fclose(file); // Close the file
    return amountLines;
}

long getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb"); // Open the file in binary mode
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    // Seek to the end of the file
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Failed to seek to end of file");
        fclose(file);
        return -1;
    }

    // Get the current file pointer position, which is the file size in bytes
    long fileSize = ftell(file);
    if (fileSize == -1L) {
        perror("Failed to get file size");
    }

    fclose(file); // Close the file
    return fileSize;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [OPTION] <filename>\n", argv[0]);
        return 1;
    }

    long result;

    if (!strcmp(argv[1], "-c")) {
        result = getFileSize(argv[2]);
    }
    else if (!strcmp(argv[1], "-l")) {
        result = (long)countLines(argv[2]);
    }
    else if (!strcmp(argv[1], "-w")) {
        result = countWords(argv[2]);
    }
    else {
        perror("The [OPTION] passed was wrong");
        result = -1;
    }

    if (result != -1) {
        printf("%ld %s\n", result, argv[2]);
    }

    return 0;
}
