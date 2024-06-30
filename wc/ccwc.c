#include <stdio.h>

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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    long fileSize = getFileSize(argv[1]);
    if (fileSize != -1) {
        printf("%ld %s\n", fileSize, argv[1]);
    }

    return 0;
}
