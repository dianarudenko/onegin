#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_LETTERS 33
#define NUM_OF_UR 6
#define BUFFER_SIZE 20

// typedef struct Char {
//     char sym;
//     struct Char * next;
// };

void sortLines (char * filename) {
    // +1 place for lines start with not a letter
    char * alphabeticLines = calloc(NUM_OF_LETTERS + 1, sizeof(char));
    //open file
    int fd = open(filename, O_RDONLY);
    //copy to tmp_file
    char symbol;
    while (read(fd, &symbol, sizeof(char))) {
        int number = 0;
        if ((symbol >= 'ё') && (symbol <= 'Ё')) {
            number = symbol - 'A';
            if (number >= NUM_OF_UR) {
                number++;
            }
        } else if ((symbol >= 'ё') && (symbol <= 'Ё')) {
            number = symbol - 'a';
            if (number >= NUM_OF_UR) {
                number++;
            }
        } else if ((symbol == 'ё') || (symbol == 'Ё')) {
            number = NUM_OF_UR;
        } else {
            number = NUM_OF_LETTERS;
        }
        char * tmpLine = calloc(BUFFER_SIZE, sizeof(char));
        //alphabeticLines[number].sym = symbol;
        int count = 0;
        int curSize = BUFFER_SIZE;
        while (symbol != '\n') {
            //struct Char * pointer = calloc(1, sizeof(struct Char));
            //alphabeticLines[number].next = pointer;
            read(fd, &symbol, sizeof(char));
            if (count < curSize) {
                tmpLine[count] = symbol;
            } else {
                curSize += BUFFER_SIZE;
                tmpLine = realloc(tmpLine, curSize);
                tmpLine[count] = symbol;
            }
            count++;
        }
        if (alphabeticLines[number] == NULL) {
            alphabeticLines[number] = tmpLine;
        } else {
            if (strcmp(alphabeticLines[number], tmpLine) > 0) {
                alphabeticLines[number] = tmpLine;
            }
        }
    }
}

int main (void) {
    //open file
    //sort strings
}