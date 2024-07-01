#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <wctype.h>
#include <locale.h>
#include <wchar.h>

/**
 * @brief WC structure containing counted bytes, characters, words and lines
 */
typedef struct  {  
  unsigned long bytes;
  unsigned long chars;
  unsigned long words;
  unsigned long lines;
} WC;

/**
 * @brief function that calculates chars, words and lines 
 * 
 * @param fd file descriptor we are reading from
 * @return WC struct
 */
WC read_from_fd(FILE *fd) {
    WC counter = { .bytes=0, .chars=0, .lines=0, .words=0};

    int thereIsWord = 0;
    char buff[8];
    wint_t ch; // Variable to store each character read from the file

    while ((ch = fgetc(fd)) != WEOF) {
        // count all words between whitespaces
        if (iswspace(ch) && thereIsWord) {
            counter.words++;
            thereIsWord = 0;
        }
        else if (!iswspace(ch))  {   
            thereIsWord = 1;
        } // #FIX this method doesn't count well the amount of words in images 

        // add new line
        if (ch == L'\n') counter.lines++;

        // add character
        counter.chars++;

        // convert wide character to multibyte and add the lenght to the sum of bytes
        counter.bytes += wctomb(buff, ch);
    }

    return counter;
}

int main(int argc, char *argv[]) {
    // Locale must be set in order to properly work with wide characters
    setlocale(LC_ALL, "");

    char *filename = NULL;
    if (argc == 3) filename = argv[2];

    FILE *fd;

    if (filename) {
        fd = fopen(argv[2], "r"); // Open the file
        if (fd == NULL) {
            perror("Failed to open file");
            return -1;
        }
    }
    else if (filename) {
        fd = stdin;
    }

    WC counter = read_from_fd(fd);

    fclose(fd);

    long result;
    if (!strcmp(argv[1], "-c")) {
        printf("%ld", counter.bytes);
    }
    else if (!strcmp(argv[1], "-l")) {
        printf("%ld", counter.lines);
    }
    else if (!strcmp(argv[1], "-w")) {
        printf("%ld", counter.words);
    }
    else if (!strcmp(argv[1], "-m")) {
        printf("%ld", counter.chars);
    }
    if(filename) printf(" %s", filename);

    printf("\n");

    return 0;
}
