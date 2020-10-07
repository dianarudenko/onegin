#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_LETTERS 33
#define NUM_OF_UR 6
#define BUFFER_SIZE 20
#define ERROR -1


struct Lines {
    char * line;
    struct Lines * next;
};

/*! This function is my analogue for strlen from string.h.
 * @param str A string witch length you wish to find out.
 * @return Length of the 'str'. If str == NULL returns -1.
 */

int myStrlen (char * str) {
    if (!str) {
        return -1;
    }
    int count = 0;
    while (*str != '\n') {
        count ++;
        str += sizeof(char);
    }
    return count;
}

/*! This function is my analogue for strcpy from string.h. 
 * @param destination String in which you whant to copy. If destination == NULL \
 * a space for the string is calloced, otherwise realloced.
 * @param sourseString String from wich you want to copy.
 * @return Zero if succsess, -1 if sourseString == NULL.
 */

int myStrcpy (char * destination, char * sourseString) {
    if (sourseString == NULL) {
        return -1;
    }
    if (destination == NULL) {
        destination = calloc(sizeof(char), myStrlen(sourseString));
    } else {
        destination = realloc(destination, myStrlen(sourseString) * sizeof(char));
    }
    for (int i = 0; i < myStrlen(sourseString); i++) {
        destination[i] = sourseString[i];
    }
    return 0;
}

/*! Function to find out whether a symbol is a letter or not.
 * @param The symbol.
 * @return 1(true) if the symbol is a letter, 0(false) otherwise.
 */
int isLetter (char symbol) {
    return ((symbol >= 'A') && (symbol <= 'Z')) || ((symbol >= 'a') && (symbol <= 'z'));
}

/*! This function is my analogue for strcmp from string.h. Compares strings in the params.\
 * Skips all symbolbols in the strings that are not letters. Isn't sensitive for case.
 * @return Zero on secess, -1 if firstStr or secondStr equals NULL.
 */
int myStrcmp (char * firstStr, char * secondStr) {
    int index1 = 0, index2 = 0;
    if ((firstStr == NULL) || (secondStr == NULL)) {
        return -1;
    }
    while ((firstStr[index1] != '\0') && (secondStr[index2] != '\0')) {
        char char1 = 0, char2 = 0;
        // compare only letters, skip other
        while (!isLetter(firstStr[index1]) && (firstStr[index1] != '\0')) {
            index1++;
        }
        while (!isLetter(secondStr[index2]) && (secondStr[index2] != '\0')) {
            index2++;
        }
        char1 = firstStr[index1];
        char2 = secondStr[index2];
        // to lowercase
        if ((char1 >= 'A') && (char1 <= 'Z')) {
            char1 += 'a' - 'A';
        }
        if ((char2 >= 'A') && (char2 <= 'Z')) {
            char2 += 'a' - 'A';
        }
        if (char1 < char2) {
            return -1;
        } else if (char1 > char2) {
            return 1;
        }
        index1++;
        index2++;
    }
    if ((firstStr[index1] == '\0') && (secondStr[index2] == '\0')) {
        return 0;
    } else if (firstStr[index1] != '\0') {
        return 1;
    } else {
        return -1;
    }
    return 0;
}

/*! This function reads a line from file.
 * @param fd Descriptor of the file.
 * @param symbol A pointer to the current symbol from file.
 * @return Read string. If nothing read returns NULL.
 */
char * readLine (int fd, char * symbol) {
    if (read(fd, symbol, sizeof(char)) == 0) {
        *symbol = EOF;
        return NULL;
    }
    if (*symbol == '\n') {
        return NULL;
    }
    char * tmpLine = calloc(BUFFER_SIZE, sizeof(char));
    int count = 0;
    tmpLine[count++] = *symbol;
    int curSize = BUFFER_SIZE;
    while (*symbol != '\n') {
        if (read(fd, symbol, sizeof(char)) == 0) {
            *symbol = EOF;
            break;
        }
        if (count < curSize) {
            tmpLine[count] = *symbol;
        } else {
            curSize += BUFFER_SIZE;
            tmpLine = realloc(tmpLine, curSize);
            tmpLine[count] = *symbol;
        }
        count++;
    }
    if (count < curSize) {
        tmpLine[count] = '\0';
    } else {
        tmpLine = realloc(tmpLine, ++curSize);
        tmpLine[count] = '\0';
    }
    return tmpLine;
}

/*! This function converts a list of Lines structures to an array of strings and cleans up the memory\
 * left from structs.
 * @param list A pointer to the first Lines structure in the dynamic array.
 * @param size Size of the list.
 */
char ** moveToArray (struct Lines * list, int size) {
    char ** linesArr = calloc(sizeof(char *), size);
    for (int i = 0; i < size; i++) {
        linesArr[i] = list->line;
        struct Lines * old = list;
        list = list->next;
        free (old);
    }
    return linesArr;
}

/*! Sorts in alphabetic order small amounts of strings (less then 4).
 * @param lines A pointer to the beggining of strings array.
 * @param linesSize Size of the 'lines' array.
 */
void simpleSort (char ** lines, int linesSize) {
    if (linesSize == 3) {
        // cmp 1 & 2
        if (myStrcmp(lines[0], lines[1]) > 0) {
            char * tmp = lines[0];
            lines[0] = lines[1];
            lines[1] = tmp;
        }
        // cmp 2 & 3
        if (myStrcmp(lines[1], lines[2]) > 0) {
            char * tmp = lines[1];
            lines[1] = lines[2];
            lines[2] = tmp;
        }
        // cmp 1 & 2 again
        if (myStrcmp(lines[0], lines[1]) > 0) {
            char * tmp = lines[0];
            lines[0] = lines[1];
            lines[1] = tmp;
        }
    } else if (linesSize == 2) {
        if (myStrcmp(lines[0], lines[1]) > 0) {
            char * tmp = lines[0];
            lines[0] = lines[1];
            lines[1] = tmp;
        }
    }
    // else return;
}

/*! A quick sort for big amount of strings.
 * @param lines A pointer to the beggining of strings array.
 * @param linesSize Size of the 'lines' array.
 */
void quickSort (char ** lines, int linesSize) {
    if (linesSize < 4) {
        simpleSort(lines, linesSize);
        return;
    }
    // if linesSize % 2 == 0 then choose as the middle one element
    // that is located right after the middle
    int middleIndex = linesSize / 2;
    int leftPointer = 0, rightPointer = linesSize - 1;
    while ((leftPointer < middleIndex) || (rightPointer > middleIndex)) {
        while ((leftPointer < middleIndex)
                && (myStrcmp(lines[leftPointer], lines[middleIndex]) < 0)) {
            leftPointer++;
        }
        while ((rightPointer > middleIndex)
                && (myStrcmp(lines[rightPointer], lines[middleIndex]) >= 0)) {
            rightPointer--;
        }
        char * tmp = lines[leftPointer];
        lines[leftPointer] = lines[rightPointer];
        lines[rightPointer] = tmp;
        if ((leftPointer == middleIndex) && (rightPointer != middleIndex)) {
            middleIndex = rightPointer;
        } else if ((rightPointer == middleIndex) && (leftPointer != middleIndex)) {
            middleIndex = leftPointer;
        }
        if (leftPointer < middleIndex) {
            leftPointer++;
        }
        if (rightPointer > middleIndex) {
            rightPointer--;
        }
    }
    // sort left half
    quickSort(lines, middleIndex);
    // sort right half
    quickSort(&lines[middleIndex + 1], linesSize - middleIndex - 1);
}

/*! A function that reads lines from one file and writes them sorted in alphabitic\
 * order to another file.
 * @param filename Name of file strings from which should be sorted.
 * @param outputFilename Name of file in which result be put.
 */
void sortLines (char * filename, char * outputFilename) {
    struct Lines * lines = calloc(1, sizeof(struct Lines));
    struct Lines * pointer = lines;
    int strCount = 0;
    //open file
    int fd = open(filename, O_RDONLY);
    //copy to tmp_file
    char symbol = 0;
    while (symbol != EOF) {
        char * tmpLine = readLine(fd, &symbol);
        if (tmpLine == NULL) {
            continue;
        }
        strCount++;
        if (pointer->line == NULL) {
            lines->line = tmpLine;
        } else {
            pointer->next = calloc (sizeof(struct Lines), 1);
            pointer = pointer->next;
            pointer->line = tmpLine;
        }
    }
    if (lines->line == NULL) {
        free(lines);
        return;
    }
    char ** linesArray = moveToArray(lines, strCount);
    quickSort (linesArray, strCount);
    close(fd);
    fd = open(outputFilename, O_WRONLY|O_CREAT, 0666);
    for (int i = 0; i < strCount; i ++) {
        char endl = '\n';
        write(fd, linesArray[i], myStrlen(linesArray[i]));
        write(fd, &endl, sizeof(char));
    }
    close(fd);
}

//--------- test functions ----------

void testMyStrcmp () {
    printf("Start testing myStrcmp.\n");
    char * str1 = calloc(sizeof(char), myStrlen("aaaaaaaa"));
    char * str2 = calloc(sizeof(char), myStrlen("BBBBBBBB"));
    int res = 0;
    printf("Test 1\n");
    myStrcpy(str1, "aaaaaaaa");
    myStrcpy(str2, "BBBBBBBB");
    if ((res = myStrcmp(str1, str2)) != -1) {
        printf("ERROR: expexted %d but got %d\n", -1, res);
    } else {
        printf ("OK\n");
    }
    printf("Test 2\n");
    myStrcpy(str1, "'aaa");
    myStrcpy(str2, "aa,a");
    if ((res = myStrcmp(str1, str2)) != 0) {
        printf("ERROR: expexted %d but got %d\n", 0, res);
    } else {
        printf ("OK\n");
    }
    printf("Finish testing myStrcmp.\n");
}

void testSimpleSort() {
    printf("Start testing simpleSort.\n");
    int error = 0;
    printf("Test 1\n");
    char ** strings = calloc(sizeof(char *), 3);
    char ** trueStrings = calloc(sizeof(char *), 3);
    strings[2] = trueStrings[0] = "ccccccccccccc";
    strings[0] = trueStrings[1] = "DDDDDDDDDDDDD";
    strings[1] = trueStrings[2] = "eeeeeeeeeeeee";
    simpleSort(strings, 3);
    for (int i = 0; i < 3; i++) {
        if(myStrcmp(strings[i], trueStrings[i])) {
            printf("ERROR: expexted %s but got %s\n(at string %d)\n", trueStrings[i], strings[i], i);
            error = 1;
        }
    }
    if (!error) {
        printf("OK\n");
    }
    free(strings);
    error = 0;
    printf("Test 2\n");
    strings = calloc(sizeof(char *), 2);
    strings[1] = trueStrings[0] = "aaaaaaaaaaaaa";
    strings[0] = trueStrings[1] = "bbbbbbbbbbbbb";
    simpleSort(strings, 2);
    for (int i = 0; i < 2; i++) {
        if(myStrcmp(strings[i], trueStrings[i])) {
            printf("ERROR: expexted %s but got %s\n(at string %d)\n", trueStrings[i], strings[i], i);
            error = 1;
        }
    }
    if (!error) {
        printf("OK\n");
    }
}

void testQuickSort() {
    testSimpleSort();
    printf("Finish testing simpleSort.\n");
    printf("Start testing quickSort.\n");
    int error = 0;
    char ** strings = calloc(sizeof(char *), 8);
    char ** trueStrings = calloc(sizeof(char *), 8);
    strings[5] = trueStrings[0] = "aaaaaaaaaaaaa";
    strings[7] = trueStrings[1] = "aaabbb";
    strings[6] = trueStrings[2] = "a,'. []!:abbb";
    strings[1] = trueStrings[3] = "bbbbbbbbbbbbb";
    strings[3] = trueStrings[4] = "ccccccccccccc";
    strings[0] = trueStrings[5] = "DDDDDDDDDDDDD";
    strings[4] = trueStrings[6] = "eeeeeeeeeeeee";
    strings[2] = trueStrings[7] = "fffffffffffff";
    quickSort(strings, 8);
    for (int i = 0; i < 8; i++) {
        if(myStrcmp(strings[i], trueStrings[i])) {
            printf("ERROR: expexted %s but got %s\n(at string %d)\n", trueStrings[i], strings[i], i);
            error = 1;
        }
    }
    if (!error) {
        printf("OK\n");
    }
}

/*! Main function. Take two arguments:
 * 1) Name of input file with strings that should be sorted;
 * 2) Name of output file.
 */
int main (int argc, char ** argv) {
    if (argc < 2) {
        printf("ERROR! Not enough arguments.\n");
        return -1;
    }
    sortLines(argv[1], argv[2]);
    testMyStrcmp();
    testQuickSort();
}