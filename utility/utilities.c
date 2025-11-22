#include <stdio.h>
#include "utilities.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/**
 * Returns a random word representing the txt files we have in our resources
 *
 * @return random word from the local array words[]
 */
char *getRandomWordFileName() {
    char *words[] = {
        "animals",
        "continents",
        "countries",
        "fruits",
        "planets",
        "vegetables"
    };

    int word_count = sizeof(words) / sizeof(words[0]);
    int random_index = rand() % word_count;

    return words[random_index];
}

/**
 * Returns a random word from the specified text file
 *
 * @param fileName name of txt file that we get random word from
 * @return random word from fileName file
 */
char *getRandomWordFromFile(const char *fileName) {
    //look for filename txt file inside resources folder
    char path[256];
    snprintf(path, sizeof(path), "resources/words/%s.txt", fileName);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    //read all lines into array of strings
    char *lines[256]; //max 256 lines possible
    int lineCount = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        //remove new line characters
        buffer[strcspn(buffer, "\n")] = '\0';

        //store each line
        lines[lineCount] = malloc(strlen(buffer) + 1);
        strcpy(lines[lineCount], buffer);
        lineCount++;

        if (lineCount >= 256) break; //prevent overflow
    }

    fclose(file);

    if (lineCount == 0) return NULL; //file would be empty

    //choose random number
    srand(time(NULL));
    int random_index = rand() % (lineCount);

    //choose and copy the random line to return
    char *selectedLine = malloc(strlen(lines[random_index]) + 1);
    strcpy(selectedLine, lines[random_index]);

    //free all others
    for (int i = 0; i < lineCount; i++) {
        free(lines[i]);
    }

    return selectedLine;
}

/**
 * Converts the provided word completely to lower case
 *
 * @param word word
 * @return word converted to lower case
 */
char *stringToLower(char *word) {
    //use tolower char function for entire string
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
    return word;
}

/**
 * Tells if character exists in an array of characters
 *
 * @param array the array containing characters
 * @param ch character that we check if it exists in the array
 * @param length length of the array we pass (indicates the last index we have character at)
 * @return true if ch exists inside array, else false
 */
bool charInArray(const char array[], char ch, int length) {
    // loop thru the array to find the char
    for (int i = 0; i < length; i++) {
        if (array[i] == ch) return true;
    }
    return false;
}

/**
 * Appends provided character at the end of the provided array
 *
 * @param array array of characters
 * @param ch character to append to array
 * @param length index where last character exists, we update this to simulate "appending"
 * @param maxLength maximum length the array was initialised with, needed for preventing out of bounds access
 * @return true if character is successfully appended
 */
bool appendCharToArray(char array[], char ch, int *length, int maxLength) {
    // leave space for null terminator
    if ((*length) >= maxLength - 1) return false;

    array[*length] = ch;
    (*length)++;
    array[*length] = '\0'; // null terminate
    return true;
}

/**
 * Checks if character exists in a string
 *
 * @param str string we check for character's existence in
 * @param ch character that we check if it exists inside above string
 * @return true if character exists in string, else false
 */
bool stringHasChar(const char *str, char ch) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ch) return true;
    }
    return false;
}

/**
 * Makes a full underscore version of source string inside the destination string
 * Used for making a blank word that we reveal over time as the player guesses characters
 *
 * @param dest destination string
 * @param src source string
 */
void copyStringToUnderscores(char *dest, const char *src) {
    // make blank version of the given string
    int len = strlen(src);
    for (int i = 0; i < len; i++) {
        dest[i] = '_';
    }
    dest[len] = '\0';
}

/**
 * Reveals (changes underscore to) actual letter inside "revealed" word if the guess letter exists inside the actual word
 *
 * @param word actual word the player has to guess
 * @param revealed the word that player has guessed thus far (contains underscores where not guessed)
 * @param guess the character that player guessed
 */
void revealGuessedLetter(const char *word, char *revealed, char guess) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == guess) {
            revealed[i] = guess;
        }
    }
}

/**
 * Determines if even a singular blank is inside the revealed word, if yes, word isnt fully "revealed" or guessed
 *
 * @param revealed the word that has underscores as blanks (or ~ as super blank) may have letters if correctly guessed
 * @return true if word has even one _ or ~ else false
 */
bool isWordFullyRevealed(const char *revealed) {
    for (int i = 0; revealed[i] != '\0'; i++) {
        if (revealed[i] == '_' || revealed[i] == '~') {
            return false; // still letters to guess
        }
    }
    return true; // all letters revealed
}

/**
 * Places null terminator at the start of string, as if it had been reset
 *
 * @param str string passed into the function
 */
void resetString(char *str) {
    str[0] = '\0';
}
